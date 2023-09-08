#ifndef TCPSERVICES_H
#define TCPSERVICES_H
#include <QDebug>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "comservices.h"
#include <sys/socket.h>
#include <thread>

class TCPServices : public COMServices
{
private:
    int socket_fd;
    std::atomic<bool> end{false};

    std::thread thrd{&TCPServices::run, this};

    void run(void) override;

public:
    TCPServices() = default;

    ~TCPServices()
    {
        end = true;
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        thrd.join();
    }
};

#endif