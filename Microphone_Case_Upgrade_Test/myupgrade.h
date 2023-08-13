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

#define CMD_CASE_OPEN       0x02
#define CMD_CASE_CLOSE      0x03
#define CMD_CHAG_OFF        0x04
#define CMD_CASE_BATT       0x06
#define CDM_FW_SIZE         0x13
#define CMD_FW_DATA         0x14
#define CMD_FW_CRC          0x15
#define CMD_FW_VER          0x16

#define VER_BLD             0
#define VER_APP             2
#define VER_HW              0

typedef struct _fw_info_t
{
    int fwSize;
    int fwOffset;
    uint8_t *fwBuf;
    int fwTxLength;
    int fwTxTimeoutCnt;
    int fwTxErrCnt;

    uint8_t bldVer;
    uint8_t appVer;
    uint8_t hwVer;
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
    uint16_t Upg_Cal_Checksum(uint8_t *buf, uint32_t length );
    void Upg_Handler(uint8_t *buf, uint16_t length );
private:
    Ui::MainWindow  *ui;
    MySerialPort *serialPort;
    fw_info_t fwInfo;
    uint8_t progressVal;
    bool fwVerRecvFlag;
signals:
private slots:
    void on_btnAddFw_Clicked(void );
    void on_btnUpgEn_Clicked(void );
    void on_btnGetFw_Clicked(void );
};

#endif // MYUPGRADE_H
