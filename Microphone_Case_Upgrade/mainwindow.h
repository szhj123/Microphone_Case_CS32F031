#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QMouseEvent>
#include <QListView>
#include <QLabel>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event );
    void mouseReleaseEvent(QMouseEvent *event );

private:
    Ui::MainWindow *ui;
    QPoint lastPoint;
private slots:
    void on_btnSerialPort_checked(bool checked);
    void on_serialPortNum_activated(int index);
};
#endif // MAINWINDOW_H
