#include "myupgrade.h"

#define UPG_INTERVAL_TIME                10 //ms
#define UPG_ERASE_TIMEOUT                5000 //ms

static fw_info_t   fwInfo;
static upg_state_t upgState;

MyUpgrade::MyUpgrade(QWidget *parent) : QWidget(parent)
{

}

void MyUpgrade::Upg_Init(Ui::MainWindow *ui, MySerialPort *serialPort)
{
    this->ui = ui;
    this->serialPort = serialPort;

    connect(ui->btnAddFw, SIGNAL(clicked()), this, SLOT(on_btnAddFw_Clicked()));
    connect(ui->btnUpgEN, SIGNAL(clicked()), this, SLOT(on_btnUpgEn_Clicked()));
    connect(ui->btnGetFw, SIGNAL(clicked()), this, SLOT(on_btnGetFw_Clicked()));

    timer = new QTimer(this);
    timer->setInterval(UPG_INTERVAL_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(upg_handler()));
}

void MyUpgrade::Upg_Set_Ver(char fwBuildVer, char fwMinorVer, char fwMajorVer)
{
    QString str = "0.0.0";

    str[0] = fwBuildVer + 0x30;
    str[2] = fwMinorVer + 0x30;
    str[3] = fwMajorVer + 0x30;

    fwVerRecvFlag = true;

    ui->labelFwVer->setText(str);
}

void MyUpgrade::Upg_Set_Ack(uchar ack)
{
    this->ack = ack;
}

void MyUpgrade::Upg_Clr_Ack()
{
    this->ack = 0 ;
}

uchar MyUpgrade::Upg_Get_Ack()
{
    return this->ack;
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

    QFile readFile(ui->lineEditFwPath->text());

    QFileInfo readFileInfo(readFile);

    if(!readFile.open(QIODevice::ReadOnly))
    {
        //QMessageBox::warning(this, tr("Read File)"), tr("Cannot open file:\n%1").arg(ui->lineEditFwPath->text()));

        return ;
    }

    QDataStream binFileData(&readFile);

    static char *pBuf = nullptr;

    fwInfo.fwSize = readFileInfo.size();

    if(pBuf != nullptr)
    {
        delete [] pBuf;
    }

    pBuf = new char[fwInfo.fwSize];
    binFileData.readRawData(pBuf, static_cast<int>(fwInfo.fwSize));
    fwInfo.fwArray = QByteArray(pBuf, static_cast<int>(fwInfo.fwSize));
    fwInfo.fwBuf = (char *)fwInfo.fwArray.data();

}

void MyUpgrade::on_btnUpgEn_Clicked()
{
    upgState = UPG_STATE_ERASE_FLASH;

    if(serialPort->Serial_Port_Get_Opened())
    {

        ui->upgProgressBar->Update_Val(0);

        timer->start();
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请打开串口!!!\n"));

        return ;
    }
}

void MyUpgrade::on_btnGetFw_Clicked(void )
{

}

void MyUpgrade::upg_handler(void )
{
    switch(upgState)
    {
        case UPG_STATE_ERASE_FLASH:
        {
            serialPort->Serial_Port_Fw_Erase(fwInfo.fwSize);

            fwInfo.fwTxTimeoutCnt = 0;

            qDebug() << QString().sprintf("firmware size:%d", fwInfo.fwSize);

            upgState = UPG_STATE_WAIT_ACK_FOR_ERASE;

            break;
        }
        case UPG_STATE_WAIT_ACK_FOR_ERASE:
        {
            if(Upg_Get_Ack() == 0x01)
            {
                Upg_Clr_Ack();

                fwInfo.fwTxTimeoutCnt = 0;

                fwInfo.fwOffset = 0;

                upgState = UPG_STATE_TX_FW_DATA;
            }
            else
            {
                if(++fwInfo.fwTxTimeoutCnt >= (UPG_ERASE_TIMEOUT / UPG_INTERVAL_TIME))
                {
                    fwInfo.fwTxTimeoutCnt = 0;

                    upgState = UPG_STATE_ERASE_FLASH;

                    timer->stop();
                }
            }
            break;
        }
        case UPG_STATE_TX_FW_DATA:
        {
            if(fwInfo.fwSize > FW_MAX_DATA_LENGTH)
            {
                serialPort->Serial_Port_Tx_Data(fwInfo.fwOffset, fwInfo.fwBuf, FW_MAX_DATA_LENGTH);
            }
            else
            {
                serialPort->Serial_Port_Tx_Data(fwInfo.fwOffset, fwInfo.fwBuf, fwInfo.fwSize);
            }

            upgState = UPG_STATE_WAIT_ACK_FOR_TX;
            break;
        }
        case UPG_STATE_WAIT_ACK_FOR_TX:
        {
            if(Upg_Get_Ack() == 0x01)
            {
                Upg_Clr_Ack();

                fwInfo.fwTxTimeoutCnt = 0;
                fwInfo.fwTxErrCnt = 0;

                if(fwInfo.fwSize > FW_MAX_DATA_LENGTH)
                {
                    fwInfo.fwSize -= FW_MAX_DATA_LENGTH;
                    fwInfo.fwOffset += FW_MAX_DATA_LENGTH;
                    fwInfo.fwBuf += FW_MAX_DATA_LENGTH;

                    upgState = UPG_STATE_TX_FW_DATA;
                }
                else
                {
                    fwInfo.fwOffset += fwInfo.fwSize;
                    fwInfo.fwSize = 0;
                    fwInfo.fwBuf = nullptr;

                    upgState = UPG_STATE_TX_FW_CHECKSUM;
                }

                uint8_t progressVal = fwInfo.fwOffset *100 / fwInfo.fwArray.length();

                if(progressVal >= 100)
                {
                    progressVal = 99;
                }

                qDebug() << QString().sprintf("firmware data offset:%d", fwInfo.fwOffset);

                ui->upgProgressBar->Update_Val(progressVal);
            }
            else
            {
                if(++fwInfo.fwTxTimeoutCnt >= (50 / UPG_INTERVAL_TIME))
                {
                    fwInfo.fwTxTimeoutCnt = 0;

                    upgState = UPG_STATE_TX_FW_DATA;

                    if(++fwInfo.fwTxErrCnt >= 10)
                    {
                        upgState = UPG_STATE_ERASE_FLASH;

                        QMessageBox::warning(this, tr("Upgrade State"),tr("Upgrade Error, it's timeout for transmiting data"));

                        timer->stop();
                    }
                }
            }
            break;
        }
        case UPG_STATE_TX_FW_CHECKSUM:
        {
            fwInfo.fwBuf = (char *)fwInfo.fwArray.data();

            fwInfo.fwSize = fwInfo.fwArray.length();

            uint16_t checksum = Upg_Cal_Checksum((uint8_t *)fwInfo.fwBuf, fwInfo.fwSize);

            serialPort->Serial_Port_Tx_Checksum((int )checksum);

            upgState = UPG_STATE_WATI_ACK_FOR_CHECKSUM;

            break;
        }
        case UPG_STATE_WATI_ACK_FOR_CHECKSUM:
        {
            if(Upg_Get_Ack() == 0x01)
            {
                Upg_Clr_Ack();

                fwInfo.fwTxTimeoutCnt = 0;

                fwInfo.fwTxErrCnt = 0;

                serialPort->Serial_Port_Tx_Reset();

                upgState = UPG_STATE_TX_GET_VERSION;
            }
            else
            {
                if(++fwInfo.fwTxTimeoutCnt >= (500 / UPG_INTERVAL_TIME))
                {
                    fwInfo.fwTxTimeoutCnt = 0;

                    upgState = UPG_STATE_TX_FW_CHECKSUM;

                    if(++fwInfo.fwTxErrCnt >= 10)
                    {
                        upgState = UPG_STATE_ERASE_FLASH;

                        QMessageBox::warning(this, tr("Upgrade State"),tr("Upgrade Error, it's timeout for getting correct checksum"));

                        timer->stop();
                    }
                }
            }
            break;
        }
        case UPG_STATE_TX_GET_VERSION:
        {
            fwVerRecvFlag = false;

            serialPort->Serial_Port_Get_Version();

            upgState = UPG_STATE_WAIT_FW_VERSION;

            break;
        }
        case UPG_STATE_WAIT_FW_VERSION:
        {
            if(fwVerRecvFlag)
            {
                upgState = UPG_STATE_IDLE;

                ui->upgProgressBar->Update_Val(100);

                QMessageBox::warning(this,tr("Upgrade State"), tr("Upgrade Successful!"));

                timer->stop();
            }
            else
            {
                if(++fwInfo.fwTxTimeoutCnt >= (1000 / UPG_INTERVAL_TIME))
                {
                    fwInfo.fwTxTimeoutCnt = 0;

                    upgState = UPG_STATE_TX_GET_VERSION;

                    if(++fwInfo.fwTxErrCnt >= 10)
                    {
                        upgState = UPG_STATE_ERASE_FLASH;

                        QMessageBox::warning(this, tr("Upgrade State"),tr("Upgrade Error, it's timeout for getting correct checksum"));

                        timer->stop();
                    }
                }
            }
            break;
        }
        default: break;
    }
}

