#include <mutex>
#include <QDebug>
#include "uartcoms.h"
#include <QSerialPort>

void UARTComs::run(void)
{
    QSerialPort serial;

    serial.setPortName(Setting::UARTX::uartPort);
    serial.setBaudRate(Setting::UARTX::baudRate);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while (!end)
    {
        if (serial.open(QIODevice::WriteOnly))
        {
            while (!end)
            {

                uint8_t tmpBuff[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{mtx};
                    memcpy(tmpBuff, buffer, sizeof(tmpBuff));
                }

                if (sizeof(buffer) == (serial.write(reinterpret_cast<const char *>(buffer), sizeof(buffer))))
                {
                    if (serial.waitForBytesWritten(Setting::INTERVAL))
                    {
                        status = true;
                        msleep(Setting::INTERVAL / 2);
                    }
                    else
                    {
                        qDebug() << "Timeout error ....";
                        status = false;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Failed to write data";
                    status = false;
                    break;
                }
            }
        }
        else
        {
            qDebug() << "Failed to open serial port for writing";
            status = false;
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}