#include <iostream>
#include <fstream>
#include <vector>
#include <arpa/inet.h>

#include "Server.hpp"
#include "PacketParser.hpp"
#include "DataPacket.hpp"
#include "AckPacket.hpp"
#include "ErrorPacket.hpp"

static const int TRIES = 5;
static const int TIMEOUT = 1;

TftpServer::TftpServer(uint16_t port) : port(port), socket(port) {}

void TftpServer::handleRRQ(const Address& client, const ReadRequestPacket* packet) {
    std::cout << "Client " << client.getIP() 
            << " requested file: " << packet->getFilename() << '\n';

    std::ifstream file(packet->getFilename(), std::ios::binary);
    if (!file.is_open()) {
        ErrorPacket errPacket(1, "File not found");
        std::vector<uint8_t> raw = errPacket.serialize();

        ssize_t bytesSent = socket.sendTo(raw.data(), raw.size(), client);
        if (bytesSent == -1)
            std::cerr << "Failed to send ERROR packet";
            
        return;
    }

    std::vector<uint8_t> buffer(512);
    ssize_t bytesSent = 0;
    std::streamsize bytesRead = 0;
    int blockNumber = 1;

    socket.setRecieveTimeout(TIMEOUT);

    do {
        file.read(reinterpret_cast<char*>(buffer.data()), 512);
        bytesRead = file.gcount();
        buffer.resize(bytesRead);

        bool failed = true;
        DataPacket responsePacket(blockNumber, buffer);
        std::vector<uint8_t> raw = responsePacket.serialize();

        for (int i = 0; i < TRIES; ++i) {   // Retry Loop
            bytesSent = socket.sendTo(raw.data(), raw.size(), client);
            if (bytesSent == -1) {
                std::cerr << "Failed to send DATA packet\n";
                return;
            }

            std::vector<uint8_t> rxBuffer(512);
            Address sender;

            ssize_t bytesRecieved = socket.receiveFrom(rxBuffer.data(), rxBuffer.size(), sender);
            if (bytesRecieved == -1) {
                std::cerr << "No reply from " + client.getIP() << '\n';
                continue;
            }

            rxBuffer.resize(bytesRecieved);

            try {
                std::unique_ptr<Packet> reply = PacketParser::parse(rxBuffer);
                AckPacket* ack = dynamic_cast<AckPacket*>(reply.get());

                if (ack && ack->getBlockNumber() == blockNumber) {
                    ++blockNumber;
                    failed = false;
                    break;
                }
                
                throw std::runtime_error("Invalid ACK packet from " + client.getIP());
            } catch (const std::runtime_error& err) {
                std::cerr << err.what() << '\n';
            }
        }

        if (failed) {
            std::cerr << "File transfer timed out\n";
            break;
        }

        buffer.resize(512);
    } while (bytesRead == 512);

    socket.setRecieveTimeout(0);
}

void TftpServer::handleWRQ(const Address& client, const WriteRequestPacket* packet) {
    std::cout << "Client " << client.getIP() 
            << " writing to file: " << packet->getFilename() << '\n';

    std::ofstream file(packet->getFilename(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << packet->getFilename() 
            << " for writing\n";

        ErrorPacket err{2, "Access Violation"};
        std::vector<uint8_t> raw = err.serialize();
        
        socket.sendTo(raw.data(), raw.size(), client);
        
        return; 
    }

    AckPacket ack{0};
    int expectedBlock = 1;
    int prevBlock = 0;
    int trial = 1;
    std::streamsize bytesWritten = 0;
    std::vector<uint8_t> raw = ack.serialize();

    socket.sendTo(raw.data(), raw.size(), client);

    socket.setRecieveTimeout(TIMEOUT);

    while (true) {
        Address sender;
        std::vector<uint8_t> rxBuffer(516);

        ssize_t bytesRecieved = 
            socket.receiveFrom(rxBuffer.data(), rxBuffer.size(), sender);
        if (bytesRecieved == -1) {
            std::cerr << "No reply from " << client.getIP() << '\n';
            if (trial++ > TRIES) break;

            AckPacket retry{static_cast<uint16_t>(prevBlock)};
            raw = retry.serialize();
            socket.sendTo(raw.data(), raw.size(), client);

            continue;
        }

        trial = 0;
        rxBuffer.resize(bytesRecieved);

        try {
            std::unique_ptr<Packet> pkt = PacketParser::parse(rxBuffer);
            DataPacket* dataPkt = dynamic_cast<DataPacket*>(pkt.get());

            if (dataPkt && dataPkt->getBlockNumber() == expectedBlock) {
                std::vector<uint8_t> data = dataPkt->getData();
                std::streampos before = file.tellp();
                file.write(reinterpret_cast<const char *>(data.data()), data.size());
                bytesWritten = file.tellp() - before;

                prevBlock = expectedBlock++;

                AckPacket reply{static_cast<uint16_t>(prevBlock)};
                std::vector<uint8_t> rawReply = reply.serialize();
                socket.sendTo(rawReply.data(), rawReply.size(), client);

                if (data.size() < 512) {
                    std::cout << "Transfer complete\n";
                    break;
                }

                continue;
            }

            AckPacket retry{static_cast<uint16_t>(prevBlock)};
            std::vector<uint8_t> rawRetry = retry.serialize();

            socket.sendTo(rawRetry.data(), rawRetry.size(), sender);
        } catch (const std::exception& err) {
            std::cerr << err.what() << '\n';
        }
    }

    socket.setRecieveTimeout(0);
}

void TftpServer::handlePacket(const Address& from, std::unique_ptr<Packet> packet) {
    if (packet->getOpcode() == Opcode::RRQ) {
        ReadRequestPacket* rrq = dynamic_cast<ReadRequestPacket*>(packet.get());

        if (rrq)
            handleRRQ(from, rrq);
    } else if (packet->getOpcode() == Opcode::WRQ) {
        WriteRequestPacket* wrq = dynamic_cast<WriteRequestPacket*>(packet.get());

        if (wrq)
            handleWRQ(from, wrq);
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
