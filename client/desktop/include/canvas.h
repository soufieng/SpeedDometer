#ifndef CANVAS_H
#define CANVAS_H

#include <mutex>
#include <QWidget>
#include <QPainter>
#include <QSoundEffect>

class Canvas : public QWidget
{
    QPen drawPen;
    QBrush brush;
    QPainter painter;
    QFont iconFont;
    QSoundEffect soundEffect;

    int speed{0};
    bool connection = false;
    int BatteryLevel{0};
    int temprature{0};
    bool leftSide{false}, rightSide{false};
    void paintEvent(QPaintEvent *event) override;

public:
    Canvas();

    void setBatteryLevel(int level) { BatteryLevel = level; }
    void setSpeed(int spd) { speed = spd; }
    void setTemprature(int temp) { temprature = temp; }
    void setConnection(bool conect) { connection = conect; }
    void setLights(bool _leftSide, bool _rightSide)
    {

        leftSide = _leftSide;
        rightSide = _rightSide;
    }

private:
    void drawBatteryLevel(void);
    void drawTmpLevel(void);
    void turnSignal(void);
    void drawSpeedometer(void);
};

#endif
