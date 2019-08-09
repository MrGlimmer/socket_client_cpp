#include <iostream>
#include <cstring>
#include "Client.h"

// TODO: async

int main(int argc, char *argv[]) {
    int         port = 3425;
    char*       ip = nullptr;
    Protocol    protocol = Protocol::TCP;

    if (argc > 1)
    {
        port = static_cast<int>(strtol(argv[1], nullptr, 0));

        if (argc > 2 && strncmp(argv[2], "UDP", 3) == 0)
        {
            protocol = Protocol::UDP;
        }

        // Debug - looppack, ip
        if (argc > 3)
        {
            ip = argv[3];
        }
    }

    try {
        auto client = new Client(port, ip, protocol);

        std::cout << "Client launched at port " << port
                  << " and uses " << (protocol == Protocol::TCP ? "TCP" : "UDP")
                  << " protocol." << std::endl;

        while (true)
        {
            char* message = new char();
            std::cout << "Write message: ";
            std::cin.getline(message, MAX_SIZE);

            if (std::strcmp(message, "exit") == 0) break;

            std::cout << "**********************" << std::endl;
            std::cout << "Answer:" << std::endl;
            std::cout << client->sendMessage(message) << std::endl;
            std::cout << "**********************" << std::endl;
        }
    } catch (SocketCreationException &e) {
        std::cout << "Socket creation error: " << e.what() << std::endl;
    } catch (ConnectionException &e) {
        std::cout << "Connection error: " << e.what() << std::endl;
    } catch (ReceivedException &e) {
        std::cout << "Received error: " << e.what() << std::endl;

    }


    return EXIT_SUCCESS;
}