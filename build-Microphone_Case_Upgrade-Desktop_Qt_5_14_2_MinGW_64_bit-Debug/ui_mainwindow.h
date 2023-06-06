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
    QComboBox *comboBox_2;
    QComboBox *comboBox_6;
    QLabel *label_2;
    QLabel *label_4;
    QComboBox *comboBox;
    QLabel *label_3;
    QComboBox *comboBox_5;
    QComboBox *comboBox_4;
    QLabel *label;
    QLabel *label_5;
    QComboBox *comboBox_7;
    MyBtn *widget;
    QTabWidget *tabWidget;
    QWidget *tabFwUpgrade;
    MyProgressBar *widget_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label_6;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_11;
    QLabel *label_10;
    QPushButton *pushButton_4;
    QLabel *label_7;
    QLabel *label_13;
    QPushButton *pushButton_5;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEdit_2;
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

        comboBox_2 = new QComboBox(gridLayoutWidget);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 1, 2, 1, 1);

        comboBox_6 = new QComboBox(gridLayoutWidget);
        comboBox_6->setObjectName(QString::fromUtf8("comboBox_6"));

        gridLayout->addWidget(comboBox_6, 4, 2, 1, 1);

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

        comboBox = new QComboBox(gridLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setFrame(true);

        gridLayout->addWidget(comboBox, 0, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font6);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        comboBox_5 = new QComboBox(gridLayoutWidget);
        comboBox_5->setObjectName(QString::fromUtf8("comboBox_5"));

        gridLayout->addWidget(comboBox_5, 3, 2, 1, 1);

        comboBox_4 = new QComboBox(gridLayoutWidget);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));

        gridLayout->addWidget(comboBox_4, 2, 2, 1, 1);

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

        comboBox_7 = new QComboBox(gridLayoutWidget);
        comboBox_7->setObjectName(QString::fromUtf8("comboBox_7"));

        gridLayout->addWidget(comboBox_7, 5, 2, 1, 1);

        widget = new MyBtn(groupBoxSerialPortSetting);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(35, 240, 71, 31));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(170, 50, 465, 311));
        tabFwUpgrade = new QWidget();
        tabFwUpgrade->setObjectName(QString::fromUtf8("tabFwUpgrade"));
        tabFwUpgrade->setFont(font6);
        widget_2 = new MyProgressBar(tabFwUpgrade);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(100, 90, 211, 161));
        pushButton = new QPushButton(tabFwUpgrade);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 30, 65, 30));
        lineEdit = new QLineEdit(tabFwUpgrade);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(90, 30, 281, 30));
        pushButton_2 = new QPushButton(tabFwUpgrade);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(390, 30, 65, 30));
        pushButton_3 = new QPushButton(tabFwUpgrade);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 190, 65, 30));
        label_6 = new QLabel(tabFwUpgrade);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 230, 65, 30));
        gridLayoutWidget_2 = new QWidget(tabFwUpgrade);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(310, 90, 141, 161));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(gridLayoutWidget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font7.setPointSize(10);
        label_11->setFont(font7);
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(label_11, 2, 0, 1, 1);

        label_10 = new QLabel(gridLayoutWidget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font8.setPointSize(10);
        font8.setBold(false);
        font8.setWeight(50);
        label_10->setFont(font8);
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_10, 4, 1, 1, 1);

        pushButton_4 = new QPushButton(gridLayoutWidget_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QFont font9;
        font9.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font9.setPointSize(10);
        font9.setBold(true);
        font9.setWeight(75);
        pushButton_4->setFont(font9);

        gridLayout_2->addWidget(pushButton_4, 3, 0, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font9);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_7, 0, 0, 1, 2);

        label_13 = new QLabel(gridLayoutWidget_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(label_13, 2, 1, 1, 1);

        pushButton_5 = new QPushButton(gridLayoutWidget_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        gridLayout_2->addWidget(pushButton_5, 3, 1, 1, 1);

        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font7);
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font9);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(label_9, 4, 0, 1, 1);

        lineEdit_2 = new QLineEdit(gridLayoutWidget_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout_2->addWidget(lineEdit_2, 1, 1, 1, 1);

        tabWidget->addTab(tabFwUpgrade, QString());
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(440, 360, 191, 21));
        QFont font10;
        font10.setFamily(QString::fromUtf8("Microsoft JhengHei UI"));
        font10.setPointSize(8);
        label_12->setFont(font10);
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
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\346\265\201\346\216\247\345\210\266", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\233\272\344\273\266", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\226\260\345\233\272\344\273\266", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\347\211\210\346\234\254", nullptr));
        label_6->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\254\241\346\225\260:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "PASS", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\265\213\350\257\225", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "IAP\345\215\207\347\272\247\346\265\213\350\257\225:", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\346\265\213\350\257\225", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\346\200\273\346\265\213\350\257\225\346\254\241\346\225\260:", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\346\265\213\350\257\225\347\273\223\346\236\234:", nullptr));
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
