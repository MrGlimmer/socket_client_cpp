#ifndef SOCKET_CLIENT_CPP_CLIENT_H
#define SOCKET_CLIENT_CPP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>
#include <vector>

const int MAX_SIZE = 1024;

// Для облегчения восприятия размещаем константы в enum
enum Protocol : int {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Client {
private:
    // Адрес подключения
    sockaddr_in address = sockaddr_in();

    // Сокет
    int     sender = 0;

    // Вспомогательные атрибуты
    int     port = 0,
            protocol = 0;

    std::string ip;

    // Буфер для взаимодействия с сокетами
    std::vector<char> buffer = std::vector<char>();

public:
    explicit Client(std::string ip, int port, Protocol protocol = Protocol::TCP);
    ~Client();

    // Метод остановки работы клиента
    void stop();
    // Метод отправки сообщения на сервер
    std::string* sendMessage(std::string& message);
};

#endif //SOCKET_CLIENT_CPP_CLIENT_H
