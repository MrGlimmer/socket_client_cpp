#ifndef SOCKET_CLIENT_CPP_CLIENT_H
#define SOCKET_CLIENT_CPP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>
#include <cstring>
#include <arpa/inet.h>

const int MAX_SIZE = 1024;

enum Protocol : int {
    TCP =  SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Client {
private:
    sockaddr_in address = sockaddr_in();

    int     sender = 0,
            port = 0,
            protocol = 0;

    char*   ip;

public:
    explicit Client(int port, char* ip, Protocol protocol = Protocol::TCP);
    ~Client();

    void stop();
    char* sendMessage(char* message);
};

class SocketCreationException : public std::exception
{
public:
    SocketCreationException() = default;
    ~SocketCreationException() override = default;

    const char* what() const noexcept override {
        return "Problem with socket creation.";
    }
};

class ConnectionException : public std::exception
{
public:
    ConnectionException() = default;
    ~ConnectionException() override = default;

    const char* what() const noexcept override {
        return "Problem with connection to TCP server.";
    }
};

class ReceivedException : public std::exception
{
public:
    ReceivedException() = default;
    ~ReceivedException() override = default;

    const char* what() const noexcept override {
        return "Problem with getting answer from server.";
    }
};

#endif //SOCKET_CLIENT_CPP_CLIENT_H
