#include <iostream>
#include <csignal>
#include <memory>
#include <Server.hpp>

std::unique_ptr<TftpServer> server;

void handler(int) {
    std::cout << "\nStopping server...\n";

    if (server)
        server->stop();
}

int main(int argc, char* argv[]) {
    uint16_t port = 6969;

    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    signal(SIGINT, handler);

    try {
        server = std::make_unique<TftpServer>(port);
        server->start();
    } catch (const std::exception& err) {
        std::cerr << "Fatal Server Error: " << err.what() << '\n';
        return 1;
    }

    return 0;
}
