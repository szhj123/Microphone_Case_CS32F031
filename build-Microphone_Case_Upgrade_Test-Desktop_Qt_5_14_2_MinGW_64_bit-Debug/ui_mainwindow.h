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
    QTabWidget *tabWidget;
    QWidget *tabFwUpgrade;
    MyProgressBar *upgProgressBar;
    QPushButton *btnAddFw;
    QLineEdit *lineEditFwPath;
    QPushButton *btnUpgEN;
    QLabel *labelFwVerNewVal;
    QLabel *label_14;
    QLabel *labelFwVerOld;
    QLabel *labelFwVerNew;
    QLabel *labelFwVerOldVal;
    QGroupBox *groupBoxSerialPortSetting;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout;
    QLabel *labelSerialPortNum;
    QComboBox *serialPortBaud;
    QComboBox *serialPortStopBit;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *serialPortNum;
    QLabel *label_8;
    QComboBox *serialPortDataBit;
    QComboBox *serialPortParityBit;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *serialPortFlowCtrl;
    MyBtn *btnSerialPort;
    QLabel *labelSerialPortName;
    QLabel *label_15;
    QWidget *logoWidget;
    QLabel *labelLogoText;
    QLabel *labelLogoPic;
    QPushButton *btnMinimize;
    QLabel *labelLogoName;
    QPushButton *btnClose;
    QLabel *label_12;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(764, 478);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(170, 50, 465, 311));
        tabFwUpgrade = new QWidget();
        tabFwUpgrade->setObjectName(QString::fromUtf8("tabFwUpgrade"));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font.setBold(true);
        font.setWeight(75);
        tabFwUpgrade->setFont(font);
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
        labelFwVerNewVal = new QLabel(tabFwUpgrade);
        labelFwVerNewVal->setObjectName(QString::fromUtf8("labelFwVerNewVal"));
        labelFwVerNewVal->setGeometry(QRect(389, 220, 71, 25));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        labelFwVerNewVal->setFont(font1);
        labelFwVerNewVal->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labelFwVerNewVal->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(tabFwUpgrade);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(400, 150, 51, 21));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font2.setPointSize(10);
        label_14->setFont(font2);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_14->setAlignment(Qt::AlignCenter);
        labelFwVerOld = new QLabel(tabFwUpgrade);
        labelFwVerOld->setObjectName(QString::fromUtf8("labelFwVerOld"));
        labelFwVerOld->setGeometry(QRect(310, 180, 80, 25));
        labelFwVerOld->setFont(font);
        labelFwVerOld->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labelFwVerNew = new QLabel(tabFwUpgrade);
        labelFwVerNew->setObjectName(QString::fromUtf8("labelFwVerNew"));
        labelFwVerNew->setGeometry(QRect(310, 220, 80, 25));
        labelFwVerNew->setFont(font);
        labelFwVerNew->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labelFwVerOldVal = new QLabel(tabFwUpgrade);
        labelFwVerOldVal->setObjectName(QString::fromUtf8("labelFwVerOldVal"));
        labelFwVerOldVal->setGeometry(QRect(390, 180, 71, 25));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        labelFwVerOldVal->setFont(font3);
        labelFwVerOldVal->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labelFwVerOldVal->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tabFwUpgrade, QString());
        groupBoxSerialPortSetting = new QGroupBox(centralwidget);
        groupBoxSerialPortSetting->setObjectName(QString::fromUtf8("groupBoxSerialPortSetting"));
        groupBoxSerialPortSetting->setGeometry(QRect(10, 80, 151, 281));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font4.setBold(true);
        font4.setUnderline(false);
        font4.setWeight(75);
        groupBoxSerialPortSetting->setFont(font4);
        groupBoxSerialPortSetting->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        gridLayoutWidget_2 = new QWidget(groupBoxSerialPortSetting);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(0, 19, 141, 211));
        gridLayout = new QGridLayout(gridLayoutWidget_2);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelSerialPortNum = new QLabel(gridLayoutWidget_2);
        labelSerialPortNum->setObjectName(QString::fromUtf8("labelSerialPortNum"));
        labelSerialPortNum->setFont(font);
        labelSerialPortNum->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelSerialPortNum, 0, 1, 1, 1);

        serialPortBaud = new QComboBox(gridLayoutWidget_2);
        serialPortBaud->addItem(QString());
        serialPortBaud->setObjectName(QString::fromUtf8("serialPortBaud"));
        serialPortBaud->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortBaud, 1, 2, 1, 1);

        serialPortStopBit = new QComboBox(gridLayoutWidget_2);
        serialPortStopBit->addItem(QString());
        serialPortStopBit->setObjectName(QString::fromUtf8("serialPortStopBit"));
        serialPortStopBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortStopBit, 4, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 2, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 4, 1, 1, 1);

        serialPortNum = new QComboBox(gridLayoutWidget_2);
        serialPortNum->setObjectName(QString::fromUtf8("serialPortNum"));
        serialPortNum->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));
        serialPortNum->setFrame(true);

        gridLayout->addWidget(serialPortNum, 0, 2, 1, 1);

        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_8, 3, 1, 1, 1);

        serialPortDataBit = new QComboBox(gridLayoutWidget_2);
        serialPortDataBit->addItem(QString());
        serialPortDataBit->setObjectName(QString::fromUtf8("serialPortDataBit"));
        serialPortDataBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortDataBit, 3, 2, 1, 1);

        serialPortParityBit = new QComboBox(gridLayoutWidget_2);
        serialPortParityBit->addItem(QString());
        serialPortParityBit->setObjectName(QString::fromUtf8("serialPortParityBit"));
        serialPortParityBit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortParityBit, 2, 2, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::NoFrame);
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 1, 1, 1, 1);

        label_10 = new QLabel(gridLayoutWidget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 5, 1, 1, 1);

        serialPortFlowCtrl = new QComboBox(gridLayoutWidget_2);
        serialPortFlowCtrl->addItem(QString());
        serialPortFlowCtrl->setObjectName(QString::fromUtf8("serialPortFlowCtrl"));
        serialPortFlowCtrl->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 8pt \"Microsoft YaHei UI\";"));

        gridLayout->addWidget(serialPortFlowCtrl, 5, 2, 1, 1);

        btnSerialPort = new MyBtn(groupBoxSerialPortSetting);
        btnSerialPort->setObjectName(QString::fromUtf8("btnSerialPort"));
        btnSerialPort->setGeometry(QRect(35, 240, 71, 31));
        labelSerialPortName = new QLabel(centralwidget);
        labelSerialPortName->setObjectName(QString::fromUtf8("labelSerialPortName"));
        labelSerialPortName->setGeometry(QRect(35, 50, 111, 20));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font5.setPointSize(12);
        labelSerialPortName->setFont(font5);
        labelSerialPortName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 360, 181, 16));
        label_15->setStyleSheet(QString::fromUtf8("color: rgb(255,255,255)"));
        logoWidget = new QWidget(centralwidget);
        logoWidget->setObjectName(QString::fromUtf8("logoWidget"));
        logoWidget->setGeometry(QRect(0, 0, 640, 35));
        QFont font6;
        font6.setPointSize(9);
        logoWidget->setFont(font6);
        labelLogoText = new QLabel(logoWidget);
        labelLogoText->setObjectName(QString::fromUtf8("labelLogoText"));
        labelLogoText->setGeometry(QRect(80, 2, 281, 31));
        QFont font7;
        font7.setFamily(QString::fromUtf8("MV Boli"));
        font7.setPointSize(13);
        labelLogoText->setFont(font7);
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
        QFont font8;
        font8.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font8.setPointSize(10);
        font8.setBold(true);
        font8.setUnderline(false);
        font8.setWeight(75);
        labelLogoName->setFont(font8);
        labelLogoName->setStyleSheet(QString::fromUtf8("color: rgb(255,255,255)"));
        labelLogoName->setTextFormat(Qt::AutoText);
        labelLogoName->setAlignment(Qt::AlignCenter);
        btnClose = new QPushButton(logoWidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(605, 2, 30, 30));
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(440, 360, 191, 21));
        QFont font9;
        font9.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font9.setPointSize(8);
        label_12->setFont(font9);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(50,254,255)"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 764, 21));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnAddFw->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\233\272\344\273\266", nullptr));
        btnUpgEN->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\226\260\345\233\272\344\273\266", nullptr));
        labelFwVerNewVal->setText(QCoreApplication::translate("MainWindow", "0.0.0", nullptr));
        label_14->setText(QString());
        labelFwVerOld->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\345\275\223\345\211\215\347\211\210\346\234\254", nullptr));
        labelFwVerNew->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\345\215\207\347\272\247\347\211\210\346\234\254", nullptr));
        labelFwVerOldVal->setText(QCoreApplication::translate("MainWindow", "0.0.0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabFwUpgrade), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        groupBoxSerialPortSetting->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", nullptr));
        labelSerialPortNum->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", nullptr));
        serialPortBaud->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));

        serialPortStopBit->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));

        label_6->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        serialPortDataBit->setItemText(0, QCoreApplication::translate("MainWindow", "8", nullptr));

        serialPortParityBit->setItemText(0, QCoreApplication::translate("MainWindow", "NONE", nullptr));

        label_9->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\346\265\201\346\216\247\345\210\266", nullptr));
        serialPortFlowCtrl->setItemText(0, QCoreApplication::translate("MainWindow", "NONE", nullptr));

        labelSerialPortName->setText(QCoreApplication::translate("MainWindow", "\351\200\232\344\277\241\350\256\276\345\244\207", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Fw Verion:1.0.0", nullptr));
        labelLogoText->setText(QCoreApplication::translate("MainWindow", "Microphone Charging Case", nullptr));
        labelLogoPic->setText(QString());
        btnMinimize->setText(QString());
        labelLogoName->setText(QCoreApplication::translate("MainWindow", "IAP\345\215\207\347\272\247\350\260\203\350\257\225\345\267\245\345\205\267", nullptr));
        btnClose->setText(QString());
        label_12->setText(QCoreApplication::translate("MainWindow", "\346\267\261\345\234\263\345\274\247\345\205\213\345\210\233\346\226\260\347\247\221\346\212\200\346\234\211\351\231\220\345\205\254\345\217\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
