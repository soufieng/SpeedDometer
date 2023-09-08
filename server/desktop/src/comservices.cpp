#include <climits>
#include "comservices.h"

void COMServices::insert(uint32_t start, uint32_t length, uint32_t value)
{
    int pos = start % CHAR_BIT;
    int index = start / CHAR_BIT;

    std::scoped_lock<std::mutex> locker(mtx);

    for (int i = 0; i < length; i++)
    {
        // get the ith bit of value

        uint8_t getBit = (uint8_t)((value >> i) & 1);

        // if the ith bit is 1 then
        if (getBit == 0)
        {
            // set the pos bit of buffer[index]
            buffer[index] &= ~(1 << pos);
        }
        else
        {
            // else reset pos bit of buffer[index]
            buffer[index] |= (1 << pos);
        }
        // increment pos
        pos++;
        // if pos == CHAR_BIT
        if (pos == CHAR_BIT)
        {
            pos = 0; // pos = 0;
            index++; // index++;
        }
    }
}

void COMServices::setSpeed(uint32_t value)
{
    insert(Setting::Signal::Speed::START, Setting::Signal::Speed::LENGTH, value);
}

void COMServices::setTemperature(int32_t value)
{

    insert(Setting::Signal::Temperature::START, Setting::Signal::Temperature::LENGTH, value);
}

void COMServices::setBattryLevel(uint32_t value)
{
    insert(Setting::Signal::BatteryLevel::START, Setting::Signal::BatteryLevel::LENGTH, value);
}

void COMServices::setLightLeft(bool value)
{
    insert(Setting::Signal::Light::Left::START, Setting::Signal::Light::Left::LENGTH, value);
}

void COMServices::setLightRight(bool value)
{
    insert(Setting::Signal::Light::Right::START, Setting::Signal::Light::Right::LENGTH, value);
}
