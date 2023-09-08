#ifndef WINDOW_H
#define WINDOW_H

#include <QTimer>
#include "canvas.h"
#include <QDialog>
#include <QGridLayout>
#include "comservice.h"

class Window : public QDialog
{
    QTimer timer;
    Canvas canvas;
    QGridLayout layout;
    COMService *communication{nullptr};

public:
    Window(COMService *com);

private:
    void refresh();
};

#endif
