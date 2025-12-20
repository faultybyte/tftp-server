#include <iostream>
#include <Server.hpp>

int main(int argc, char* argv[]) {
    uint16_t port = 6969;

    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    try {
        TftpServer server(port);
        server.start();
    } catch (const std::exception& err) {
        std::cerr << "Fatal Server Error: " << err.what() << '\n';
        return 1;
    }

    return 0;
}