#include "Client.h"


Client::Client(std::string ip_, int port_, Protocol protocol_)
    : ip(std::move(ip_)), port(port_), protocol(protocol_)
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
    address.sin_addr.s_addr = ip.compare("debug") == 0 ? htonl(INADDR_LOOPBACK) : inet_addr(ip.c_str());

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

std::string* Client::sendMessage(std::string& message) {
    // Инициализируем буфер
    auto message_size = message.size();
    message_size = message_size < MAX_SIZE ? message_size : MAX_SIZE;

    buffer.clear();
    buffer.resize(message_size + 1);

    if (protocol == Protocol::TCP)
    {
        // Отправляем сообщение
        send(sender, message.c_str(), message_size, 0);
        // Получаем ответ
        recv(sender, &buffer[0], MAX_SIZE, 0);
    }
    else if (protocol == Protocol::UDP)
    {
        // Отправляем сообщение
        sendto( sender, message.c_str(), message_size, 0, (struct sockaddr *) &address, sizeof(address) );

        // Получаем ответ
        socklen_t address_size = sizeof address;
        int bytes = recvfrom(sender, &buffer[0], MAX_SIZE, 0, (struct sockaddr *) &address, &address_size);

        if (bytes <= 0)
        {
            this->stop();
            throw std::runtime_error("Problem with getting answer from server.");
        }
        buffer.at(bytes) = '\0';
    }

    return new std::string(buffer.begin(), buffer.end());
}
