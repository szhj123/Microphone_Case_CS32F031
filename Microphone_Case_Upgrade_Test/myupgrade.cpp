#include "myupgrade.h"

#define UPG_INTERVAL_TIME                 15//ms
#define UPG_ERASE_TIMEOUT                5000 //ms


MyUpgrade::MyUpgrade(QWidget *parent) : QWidget(parent)
{
    this->fwVerRecvFlag = false;
}

void MyUpgrade::Upg_Init(Ui::MainWindow *ui, MySerialPort *serialPort)
{
    this->ui = ui;
    this->serialPort = serialPort;

    connect(ui->btnAddFw, SIGNAL(clicked()), this, SLOT(on_btnAddFw_Clicked()));
    connect(ui->btnUpgEN, SIGNAL(clicked()), this, SLOT(on_btnUpgEn_Clicked()));
    connect(ui->btnGetFw, SIGNAL(clicked()), this, SLOT(on_btnGetFw_Clicked()));
}

void MyUpgrade::Upg_Set_Ver(char fwBuildVer, char fwMinorVer, char fwMajorVer)
{
    QString str = "0.0.0";

    str[0] = fwBuildVer + 0x30;
    str[2] = fwMinorVer + 0x30;
    str[4] = fwMajorVer + 0x30;

    fwVerRecvFlag = true;

    qDebug() << str;

    ui->labelFwVer->setText(str);
}



void InvertUint16(uint16_t *poly )
{
    uint8_t i;
    uint16_t tmp = 0;
    uint16_t polyVal = *poly;

    for(i=0;i<16;i++)
    {
        if(polyVal & (1 << i))
            tmp |= 1 << (15-i);
    }
    *poly = tmp;
}

uint16_t MyUpgrade::Upg_Cal_Checksum(uint8_t *buf, uint32_t length)
{
    uint16_t wCRCin = 0x0000;
    uint16_t wCPoly = 0x8005;
    uint8_t i;

    InvertUint16(&wCPoly);

    while(length--)
    {
        wCRCin ^= *(buf++);
        for(i=0;i<8;i++)
        {

            if(wCRCin & 0x0001)
                wCRCin = (wCRCin >> 1) ^ wCPoly;
            else
                wCRCin >>= 1;
        }
    }
    return wCRCin;
}


void MyUpgrade::on_btnAddFw_Clicked(void )
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open Upgrade Firmware"),QDir::homePath(),tr("(*.bin)"));

    ui->lineEditFwPath->setText(filePath);

    ui->lineEditFwPath->setReadOnly(true);

    if(serialPort->Serial_Port_Get_Opened())
    {
        QFile readFile(ui->lineEditFwPath->text());

        QFileInfo readFileInfo(readFile);

        if(!readFile.open(QIODevice::ReadOnly))
        {
            //QMessageBox::warning(this, tr("Read File)"), tr("Cannot open file:\n%1").arg(ui->lineEditFwPath->text()));

            return ;
        }

        QDataStream binFileData(&readFile);

        fwInfo.fwSize = readFileInfo.size();

        static char *pBuf;

        if(pBuf != nullptr)
        {
            delete []  pBuf;
        }

        pBuf = new char[fwInfo.fwSize];

        binFileData.readRawData(pBuf, static_cast<int>(fwInfo.fwSize));

        fwInfo.fwArray = QByteArray(pBuf, static_cast<int>(fwInfo.fwSize));

        fwInfo.fwBuf = (char *)fwInfo.fwArray.data();

        fwInfo.fwOffset = 0;

        ui->upgProgressBar->Update_Val(0);

        ui->btnUpgEN->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请打开串口!!!\n"));

        return ;
    }
}

void MyUpgrade::on_btnUpgEn_Clicked()
{

}

void MyUpgrade::on_btnGetFw_Clicked(void )
{

}

void MyUpgrade::Upg_Handler(uint8_t *buf, uint16_t length)
{
    uint8_t cmd = buf[0];

    switch(cmd)
    {
        case CMD_CASE_OPEN:  serialPort->Serial_Port_Case_Open();break;
        case CMD_CASE_CLOSE: serialPort->Serial_Port_Case_Close();break;
        case CMD_CHAG_OFF:   serialPort->Serial_Port_Chrg_Off();break;
        case CMD_CASE_BATT:  serialPort->Serial_Port_Case_Batt();break;
        case CDM_FW_SIZE:    serialPort->Serial_Port_Fw_Size(fwInfo.fwSize);break;
        case CMD_FW_DATA:
        {
            int offset = (int)buf[6] << 24 | (int)buf[5] << 16 | (int)buf[4] << 8 | (int)buf[3];
            int length = (int)buf[10] << 24 | (int)buf[9] << 16 | (int)buf[8] << 8 | (int)buf[7];

            if(offset != fwInfo.fwOffset)
            {
                fwInfo.fwOffset = offset;

                fwInfo.fwBuf += offset;
            }

            serialPort->Serial_Port_Fw_Data(fwInfo.fwBuf, length);

            uint8_t progressVal = fwInfo.fwOffset *100 / fwInfo.fwArray.length();

            if(progressVal >= 100)
            {
                progressVal = 99;
            }

            qDebug() << QString().sprintf("firmware data offset:%d", fwInfo.fwOffset);

            ui->upgProgressBar->Update_Val(progressVal);

            break;
        }
        case CMD_FW_CRC: break;
        case CMD_FW_VER: serialPort->Serial_Port_Fw_Ver();break;
        default:break;
    }
}



