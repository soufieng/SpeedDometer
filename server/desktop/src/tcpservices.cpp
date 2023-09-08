#include "setting.h"
#include "tcpservices.h"
#include <arpa/inet.h>
#include <QDebug>

void TCPServices::run(void)
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address{0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Setting::TCP::PORT);
    server_address.sin_addr.s_addr = inet_addr(Setting::TCP::IP);

    if (socket_fd == -1)
    {
        qDebug() << "Failed to create socket.";
        std::exit(1);
    }

    int temp = 1;
    if (0 != setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)))
    {
        qDebug() << "setsockopt(SO_REUSEADDR) failed";
        std::exit(1);
    }

    if (0 != bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        qDebug() << "failed to bind socket";
        std::exit(1);
    }
    if (0 != listen(socket_fd, 1))
    {
        qDebug() << "Failed to listen to the port...";
        std::exit(1);
    }

    while (!end) // Continue until 'end' flag is set
    {
        sockaddr_in cli{0};
        socklen_t len = sizeof(cli);

        int connfd = accept(socket_fd, (sockaddr *)&cli, &len);

        if (connfd != -1)
        {
            while (!end) // Continue sending buffer until 'end' flag is set
            {
                status = true;
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{mtx};
                    memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) != write(connfd, temp, sizeof(temp)))
                {
                    status = false;

                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL / 2));
            }
        }
        close(connfd); // Close the client connection
    }
}
