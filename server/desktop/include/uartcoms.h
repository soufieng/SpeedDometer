#ifndef UARTCOMS_H
#define UARTCOMS_H

#include <QThread>
#include "comservices.h"

class UARTComs : public COMServices, public QThread
{
    std::atomic<bool> end{false};

    void run(void) override;

public:
    UARTComs()
    {
        start();
    };

    ~UARTComs()
    {
        end = true;
        wait();
    }
};

#endif