#include "dialog.h"
#include "setting.h"
#include "comservices.h"

Dialog::Dialog(COMServices *coms) : communication{coms}
{

    setWindowFlags(Qt::WindowStaysOnTopHint);
    setFixedHeight(Setting::Server::Window::Height);
    setFixedWidth(Setting::Server::Window::Width);
    setWindowTitle("Server");
    setLayout(&layout);

    // Set slider properties
    sliderSpeed.setCursor(QCursor(Qt::PointingHandCursor));
    sliderSpeed.setOrientation(Qt::Horizontal);
    sliderSpeed.setRange(0, 240);
    sliderSpeed.setValue(0);

    sliderTemp.setCursor(QCursor(Qt::PointingHandCursor));
    sliderTemp.setOrientation(Qt::Horizontal);
    sliderTemp.setRange(-60, 60);
    sliderTemp.setValue(0);

    sliderBattery.setCursor(QCursor(Qt::PointingHandCursor));
    sliderBattery.setOrientation(Qt::Horizontal);
    sliderBattery.setRange(0, 100);
    sliderBattery.setValue(0);

    speedValue.setAlignment(Qt::AlignLeft);
    speedValue.setFixedSize(60, 17);
    tempValue.setAlignment(Qt::AlignLeft);
    batteryValue.setAlignment(Qt::AlignLeft);

    // TEXT LABELS
    textSpeed.setAlignment(Qt::AlignRight);
    textTemp.setAlignment(Qt::AlignRight);
    textBattery.setAlignment(Qt::AlignRight);
    textLightSignal.setAlignment(Qt::AlignRight);

    // Speed
    layout.addWidget(&textSpeed, 0, 0);
    layout.addWidget(&sliderSpeed, 0, 1);
    layout.addWidget(&speedValue, 0, 2);

    // Temp
    layout.addWidget(&textTemp, 1, 0);
    layout.addWidget(&sliderTemp, 1, 1);
    layout.addWidget(&tempValue, 1, 2);

    // Battery
    layout.addWidget(&textBattery, 2, 0);
    layout.addWidget(&sliderBattery, 2, 1);
    layout.addWidget(&batteryValue, 2, 2);

    connect(&sliderSpeed, &QSlider::valueChanged, this, &Dialog::updateValueSpeed);
    connect(&sliderTemp, &QSlider::valueChanged, this, &Dialog::updateValueTemp);
    connect(&sliderBattery, &QSlider::valueChanged, this, &Dialog::updateValueBattery);

    // Initialize value labels with initial slider values
    updateValueSpeed(sliderSpeed.value());
    updateValueTemp(sliderTemp.value());
    updateValueBattery(sliderBattery.value());

    // Add the bottom layout to the main layout for checkBox
    layout.addWidget(&textLightSignal, 3, 0);

    layout.addLayout(&bottomLayout, 3, 1);

    checkBoxLeft.setCursor(QCursor(Qt::PointingHandCursor));
    checkBoxRight.setCursor(QCursor(Qt::PointingHandCursor));
    checkBoxWarning.setCursor(QCursor(Qt::PointingHandCursor));

    bottomLayout.addWidget(&checkBoxLeft, 0, 0);
    bottomLayout.addWidget(&checkBoxRight, 0, 1);
    bottomLayout.addWidget(&checkBoxWarning, 0, 2);

    setLayout(&layout);

    // NOTE: Remakes this into three diffrent functions later.
    connect(&checkBoxLeft, &QCheckBox::toggled, this, &Dialog::onCheckboxToggledLeft);
    connect(&checkBoxRight, &QCheckBox::toggled, this, &Dialog::onCheckboxToggledRight);
    connect(&checkBoxWarning, &QCheckBox::toggled, this, &Dialog::onCheckboxToggledWarning);
}

void Dialog::updateValueSpeed(int value)
{

    communication->setSpeed(value);
    speedValue.setText(QString::number(value) + " Kph");
}

void Dialog::updateValueTemp(int value)
{
    communication->setTemperature(value);
    tempValue.setText(QString::number(value) + " °C");
}

void Dialog::updateValueBattery(int value)
{
    communication->setBattryLevel(value);
    batteryValue.setText(QString::number(value) + " %");
}

void Dialog::onCheckboxToggledWarning(bool checked)
{
    if (checked)
    {
        communication->setLightLeft(true);
        communication->setLightRight(true);
    }
    else
    {

        if (!checkBoxLeft.isChecked())
        {
            communication->setLightLeft(false);
        }
        if (!checkBoxRight.isChecked())
        {
            communication->setLightRight(false);
        }
    }
}

void Dialog::onCheckboxToggledRight(bool checked)
{
    if (checked)
    {
        if (!checkBoxWarning.isChecked())
        {
            checkBoxLeft.setChecked(false);
            checkBoxLeft.setEnabled(false);
            checkBoxWarning.setEnabled(true);

            communication->setLightRight(true);
            communication->setLightLeft(false);
        }
        else
        {

            checkBoxLeft.setChecked(false);
            checkBoxLeft.setEnabled(false);
            checkBoxWarning.setEnabled(true);
        }
    }
    else
    {
        checkBoxLeft.setEnabled(true);
        checkBoxWarning.setEnabled(true);
        if (!checkBoxWarning.isChecked())
        {
            if (!checkBoxWarning.isChecked() && checkBoxRight.isChecked())
            {

                communication->setLightRight(true);
            }
            else
            {
                communication->setLightRight(false);
            }
        }
        else
        {
        }
    }
}

void Dialog::onCheckboxToggledLeft(bool checked)
{
    if (checked)
    {
        if (!checkBoxWarning.isChecked())
        {
            checkBoxRight.setChecked(false);
            checkBoxRight.setEnabled(false);
            checkBoxWarning.setEnabled(true);

            communication->setLightRight(false);
            communication->setLightLeft(true);
        }
        else
        {

            checkBoxRight.setChecked(false);
            checkBoxRight.setEnabled(false);
            checkBoxWarning.setEnabled(true);
        }
    }
    else
    {
        checkBoxRight.setEnabled(true);
        checkBoxWarning.setEnabled(true);
        if (!checkBoxWarning.isChecked())
        {
            if (!checkBoxWarning.isChecked() && checkBoxRight.isChecked())
            {

                communication->setLightLeft(true);
            }
            else
            {
                communication->setLightLeft(false);
            }
        }
        else
        {
        }
    }
}