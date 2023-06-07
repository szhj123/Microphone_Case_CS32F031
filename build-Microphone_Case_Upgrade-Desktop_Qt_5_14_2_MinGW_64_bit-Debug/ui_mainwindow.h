/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <mybtn.h>
#include "myprogressbar.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *logoWidget;
    QLabel *labelLogoText;
    QLabel *labelLogoPic;
    QPushButton *btnMinimize;
    QLabel *labelLogoName;
    QPushButton *btnClose;
    QLabel *labelSerialPortName;
    QGroupBox *groupBoxSerialPortSetting;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *labelSerialPortNum;
    QComboBox *serialPortBaud;
    QComboBox *serialPortStopBit;
    QLabel *label_2;
    QLabel *label_4;
    QComboBox *serialPortNum;
    QLabel *label_3;
    QComboBox *serialPortDataBit;
    QComboBox *serialPortParityBit;
    QLabel *label;
    QLabel *label_5;
    QComboBox *serialPortFlowCtrl;
    MyBtn *btnSerialPort;
    QTabWidget *tabWidget;
    QWidget *tabFwUpgrade;
    MyProgressBar *upgProgressBar;
    QPushButton *btnAddFw;
    QLineEdit *lineEditFwPath;
    QPushButton *btnUpgEN;
    QPushButton *btnGetFw;
    QLabel *labelFwVer;
    QLabel *label_13;
    QLabel *label_12;
    QLabel *label_14;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 400);
        MainWindow->setMinimumSize(QSize(640, 400));
        MainWindow->setMaximumSize(QSize(640, 400));
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        logoWidget = new QWidget(centralwidget);
        logoWidget->setObjectName(QString::fromUtf8("logoWidget"));
        logoWidget->setGeometry(QRect(0, 0, 640, 35));
        QFont font1;
        font1.setPointSize(9);
        logoWidget->setFont(font1);
        labelLogoText = new QLabel(logoWidget);
        labelLogoText->setObjectName(QString::fromUtf8("labelLogoText"));
        labelLogoText->setGeometry(QRect(80, 2, 281, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MV Boli"));
        font2.setPointSize(13);
        labelLogoText->setFont(font2);
        labelLogoText->setStyleSheet(QString::fromUtf8("color: rgb(50,254,255)"));
        labelLogoPic = new QLabel(logoWidget);
        labelLogoPic->setObjectName(QString::fromUtf8("labelLogoPic"));
        labelLogoPic->setGeometry(QRect(10, 2, 61, 31));
        btnMinimize = new QPushButton(logoWidget);
        btnMinimize->setObjectName(QString::fromUtf8("btnMinimize"));
        btnMinimize->setGeometry(QRect(570, 2, 30, 30));
        labelLogoName = new QLabel(logoWidget);
        labelLogoName->setObjectName(QString::fromUtf8("labelLogoName"));
        labelLogoName->setGeometry(QRect(370, 10, 161, 20));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setUnderline(false);
        font3.setWeight(75);
        labelLogoName->setFont(font3);
        labelLogoName->setStyleSheet(QString::fromUtf8("color: rgb(255,255,255)"));
        labelLogoName->setTextFormat(Qt::AutoText);
        labelLogoName->setAlignment(Qt::AlignCenter);
        btnClose = new QPushButton(logoWidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(605, 2, 30, 30));
        labelSerialPortName = new QLabel(centralwidget);
        labelSerialPortName->setObjectName(QString::fromUtf8("labelSerialPortName"));
        labelSerialPortName->setGeometry(QRect(35, 50, 111, 20));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font4.setPointSize(12);
        labelSerialPortName->setFont(font4);
        labelSerialPortName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        groupBoxSerialPortSetting = new QGroupBox(centralwidget);
        groupBoxSerialPortSetting->setObjectName(QString::fromUtf8("groupBoxSerialPortSetting"));
        groupBoxSerialPortSetting->setGeometry(QRect(10, 80, 151, 281));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font5.setBold(true);
        font5.setUnderline(false);
        font5.setWeight(75);
        groupBoxSerialPortSetting->setFont(font5);
        groupBoxSerialPortSetting->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        gridLayoutWidget = new QWidget(groupBoxSerialPortSetting);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 19, 141, 211));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelSerialPortNum = new QLabel(gridLayoutWidget);
        labelSerialPortNum->setObjectName(QString::fromUtf8("labelSerialPortNum"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font6.setBold(true);
        font6.setWeight(75);
        labelSerialPortNum->setFont(font6);
        labelSerialPortNum->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelSerialPortNum, 0, 1, 1, 1);

        serialPortBaud = new QComboBox(gridLayoutWidget);
        serialPortBaud->addItem(QString());
        serialPortBaud->setObjectName(QString::fromUtf8("serialPortBaud"));
        serialPortBaud->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortBaud, 1, 2, 1, 1);

        serialPortStopBit = new QComboBox(gridLayoutWidget);
        serialPortStopBit->addItem(QString());
        serialPortStopBit->setObjectName(QString::fromUtf8("serialPortStopBit"));
        serialPortStopBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortStopBit, 4, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font6);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font6);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 4, 1, 1, 1);

        serialPortNum = new QComboBox(gridLayoutWidget);
        serialPortNum->setObjectName(QString::fromUtf8("serialPortNum"));
        serialPortNum->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));
        serialPortNum->setFrame(true);

        gridLayout->addWidget(serialPortNum, 0, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font6);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        serialPortDataBit = new QComboBox(gridLayoutWidget);
        serialPortDataBit->addItem(QString());
        serialPortDataBit->setObjectName(QString::fromUtf8("serialPortDataBit"));
        serialPortDataBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortDataBit, 3, 2, 1, 1);

        serialPortParityBit = new QComboBox(gridLayoutWidget);
        serialPortParityBit->addItem(QString());
        serialPortParityBit->setObjectName(QString::fromUtf8("serialPortParityBit"));
        serialPortParityBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortParityBit, 2, 2, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font6);
        label->setFrameShape(QFrame::NoFrame);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font6);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 5, 1, 1, 1);

        serialPortFlowCtrl = new QComboBox(gridLayoutWidget);
        serialPortFlowCtrl->addItem(QString());
        serialPortFlowCtrl->setObjectName(QString::fromUtf8("serialPortFlowCtrl"));
        serialPortFlowCtrl->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortFlowCtrl, 5, 2, 1, 1);

        btnSerialPort = new MyBtn(groupBoxSerialPortSetting);
        btnSerialPort->setObjectName(QString::fromUtf8("btnSerialPort"));
        btnSerialPort->setGeometry(QRect(35, 240, 71, 31));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(170, 50, 465, 311));
        tabFwUpgrade = new QWidget();
        tabFwUpgrade->setObjectName(QString::fromUtf8("tabFwUpgrade"));
        tabFwUpgrade->setFont(font6);
        upgProgressBar = new MyProgressBar(tabFwUpgrade);
        upgProgressBar->setObjectName(QString::fromUtf8("upgProgressBar"));
        upgProgressBar->setGeometry(QRect(100, 90, 211, 161));
        btnAddFw = new QPushButton(tabFwUpgrade);
        btnAddFw->setObjectName(QString::fromUtf8("btnAddFw"));
        btnAddFw->setGeometry(QRect(10, 30, 65, 30));
        lineEditFwPath = new QLineEdit(tabFwUpgrade);
        lineEditFwPath->setObjectName(QString::fromUtf8("lineEditFwPath"));
        lineEditFwPath->setGeometry(QRect(90, 30, 281, 30));
        btnUpgEN = new QPushButton(tabFwUpgrade);
        btnUpgEN->setObjectName(QString::fromUtf8("btnUpgEN"));
        btnUpgEN->setGeometry(QRect(390, 30, 65, 30));
        btnGetFw = new QPushButton(tabFwUpgrade);
        btnGetFw->setObjectName(QString::fromUtf8("btnGetFw"));
        btnGetFw->setGeometry(QRect(310, 220, 65, 30));
        labelFwVer = new QLabel(tabFwUpgrade);
        labelFwVer->setObjectName(QString::fromUtf8("labelFwVer"));
        labelFwVer->setGeometry(QRect(380, 220, 71, 30));
        labelFwVer->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labelFwVer->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(tabFwUpgrade);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(400, 150, 51, 21));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font7.setPointSize(10);
        label_13->setFont(font7);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_13->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tabFwUpgrade, QString());
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(440, 360, 191, 21));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font8.setPointSize(8);
        label_12->setFont(font8);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(50,254,255)"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 360, 181, 16));
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255,255,255)"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelLogoText->setText(QCoreApplication::translate("MainWindow", "Microphone Charging Case", nullptr));
        labelLogoPic->setText(QString());
        btnMinimize->setText(QString());
        labelLogoName->setText(QCoreApplication::translate("MainWindow", "IAP\345\215\207\347\272\247\350\260\203\350\257\225\345\267\245\345\205\267", nullptr));
        btnClose->setText(QString());
        labelSerialPortName->setText(QCoreApplication::translate("MainWindow", "\351\200\232\344\277\241\350\256\276\345\244\207", nullptr));
        groupBoxSerialPortSetting->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", nullptr));
        labelSerialPortNum->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", nullptr));
        serialPortBaud->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));

        serialPortStopBit->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        serialPortDataBit->setItemText(0, QCoreApplication::translate("MainWindow", "8", nullptr));

        serialPortParityBit->setItemText(0, QCoreApplication::translate("MainWindow", "NONE", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\346\265\201\346\216\247\345\210\266", nullptr));
        serialPortFlowCtrl->setItemText(0, QCoreApplication::translate("MainWindow", "NONE", nullptr));

        btnAddFw->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\233\272\344\273\266", nullptr));
        btnUpgEN->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\226\260\345\233\272\344\273\266", nullptr));
        btnGetFw->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        labelFwVer->setText(QCoreApplication::translate("MainWindow", "0.0.0", nullptr));
        label_13->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabFwUpgrade), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\346\267\261\345\234\263\345\274\247\345\205\213\345\210\233\346\226\260\347\247\221\346\212\200\346\234\211\351\231\220\345\205\254\345\217\270", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Fw Verion:1.0.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
