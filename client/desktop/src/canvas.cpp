#include "canvas.h"
#include <QFileInfo>
#include <QPaintEvent>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QFontDatabase>

Canvas::Canvas()
{
    // Initialize paths and fonts
    QFileInfo file{__FILE__};
    QString resPath = file.absolutePath() + "/../res/";
    QFontDatabase::addApplicationFont(resPath + "font.ttf");

    soundEffect.setAudioDevice(QMediaDevices::defaultAudioOutput());
    soundEffect.setSource(QUrl::fromLocalFile(resPath + "turn-signals.wav"));
    soundEffect.setLoopCount(QSoundEffect::Infinite);
    soundEffect.setVolume(1.0f);

    brush = QBrush(QColor(0xa6, 0x2e, 0x39));
}

void Canvas::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor(64, 32, 64)));
    painter.translate(40, 40);

    // Call drawing functions
    drawBatteryLevel();
    drawTmpLevel();
    turnSignal();
    drawSpeedometer();

    painter.end();
}

void Canvas::drawBatteryLevel(void)
{
    // Draw battery level
    QString BatteryText = QString::number(BatteryLevel) + " %";
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(660, 340, 100, 100), Qt::AlignCenter, BatteryText);

    // Draw battery icon

    iconFont = QFont{"Material Icons", 120, QFont::Thin};

    if (BatteryLevel < 25)
    {
        painter.setPen(QColor(Qt::red));
    }
    else if (BatteryLevel >= 25 && BatteryLevel <= 49)
    {
        painter.setPen(QColor(Qt::yellow));
    }
    else if (BatteryLevel >= 50)
    {
        painter.setPen(QColor(Qt::green));
    }
    iconFont.setStretch(100);
    painter.setFont(iconFont);
    painter.drawText(QRect(685, 255, 50, 120), Qt::AlignCenter, QChar(0xebdc));

    // battery level square
    QRect squareRect(689, 375, 42, -BatteryLevel);
    if (BatteryLevel < 25)
    {
        painter.fillRect(squareRect, Qt::red);
    }
    else if (BatteryLevel >= 25 && BatteryLevel <= 49)
    {
        painter.fillRect(squareRect, Qt::yellow);
    }
    else if (BatteryLevel >= 50)
    {
        painter.fillRect(squareRect, Qt::green);
    }
}

void Canvas::drawTmpLevel(void)
{
    // Draw temperature level

    QString TempratureText = QString::number(temprature) + " °C";
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(660, 435, 100, 100), Qt::AlignCenter, TempratureText);

    // Draw temperature icon
    iconFont = QFont{"Material Icons", 40, QFont::Normal};
    if (temprature < 5)
    {
        painter.setPen(QColor(Qt::white));
    }
    else if (temprature >= 5 && temprature <= 39)
    {
        painter.setPen(QColor(Qt::blue));
    }
    else if (temprature > 39)
    {
        painter.setPen(QColor(Qt::red));
    }
    painter.setFont(iconFont);
    iconFont.setPixelSize(40);
    painter.drawText(QRect(660, 400, 100, 100), Qt::AlignCenter, QChar(0xf076));
}

void Canvas::turnSignal(void)
{
    static int counter{-1};
    constexpr int PERIOD{20};

    if (leftSide || rightSide)
    {
        counter = (counter + 1) % PERIOD;

        if (!soundEffect.isPlaying())
        {
            soundEffect.play();
        }

        if (counter < (PERIOD / 2))
        {
            painter.setPen(QColor(0, 255, 0));
            painter.setFont(QFont{"Material Icons", 46, QFont::Normal});

            if (leftSide)
            {
                painter.drawText(QRect(10, 5, 40, 40), Qt::AlignCenter, QChar(0xe5c4));
            }

            if (rightSide)
            {
                painter.drawText(QRect(620, 5, 40, 40), Qt::AlignCenter, QChar(0xe5c8));
            }
        }
    }
    else
    {
        counter = -1;
        soundEffect.stop();
    }
}

void Canvas::drawSpeedometer(void)
{
    // Draw speedometer arc
    int radius{330};
    int centerX = radius, centerY = radius;

    QPoint center{radius, radius};
    int startAngle = -33 * 16;
    int endAngle = 246 * 16;

    painter.setPen(QPen(Qt::white, 9, Qt::SolidLine, Qt::FlatCap));
    painter.drawArc(QRect(0, 0, 2 * center.x(), 2 * center.y()), startAngle, endAngle);
    painter.drawEllipse(center, 10, 10);

    // Draw main markings at evenly spaced intervals
    int numMainMarkings = 13;
    int numSubMarkings = 1;          // Adjust the number of sub-markings
    int markingThickness = 7;        // Adjust the thickness of the markings
    int markingRadius = radius - 25; // Adjust the radius where markings are drawn
    int markingLength = 30;

    int startA = -30 * 16; // Start angle for the markings
    int endA = 210 * 16;   // End angle for the markings

    QPen pen(Qt::white);
    pen.setWidth(markingThickness); // Thickness of the markings outline in pixels
    qreal angleIncrement = static_cast<qreal>(endA - startA) / (numMainMarkings - 1);

    for (int i = 0; i < numMainMarkings; ++i)
    {
        qreal angle = startA + i * angleIncrement;
        qreal angleRadians = qDegreesToRadians(angle / 16.0);

        qreal startX = centerX + (radius - 10) * qCos(angleRadians);
        qreal startY = centerY - (radius - 10) * qSin(angleRadians);
        qreal endX = centerX + (radius - markingLength) * qCos(angleRadians);
        qreal endY = centerY - (radius - markingLength) * qSin(angleRadians);

        painter.setPen(QPen(Qt::white, markingThickness));
        painter.drawLine(QPointF(startX, startY), QPointF(endX, endY));

        // Draw sub-markings
        if (i != numMainMarkings - 1)
        {
            qreal subMarkingAngleIncrement = (endA - startA) / (numMainMarkings - 1) / (numSubMarkings + 1);
            qreal subMarkingAngle = angle + subMarkingAngleIncrement;

            for (int j = 0; j < numSubMarkings; ++j)
            {
                qreal subMarkingStartX = centerX + (markingRadius - 1) * qCos(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingStartY = centerY - (markingRadius - 1) * qSin(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingEndX = subMarkingStartX + markingLength * 0.7 * qCos(qDegreesToRadians(subMarkingAngle / 16.0));
                qreal subMarkingEndY = subMarkingStartY - markingLength * 0.7 * qSin(qDegreesToRadians(subMarkingAngle / 16.0));

                painter.setPen(QPen(Qt::white, 3));
                painter.drawLine(QPointF(subMarkingStartX, subMarkingStartY), QPointF(subMarkingEndX, subMarkingEndY));

                subMarkingAngle += subMarkingAngleIncrement;

                // Draw sub-sub-markings between main and sub markings
                qreal subSubMarkingAngleIncrement = subMarkingAngleIncrement / (numSubMarkings + 1);
                qreal subSubMarkingAngle = subMarkingAngle - subSubMarkingAngleIncrement;
                qreal subSubMarkingAngle2 = angle + subSubMarkingAngleIncrement;

                for (int k = 0; k < numSubMarkings; ++k)
                {
                    qreal subSubMarkingStartX = centerX + (markingRadius + 5) * qCos(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingStartY = centerY - (markingRadius + 5) * qSin(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingEndX = subSubMarkingStartX + markingLength * 0.5 * qCos(qDegreesToRadians(subSubMarkingAngle / 16.0));
                    qreal subSubMarkingEndY = subSubMarkingStartY - markingLength * 0.5 * qSin(qDegreesToRadians(subSubMarkingAngle / 16.0));

                    painter.setPen(QPen(Qt::white, 3));
                    painter.drawLine(QPointF(subSubMarkingStartX, subSubMarkingStartY), QPointF(subSubMarkingEndX, subSubMarkingEndY));

                    subSubMarkingAngle -= subSubMarkingAngleIncrement;

                    qreal subSubMarkingStartX2 = centerX + (markingRadius + 5) * qCos(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingStartY2 = centerY - (markingRadius + 5) * qSin(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingEndX2 = subSubMarkingStartX2 + markingLength * 0.5 * qCos(qDegreesToRadians(subSubMarkingAngle2 / 16.0));
                    qreal subSubMarkingEndY2 = subSubMarkingStartY2 - markingLength * 0.5 * qSin(qDegreesToRadians(subSubMarkingAngle2 / 16.0));

                    painter.setPen(QPen(Qt::white, 3));
                    painter.drawLine(QPointF(subSubMarkingStartX2, subSubMarkingStartY2), QPointF(subSubMarkingEndX2, subSubMarkingEndY2));

                    subSubMarkingAngle2 += subSubMarkingAngleIncrement;

                    // Draw sub-sub-markings
                    qreal subSubMarkingAngleIncrement = subMarkingAngleIncrement / (numSubMarkings + 1);
                    qreal subSubMarkingAngle = subMarkingAngle + subSubMarkingAngleIncrement;
                    qreal subSubMarkingAngle2 = subMarkingAngle - subSubMarkingAngleIncrement;
                }
            }
        }
    }

    // Draw the integer labels for the speedometer
    int labelRadius = radius - 65; // Adjust the radius where labels are drawn
    int labelPadding = 10;         // Adjust the padding between labels

    for (int speed = 0; speed <= 240; speed += 20)
    {
        qreal angle = startA + (endA - startA) * (1.0 - (speed / 240.0)); // Invert the angle calculation
        qreal angleRadians = qDegreesToRadians(angle / 16.0);

        QString label = QString::number(speed);
        QFont labelFont("Arial", 18, QFont::Bold);
        QFontMetrics labelMetrics(labelFont);
        QRect labelRect = labelMetrics.boundingRect(label);
        int labelWidth = labelRect.width();
        int labelHeight = labelRect.height();

        qreal labelX = centerX + (labelRadius + labelPadding) * qCos(angleRadians) - labelWidth / 2.0;
        qreal labelY = centerY - (labelRadius + labelPadding) * qSin(angleRadians) + labelHeight / 4.0;

        painter.setFont(labelFont);
        painter.setPen(QPen(Qt::white));
        painter.drawText(labelX, labelY, label);
    }
    // Draw the speedometer needle

    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(center, 11, 11);

    int needleLength = radius - 45;
    qreal needleAngle = 210 - speed; // Change the value for the speed / needle's position
    qreal needleX = centerX + needleLength * qCos(qDegreesToRadians(needleAngle));
    qreal needleY = centerY - needleLength * qSin(qDegreesToRadians(needleAngle));
    painter.setPen(QPen(Qt::red, 7));
    painter.drawLine(centerX, centerY, needleX, needleY);

    // Draw digital speedometer
    if (connection == true)
    {
        QString SpeedText = QString::number(speed) + " km/h";
        painter.setPen(QColor(255, 255, 255));
        painter.setFont(QFont("Arial", 20));
        painter.drawText(QRect(210, 440, 250, 100), Qt::AlignCenter, SpeedText);

        iconFont = QFont{"Material Icons", 50, QFont::Normal};
        painter.setFont(iconFont);
        painter.drawText(QRect(255, 350, 150, 150), Qt::AlignCenter, QChar(0xe9e4));
    }
    else
    {

        QString SpeedText = "Connection lost";
        painter.setPen(QColor(Qt::red));
        painter.setFont(QFont("Arial", 20));
        painter.drawText(QRect(205, 440, 250, 100), Qt::AlignCenter, SpeedText);

        iconFont = QFont{"Material Icons", 50, QFont::Normal};
        painter.setFont(iconFont);
        painter.drawText(QRect(255, 350, 150, 150), Qt::AlignCenter, QChar(0xe628));
    }
}
