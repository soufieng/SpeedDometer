#include "window.h"
#ifdef UART_COM
#include "uartcom.h"
#else
#include "tcpservice.h"
#endif
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
#ifdef UART_COM
    UARTCom uartCom;
    Window myWindow(&uartCom);
#else
    TCPService service;
    Window myWindow(&service);
#endif
    myWindow.show();

    return app.exec();
}
