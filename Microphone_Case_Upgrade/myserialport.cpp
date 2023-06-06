#include "mainwindow.h"
#include "myserialport.h"

MySerialPort::MySerialPort(QWidget *parent) : QWidget(parent)
{
    serialPortOpened = false;
}

void MySerialPort::Serial_Port_Init(Ui::MainWindow *ui)
{
    //创建串口串口对象
    serialPort = new QSerialPort(this);
    //数据接收处理
    connect(serialPort,&QSerialPort::readyRead,this,&MySerialPort::Serial_Port_Recv);

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

void MySerialPort::Serial_Port_Refresh(Ui::MainWindow *ui)
{
    if(serialPortOpened)
    {
        ui->serialPortNum->clear();

        Serial_Port_Init(ui);
    }
}

void MySerialPort::Serial_Port_Open(Ui::MainWindow *ui)
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
            Serial_Port_Enable(ui, false);//改变ui状态
            Serial_Port_Set_Opened(true);
        }
        else
        {
            qDebug()<<"串口打开异常"<<portnameStr<<serialPort->errorString();
            serialPort->clearError();
            Serial_Port_Enable(ui, true);
            Serial_Port_Set_Opened(false);
        }
    }
    else
    {
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}

void MySerialPort::Serial_Port_Close(Ui::MainWindow *ui)
{
    serialPort->clear();
    serialPort->close();
    qDebug()<<"串口已关闭";
    Serial_Port_Enable(ui, true);
    Serial_Port_Set_Opened(false);
}

void MySerialPort::Serial_Port_Enable(Ui::MainWindow *ui, bool enabled)
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

void MySerialPort::Serial_Port_Recv(void )
{

}
