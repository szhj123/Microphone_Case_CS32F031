#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include "ui_mainwindow.h"
#include <QWidget>
#include <QDebug>
#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#define CMD_FW_ERASE        0x75
#define CMD_FW_DATA         0x76
#define CMD_FW_CHECKSUM     0x77
#define CDM_FW_ACK          0x78
#define CMD_FW_VERSION      0x79
#define CMD_FW_RESET        0x7a

class MySerialPort : public QWidget
{
    Q_OBJECT
public:
    explicit MySerialPort(QWidget *parent = nullptr);

    void Serial_Port_Init(Ui::MainWindow *ui);
    void Serial_Port_Refresh(void );
    void Serial_Port_Open(void );
    void Serial_Port_Close(void );
    void Serial_Port_Enable(bool enabled);
    void Serial_Port_Send_Data(char *pBuf, int length );
    void Serial_Port_Set_Opened(bool opened);
    bool Serial_Port_Get_Opened(void);

    void Serial_Port_Fw_Erase(int fwLength );
    void Serial_Port_Tx_Data(int offset, char *pBuf, int length);
    void Serial_Port_Tx_Checksum(uint16_t fwChecksum);
    void Serial_Port_Tx_Reset(void );
    void Serial_Port_Get_Version(void );
private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTimer *timer;
    bool serialPortOpened;
private slots:
    void Serial_Port_Recv(void );
    void Serial_Port_Monitor(void );
signals:

};

#endif // MYSERIALPORT_H
