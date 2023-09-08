#ifndef SETTING_H
#define SETTING_H

namespace Setting
{
    namespace Server
    {
        namespace Window
        {
            constexpr int Width{800};
            constexpr int Height{150};
        }
    }

    namespace Client
    {
        namespace Windows
        {
            constexpr int Width{800};
            constexpr int Height{560};
        }
    }

    constexpr int INTERVAL{50};

    namespace Signal
    {
        namespace Speed
        {
            constexpr int MIN{0};
            constexpr int MAX{240};
            constexpr int START{0};
            constexpr int LENGTH{8};
        }

        namespace Temperature
        {
            constexpr int MIN{-60};
            constexpr int MAX{60};
            constexpr int START{8};
            constexpr int LENGTH{7};
        }

        namespace BatteryLevel
        {
            constexpr int MIN{0};
            constexpr int MAX{100};
            constexpr int START{15};
            constexpr int LENGTH{7};
        }

        namespace Light
        {
            namespace Left
            {
                constexpr int MIN{0};
                constexpr int MAX{1};
                constexpr int START{22};
                constexpr int LENGTH{1};
            }
            namespace Right
            {
                constexpr int MIN{0};
                constexpr int MAX{1};
                constexpr int START{23};
                constexpr int LENGTH{1};
            }
        }

        constexpr int BUFSIZE{3};
    }
#ifdef UART_COM

    namespace UARTX
    {
        constexpr int baudRate{115200};
        constexpr char uartPort[] = {"/dev/ttyUSB0"};
    }

#else
    namespace TCP
    {
        constexpr int PORT{12345};
        const char IP[]{"127.0.0.1"};
    }
#endif
}

#endif
