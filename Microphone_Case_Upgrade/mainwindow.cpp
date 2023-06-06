#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlag(Qt::FramelessWindowHint);

    setFixedSize(this->width(),this->height());

    ui->setupUi(this);

    ui->tabWidget->setTabText(0, tr("固件升级"));
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
