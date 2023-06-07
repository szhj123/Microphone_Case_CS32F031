#include "myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget *parent) : QWidget(parent)
{
    bgColor = QColor(85, 125, 200);
    processBgColor = QColor(255, 255, 255);
    processPieColor = QColor(51, 254, 255);
    textColor = QColor(255, 255, 255);

    marginVal = 15;

    minVal = 0;
    maxVal = 100;
    currVal = 0;

    textWidth = 16;
    processWidth = 10;

    setMinimumSize(50, 50);
}

MyProgressBar::~MyProgressBar()
{

}

void MyProgressBar::Update_Val(uint8_t val)
{
    if(val > 100)
        val = 100;

    currVal = val;

    update();
}

void MyProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    Draw_ProcessBar(&painter);

    Draw_Text(&painter);

}

void MyProgressBar::Draw_ProcessBar(QPainter *painter)
{
    painter->save();

    int width = this->width();
    int height = this->height();
    int diameter = qMin(width, height);

    QRect rectCircle(0, 0, diameter, diameter);

    painter->setBrush(bgColor);
    painter->drawEllipse(rectCircle);

    QRect rectProcess(marginVal, marginVal, diameter-marginVal*2, diameter-marginVal*2);
    painter->setBrush(processBgColor);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(rectProcess);

    painter->setBrush(processPieColor);
    painter->drawPie(rectProcess, (90)*16, (360*16*currVal)/100);

    QRect rectInterCircle(marginVal+processWidth, marginVal+processWidth, diameter-(marginVal+processWidth)*2, diameter-(marginVal+processWidth)*2);
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawEllipse(rectInterCircle);

    painter->restore();
}

void MyProgressBar::Draw_Text(QPainter *painter)
{
    painter->save();

    int width = this->width();
    int height = this->height();
    int diameter = qMin(width, height);

    QRect textRect(diameter/2 -25, diameter/2-textWidth/2, 50, textWidth);

    QPen pen;
    pen.setColor(QColor(255, 255, 255));
    pen.setWidth(5);
    QFont font;
    font.setFamily(tr("微软雅黑"));
    font.setPixelSize(textWidth);
    font.setBold(true);
    painter->setFont(font);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(pen);

    QString strValue = QString("%1").arg(currVal);
    strValue += "%";
    painter->drawText(textRect, Qt::AlignCenter, strValue);

}
