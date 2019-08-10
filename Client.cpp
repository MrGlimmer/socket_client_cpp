
#include "Client.h"

Client::Client(char* ip_, int port_, Protocol protocol_)
    :  ip(ip_), port(port_), protocol(protocol_)
{
    // Инициализируем сокет
    sender = socket(AF_INET, protocol, 0);
    if (sender < 0)
    {
        this->stop();
        throw std::runtime_error("Problem with socket creation.");
    }

    // Инициализируем адрес
    bzero( &address, sizeof( sockaddr_in  ) );
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = strcmp(ip, "debug") == 0 ? htonl(INADDR_LOOPBACK) : inet_addr(ip);

    if (protocol == Protocol::TCP)
    {
        // Соединение с сервером при TCP
        if (connect(sender, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            this->stop();
            throw std::runtime_error("Problem with connection to TCP server.");
        }
    }
}

Client::~Client()
{
    this->stop();
}

void Client::stop() {
    // Закрываем соединение
    close(sender);
}

char* Client::sendMessage(char* message) {
    // Инициализируем буфер
    auto message_size = strlen(message);
    message_size = message_size < MAX_SIZE ? message_size : MAX_SIZE;

    if (protocol == Protocol::TCP)
    {
        // Отправляем сообщение
        send(sender, message, message_size, 0);
        // Получаем ответ
        recv(sender, buffer, MAX_SIZE, 0);
    }
    else if (protocol == Protocol::UDP)
    {
        // Отправляем сообщение
        sendto( sender, message, message_size, 0, (struct sockaddr *) &address, sizeof(address) );

        // Получаем ответ
        socklen_t address_size = sizeof address;
        int bytes = recvfrom(sender, buffer, MAX_SIZE, 0, (struct sockaddr *) &address, &address_size);

        if (bytes <= 0)
        {
            this->stop();
            throw std::runtime_error("Problem with getting answer from server.");
        }
        buffer[bytes] = '\0';
    }

    return buffer;
}

void Client::clearBuffer() {
    // bzero( &buffer, sizeof( buffer  ) );
    delete[] buffer;
    buffer = new char[MAX_SIZE];
}
