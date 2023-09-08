#include "window.h"
#include "setting.h"
#include "comservice.h"

Window::Window(COMService *com) : communication{com}
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("Client");
    setFixedHeight(Setting::Client::Windows::Height);
    setFixedWidth(Setting::Client::Windows::Width);
    setContentsMargins(0, 0, 0, 0);

    layout.addWidget(&canvas, 0, 0, 1, 3);
    layout.setContentsMargins(0, 0, 0, 0);

    setLayout(&layout);

    canvas.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    canvas.setFixedSize(800, 560);

    // Connect the timer's timeout signal to the refresh slot
    connect(&timer, &QTimer::timeout, this, &Window::refresh);

    // Set the timer interval (in milliseconds)
    timer.start(Setting::INTERVAL); // Refresh every 1000 ms (1 second)
}

void Window::refresh()
{
    if (communication->getStatus())
    {
        canvas.setConnection(true);
        canvas.setSpeed(communication->getSpeed());
        canvas.setBatteryLevel(communication->getBattryLevel());
        canvas.setTemprature(communication->getTemperature());
        canvas.setLights(communication->getLightLeft(), communication->getLightRight());
    }
    else
    {
        canvas.setConnection(false);
        canvas.setSpeed(0);
        canvas.setBatteryLevel(0);
        canvas.setTemprature(0);
        canvas.setLights(false, false);
    }

    // Trigger the repaint of the canvas
    /*
        canvas.setBatteryLevel(50);
        canvas.setTemprature(45);
        canvas.setSpeed(102);
        canvas.setConnection(false)
        ;canvas.setLights(true, true);
    */

    canvas.update();
}
