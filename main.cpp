#include <iostream>
#include <cstring>
#include <limits>
#include <sstream>
#include "Client.h"

// TODO: async

int main(int argc, char *argv[]) {
    // Инициализируем переменные
    std::stringstream   ip;
    int                 port = 3425;
    Protocol            protocol = Protocol::TCP;

    if (argc > 1)
    {
        // Собираем параметры
        ip << argv[1];

        if (argc > 2) port = static_cast<int>(strtol(argv[2], nullptr, 0));

        if (argc > 3 && strncmp(argv[3], "UDP", 3) == 0) protocol = Protocol::UDP;

        try {
            // Создаем клиента
            auto client = new Client(ip.str(), port, protocol);

            std::cout << "Client launched at port " << port
                      << " and uses " << (protocol == Protocol::TCP ? "TCP" : "UDP")
                      << " protocol." << std::endl;

            // Начинаем цикл отправки сообщений, чтобы выйти -> вводим exit
            while (true)
            {
                auto message = new std::string();
                std::cout << "Write message: ";
                std::getline(std::cin, *message);

                if (message->compare("exit") == 0) break;

                std::cout << "**********************" << std::endl;
                std::cout << "Answer:" << std::endl;
                std::cout << client->sendMessage(*message)->c_str() << std::endl;
                std::cout << "**********************" << std::endl;
            }
        } catch (std::runtime_error &e) {
            std::cout << "Run time error: " << e.what() << std::endl;
        } catch (std::exception &e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cout << "Please, set server ip like 1st parameter." << std::endl;
    }

    return EXIT_SUCCESS;
}