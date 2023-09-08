#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include <QCursor>
#include "comservices.h"

class Dialog : public QDialog
{

    QSlider sliderSpeed;
    QSlider sliderTemp;
    QSlider sliderBattery;
    // Slider value LABELS
    QLabel speedValue{"0 Kph"};
    QLabel tempValue{"0 °C"};
    QLabel batteryValue{"0%"};
    // text labels
    QLabel textSpeed{"Speed:"};
    QLabel textTemp{"Temperature:"};
    QLabel textBattery{"Battery level:"};
    QLabel textLightSignal{"Light Signal:"};
    // check boxes
    QCheckBox checkBoxLeft{"Left"};
    QCheckBox checkBoxRight{"Right"};
    QCheckBox checkBoxWarning{"Warning"};
    QGridLayout layout;

    QGridLayout bottomLayout;

    QCursor cursor;
    COMServices *communication{nullptr};

public:
    Dialog(COMServices *coms);

private:
    void updateValueSpeed(int value);

    void updateValueTemp(int value);

    void updateValueBattery(int value);

    void onCheckboxToggledRight(bool checked);

    void onCheckboxToggledLeft(bool checked);

    void onCheckboxToggledWarning(bool checked);
};

#endif
