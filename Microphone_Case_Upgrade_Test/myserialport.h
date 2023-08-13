#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include "ui_mainwindow.h"
#include <QWidget>
#include <QDebug>
#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

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

    void Serial_Port_Case_Open(void );
    void Serial_Port_Case_Close(void );
    void Serial_Port_Chrg_Off(void );
    void Serial_Port_Case_Batt(void );
    void Serial_Port_Fw_Size(int fwSize);
    void Serial_Port_Fw_Data(uint8_t *buf, int length );
    void Serial_Port_Fw_Crc(uint16_t fwCrc );
    void Serial_Port_Fw_Ver(uint8_t bldVer, uint8_t appVer, uint8_t hwVer);
private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTimer *timer;
    QTimer *pTimerRecv;
    bool serialPortOpened;
private slots:
    void Serial_Port_Timeout_ReadyRead(void );
    void Serial_Port_Recv(void );
    void Serial_Port_Monitor(void );
signals:

};

#endif // MYSERIALPORT_H
