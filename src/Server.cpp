#include <iostream>
#include <fstream>
#include <vector>
#include <arpa/inet.h>

#include "Server.hpp"
#include "PacketParser.hpp"
#include "DataPacket.hpp"

TftpServer::TftpServer(uint16_t port) : port(port), socket(port) {}

void TftpServer::handleRRQ(const Address& client, const ReadRequestPacket* packet) {
    std::cout << "Client " << client.getIP() 
            << " requested file: " << packet->getFilename() << '\n';

    std::ifstream file(packet->getFilename(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "File not found!\n";   // TODO: Implement ERROR packets
        return;
    }

    std::vector<uint8_t> buffer(512);

    file.read(reinterpret_cast<char*>(buffer.data()), 512);
    std::streamsize bytesRead = file.gcount();
    buffer.resize(bytesRead);

    DataPacket responsePacket(1, buffer);
    std::vector<uint8_t> raw = responsePacket.serialize();

    ssize_t bytesSent = socket.sendTo(raw.data(), raw.size(), client);
    if (bytesSent == -1) {
        std::cerr << "Failed to send DATA packet\n";
        return;
    }
}

void TftpServer::handlePacket(const Address& from, std::unique_ptr<Packet> packet) {
    if (packet->getOpcode() == Opcode::RRQ) {
        ReadRequestPacket* rrq = dynamic_cast<ReadRequestPacket*>(packet.get());

        if (rrq)
            handleRRQ(from, rrq);
    }

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
