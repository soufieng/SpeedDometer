#include <mutex>
#include <QDebug>
#include "uartcom.h"
#include <QSerialPort>

void UARTCom::run(void)
{
    QSerialPort serial;

    serial.setPortName(Setting::UARTX::uartPort);
    serial.setBaudRate(Setting::UARTX::baudRate);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setReadBufferSize(Setting::Signal::BUFSIZE);

    while (!end)
    {
        if (serial.open(QIODevice::ReadOnly))
        {
            while (!end && serial.isReadable())
            {
                serial.clear();
                uint8_t temp[sizeof(buffer)]{0};

                if (serial.waitForReadyRead())
                {
                    if (sizeof(temp) == serial.read(reinterpret_cast<char *>(temp), sizeof(temp)))
                    {
                        std::scoped_lock<std::mutex> lock(mtx);
                        memcpy(buffer, temp, sizeof(buffer));
                        status = true;
                    }
                    else
                    {
                        qDebug() << "Failed to read data from the port";
                        status = false;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Timeout error";
                    status = false;
                    break;
                }
            }
        }
        else
        {
            qDebug() << "Failed to open serial port";
            status = false;
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}
