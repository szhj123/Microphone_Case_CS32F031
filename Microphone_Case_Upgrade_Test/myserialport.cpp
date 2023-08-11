#include "mainwindow.h"
#include "myserialport.h"
#include "myupgrade.h"

MySerialPort::MySerialPort(QWidget *parent) : QWidget(parent)
{
    serialPortOpened = false;

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(Serial_Port_Monitor()));
    timer->start();

    pTimerRecv = new QTimer(this);
    pTimerRecv->setTimerType(Qt::PreciseTimer);
    pTimerRecv->setSingleShot(true);
    connect(pTimerRecv, SIGNAL(timeout()), this, SLOT(Serial_Port_Recv()));
}

void MySerialPort::Serial_Port_Init(Ui::MainWindow *ui)
{
    //创建串口串口对象
    this->ui = ui;
    serialPort = new QSerialPort(this);
    //数据接收处理
    connect(serialPort,&QSerialPort::readyRead,this,&MySerialPort::Serial_Port_Timeout_ReadyRead);

    QStringList slist;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //检测是否可用
        if(!info.isBusy())
            slist<<info.portName();
    }
    if(slist.isEmpty()){
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }

    ui->serialPortNum->clear();
    ui->serialPortNum->addItems(slist);
}

void MySerialPort::Serial_Port_Refresh(void )
{
    if(serialPortOpened)
    {
        ui->serialPortNum->clear();

        Serial_Port_Init(ui);
    }
}

void MySerialPort::Serial_Port_Monitor(void )
{
    static uint8_t serialPortNum = 0;

    QStringList slist;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //检测是否可用
        if(!info.isBusy())
            slist<<info.portName();
    }

    if(slist.size() != serialPortNum)
    {
        serialPortNum = slist.size();

        ui->serialPortNum->clear();
        ui->serialPortNum->addItems(slist);
    }
}

void MySerialPort::Serial_Port_Open(void )
{
    const QString portnameStr=ui->serialPortNum->currentText();

    if(!portnameStr.isEmpty())
    {
        QSerialPortInfo info(portnameStr);

        if(info.isBusy())
        {
            qDebug()<<"当前串口繁忙,可能已被占用,请确认后再连接"<<portnameStr;
            return;
        }

        qint32 baudrate=ui->serialPortBaud->currentText().toInt();

        QSerialPort::DataBits databit = QSerialPort::Data8;

        QSerialPort::Parity parity = QSerialPort::NoParity;

        QSerialPort::StopBits stopbit = QSerialPort::OneStop;

        QSerialPort::FlowControl flowcontrol = QSerialPort::NoFlowControl;

        //qDebug() <<"波特率："<<QString().sprintf("%d", baudrate);
        //qDebug() <<"数据位："<<QString().sprintf("%d", ui->boxDataBits->currentIndex());
        //qDebug() <<"奇偶校验位"<<QString().sprintf("%d", ui->boxParity->currentIndex());
        //qDebug() <<"停止位："<<QString().sprintf("%d", ui->boxStopBits->currentIndex());
        //qDebug() <<"流控制："<<QString().sprintf("%d", ui->boxFlowControl->currentIndex());
        //串口配置设置
        serialPort->setPortName(portnameStr);
        serialPort->setBaudRate(baudrate);
        serialPort->setDataBits(databit);
        serialPort->setParity(parity);
        serialPort->setStopBits(stopbit);
        serialPort->setFlowControl(flowcontrol);//这个我一般没用

        if(serialPort->open(QIODevice::ReadWrite))
        {
            qDebug()<<"串口已打开,读写模式";
            Serial_Port_Enable(false);//改变ui状态
            Serial_Port_Set_Opened(true);

            timer->stop();
        }
        else
        {
            qDebug()<<"串口打开异常"<<portnameStr<<serialPort->errorString();
            serialPort->clearError();
            Serial_Port_Enable(true);
            Serial_Port_Set_Opened(false);

            timer->start();
        }
    }
    else
    {
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}

void MySerialPort::Serial_Port_Close(void )
{
    serialPort->clear();
    serialPort->close();
    qDebug()<<"串口已关闭";
    Serial_Port_Enable(true);
    Serial_Port_Set_Opened(false);

    timer->start();
}

void MySerialPort::Serial_Port_Enable(bool enabled)
{
    //可以把btn和配置分在两个widget里，这样直接设置widget的enable就没这么麻烦了
    ui->serialPortNum->setEnabled(enabled);
    ui->serialPortBaud->setEnabled(enabled);
    ui->serialPortDataBit->setEnabled(enabled);
    ui->serialPortParityBit->setEnabled(enabled);
    ui->serialPortStopBit->setEnabled(enabled);
    ui->serialPortFlowCtrl->setEnabled(enabled);
}

void MySerialPort::Serial_Port_Send_Data(char *pBuf, int length)
{
    if(pBuf == nullptr || length <=0)
        return ;
    if(serialPort->isOpen()){
        serialPort->write(pBuf, length);
        //qDebug()<<"已发送："<<QString::fromUtf8(send_data);
    }else{
        qDebug()<<"发送失败,串口未打开";
        return;
    }
    //Qt新版本默认值是30 000
    if(!serialPort->waitForBytesWritten(30000)){
        qDebug()<<"命令发送异常"<<serialPort->errorString();
        serialPort->clearError();
    }
}

void MySerialPort::Serial_Port_Set_Opened(bool opened)
{
    serialPortOpened = opened;
}

bool MySerialPort::Serial_Port_Get_Opened()
{
    return serialPortOpened;
}

void MySerialPort::Serial_Port_Case_Open()
{
    char txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CASE_OPEN;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Case_Close()
{
    char txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CASE_CLOSE;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Fw_Ver(void )
{
    char txBuf[14] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x0a;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_FW_VER;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;
    txBuf[10] = VER_BLD;
    txBuf[11] = VER_APP;
    txBuf[12] = VER_HW;

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[13] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Chrg_Off()
{
    char txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CHAG_OFF;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Case_Batt()
{
    char txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CASE_BATT;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Fw_Size(int fwSize)
{
    char txBuf[15] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = 0x0b;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CDM_FW_SIZE;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;
    txBuf[10] = (char )fwSize;
    txBuf[11] = (char )(fwSize >> 8);
    txBuf[12] = (char )(fwSize >> 16);
    txBuf[13] = (char )(fwSize >> 24);

    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[14] = checkSum;

    Serial_Port_Send_Data(txBuf, sizeof(txBuf));
}

void MySerialPort::Serial_Port_Fw_Data(char *buf, int length)
{
    char txBuf[75] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5b;
    txBuf[2] = length + 7;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_FW_DATA;
    txBuf[8] = 0x00;
    txBuf[9] = 0x32;

    for(i=0;i<length;i++)
    {
        txBuf[i+10] = buf[i];
    }

    for(i=0;i<length-1;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[length+10] = checkSum;

    Serial_Port_Send_Data(txBuf, length+11);
}

void MySerialPort::Serial_Port_Timeout_ReadyRead()
{
    pTimerRecv->start(500);
}


void MySerialPort::Serial_Port_Recv(void )
{
    extern MyUpgrade *myUpgrade;

    if (serialPort->bytesAvailable())
    {
        //串口收到的数据可能不是连续的，需要的话应该把数据缓存下来再进行协议解析，类似tcp数据处理
        QByteArray recv_data;
        recv_data.resize(256);
        recv_data = serialPort->readAll();
        int length = recv_data.length();
        char *data = recv_data.data();
        //接收发送要一致，如果是处理字节数据，可以把QByteArray当数组一样取下标，或者用data()方法转为char*形式
        //ui->textRecv->append(QString::fromUtf8(recv_data));
        //qDebug() << QString().sprintf("recv count:%d", length);
        //qDebug()<<"已接收："<<QString::fromUtf8(recv_data);

        int i=0;
        while(i<(length-1))
        {
            if(data[i] == 0x05 && data[i+1] == 0x5a)
            {
                uchar calChecksum = 0;
                uint16_t cmdLength = (uint16_t)data[i+3] | data[i+2];
                uchar cmd = (uchar)data[i+7];
                uchar cmdCheckSum = (uchar)data[i+cmdLength+3];

                for(int j=0;j<cmdLength-1;j++)
                {
                    calChecksum += (uchar)data[i+4+j];
                }

                if(calChecksum == cmdCheckSum)
                {
                    myUpgrade->Upg_Handler((uint8_t *)&data[i+7], length-4);
                }

                i += cmdLength + 2;
            }
            else
            {
                i++;
            }
        }

        recv_data.clear();
    }
}
