#ifndef UARTCOM_H
#define UARTCOM_H

#include <QThread>
#include "comservice.h"

class UARTCom : public COMService, public QThread
{
    std::atomic<bool> end{false};

    void run(void) override;

public:
    UARTCom()
    {
        start();
    };

    ~UARTCom()
    {
        end = true;
        wait();
    }
};

#endif