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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

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
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 400);
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
        labelLogoName->setFont(font3);
        labelLogoName->setStyleSheet(QString::fromUtf8("color: rgb(255,255,255)"));
        labelLogoName->setTextFormat(Qt::AutoText);
        btnClose = new QPushButton(logoWidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(605, 2, 30, 30));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelLogoText->setText(QCoreApplication::translate("MainWindow", "Microphone Charging Case", nullptr));
        labelLogoPic->setText(QString());
        btnMinimize->setText(QString());
        labelLogoName->setText(QCoreApplication::translate("MainWindow", "Serial Port Upgrade Tool", nullptr));
        btnClose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
