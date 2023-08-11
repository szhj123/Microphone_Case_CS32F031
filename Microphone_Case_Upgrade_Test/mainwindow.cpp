#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myserialport.h"
#include "myupgrade.h"
#include "mybtn.h"

MySerialPort *mySerialPort;
MyUpgrade *myUpgrade;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlag(Qt::FramelessWindowHint);

    setGeometry(0, 0, 640, 400);

    setFixedSize(this->width(),this->height());

    ui->setupUi(this);

    ui->tabWidget->setTabText(0, tr("固件升级"));

    mySerialPort = new MySerialPort(this);

    mySerialPort->Serial_Port_Init(ui);

    myUpgrade = new MyUpgrade(this);

    myUpgrade->Upg_Init(ui, mySerialPort);

    connect(ui->btnSerialPort, SIGNAL(checkedChanged(bool)), this, SLOT(slot_btn_SerialPort_checked(bool)));

    connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(slot_btn_minimize_clicked()));

    connect(ui->btnClose, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int height = ui->logoWidget->height();

    if(event->y() < height)
    {
        lastPoint = event->globalPos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int height = ui->logoWidget->height();

    if(event->y() < height )
    {
        int dx = event->globalX() - lastPoint.x();
        int dy = event->globalY() - lastPoint.y();

        lastPoint = event->globalPos();

        this->move(this->x() + dx, this->y() + dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int height = ui->logoWidget->height();

    if(event->y() < height )
    {
        int dx = event->globalX() - lastPoint.x();
        int dy = event->globalY() - lastPoint.y();

        this->move(this->x() + dx, this->y() + dy);
    }
}

void MainWindow::slot_btn_SerialPort_checked(bool checked)
{
    if(checked)
    {
        mySerialPort->Serial_Port_Open();
    }
    else
    {
        mySerialPort->Serial_Port_Close();
    }
}

void MainWindow::slot_btn_minimize_clicked(void )
{
   this->showMinimized();
}

