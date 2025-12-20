#include <iostream>
#include <vector>
#include <arpa/inet.h>

#include "Server.hpp"
#include "PacketParser.hpp"

TftpServer::TftpServer(uint16_t port) : port(port), socket(port) {}

void TftpServer::handlePacket(const Address& from, std::unique_ptr<Packet> packet) {
    std::cout << "Received packet type " << static_cast<int>(packet->getOpcode()) 
            << " from IP " << from.getIP() << '\n';
}

void TftpServer::start() {
    std::vector<uint8_t> buffer(1024);
    std::cout << "TFTP Server started on port " << port << "...\n";

    while (true) {
        Address sender;

        buffer.resize(1024);
        ssize_t bytes = socket.receiveFrom(buffer.data(), buffer.size(), sender);

        if (bytes < 0) {
            std::cerr << "Socket receive error\n";
            continue;
        }

        try {
            buffer.resize(bytes);

            auto packet = PacketParser::parse(buffer);
            handlePacket(sender, std::move(packet));
        } catch (const std::exception& err) {
            std::cerr << "Bad packet from " << sender.getIP() << ": " << err.what() << '\n';
        }
    }
}
