#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <atomic>

class TCPService : public COMService
{
    int socket_fd;
    std::atomic<bool> end{false};
    std::thread thrd{&TCPService::run, this};

    void run(void) override;

public:
    TCPService() = default;

    ~TCPService()
    {
        end = true;
        close(socket_fd);
        thrd.join();
    }
};

#endif