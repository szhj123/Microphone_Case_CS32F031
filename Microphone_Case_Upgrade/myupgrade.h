#ifndef MYUPGRADE_H
#define MYUPGRADE_H

#include "ui_mainwindow.h"
#include "myserialport.h"
#include "myprogressbar.h"

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#define FW_MAX_DATA_LENGTH               64

typedef enum
{
    UPG_STATE_ERASE_FLASH = 0,
    UPG_STATE_WAIT_ACK_FOR_ERASE,
    UPG_STATE_TX_FW_DATA,
    UPG_STATE_WAIT_ACK_FOR_TX,
    UPG_STATE_TX_FW_CHECKSUM,
    UPG_STATE_WATI_ACK_FOR_CHECKSUM,
    UPG_STATE_TX_GET_VERSION,
    UPG_STATE_WAIT_FW_VERSION,
    UPG_STATE_IDLE
}upg_state_t;

typedef struct _fw_info_t
{
    int fwSize;
    int fwOffset;
    char *fwBuf;
    int fwTxLength;
    int fwTxTimeoutCnt;
    int fwTxErrCnt;

    uint8_t buildVer;
    uint8_t minorVer;
    uint8_t majorVer;
    QByteArray fwArray;
}fw_info_t;
Q_DECLARE_METATYPE(fw_info_t)

class MyUpgrade : public QWidget
{
    Q_OBJECT
public:
    explicit MyUpgrade(QWidget *parent = nullptr);
    void Upg_Init(Ui::MainWindow *ui, MySerialPort *serialPort);
    void Upg_Set_Ver(char fwBuildVer, char fwMinorVer, char fwMajorVer);
    void Upg_Set_Ack(uchar ack );
    void Upg_Clr_Ack(void );
    uchar Upg_Get_Ack(void );
    uint16_t Upg_Cal_Checksum(uint8_t *buf, uint32_t length );
private:
    Ui::MainWindow  *ui;
    MySerialPort *serialPort;
    QTimer *timer;
    char ack;
    bool fwVerRecvFlag;
signals:
private slots:
    void on_btnAddFw_Clicked(void );
    void on_btnUpgEn_Clicked(void );
    void on_btnGetFw_Clicked(void );
    void upg_handler(void );
};

#endif // MYUPGRADE_H
