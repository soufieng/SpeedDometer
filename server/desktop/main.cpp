#include "dialog.h"
#ifdef UART_COM
#include "uartcoms.h"
#else
#include "tcpservices.h"
#endif
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
#ifdef UART_COM
    UARTComs uartComs;
    Dialog dialog(&uartComs);
#else
    TCPServices services;
    Dialog dialog(&services);
#endif
    dialog.show();

    return app.exec();
}
