#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class MyProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyProgressBar(QWidget *parent = nullptr);
    ~MyProgressBar();
    void Update_Val(uint8_t val);
protected:
    void paintEvent(QPaintEvent *event);
    void Draw_ProcessBar(QPainter *painter);
    void Draw_Text(QPainter *painter);
private:
    int minVal;
    int maxVal;
    int currVal;

    int processWidth;
    int textWidth;
    int marginVal;

    QColor bgColor;
    QColor processPieColor;
    QColor processBgColor;
    QColor textColor;

private slots:

signals:

};

#endif // MYPROGRESSBAR_H
