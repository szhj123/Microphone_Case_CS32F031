#ifndef MYBTN_H
#define MYBTN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

class MyBtn : public QWidget
{
    Q_OBJECT
public:
    explicit MyBtn(QWidget *parent = nullptr);
    ~MyBtn();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void Draw_Bg(QPainter *painter);
    void Draw_Slider(QPainter *painter);
private:
    QColor bgOnColor;
    QColor bgOffColor;
    QColor sliderOnColor;
    QColor sliderOffColor;
    QColor textOnColor;
    QColor textOffColor;

    int marginVal;
    int sliderStartX;
    int sliderEndX;
    int stepVal;

    bool checked;

    QTimer *timer;
private slots:
    void Update_Val(void );
public:
    bool Get_Checked(void );
signals:
    void checkedChanged(bool checked);
};

#endif // MYBTN_H

