#ifndef COMSERVICES_H
#define COMSERVICES_H

#include <mutex>
#include <cstdint>
#include <atomic>
#include "setting.h"

class COMServices
{
    void insert(uint32_t start, uint32_t length, uint32_t value);

protected:
    std::mutex mtx;
    std::atomic<bool> status{false};
    uint8_t buffer[Setting::Signal::BUFSIZE]{};

    virtual void run(void) = 0;

public:
    bool getStatus(void) { return status; }

    void setSpeed(uint32_t value);

    void setTemperature(int32_t value);

    void setBattryLevel(uint32_t value);

    void setLightLeft(bool value);

    void setLightRight(bool value);

    virtual ~COMServices() = default;
};

#endif