#pragma once

#include "Socket.hpp"
#include "Address.hpp"
#include "Packet.hpp"
#include "ReadRequestPacket.hpp"
#include "WriteRequestPacket.hpp"
#include <cstdint>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>

class TftpServer {
    UDPSocket socket;
    uint16_t port;
    std::atomic<bool> running;
    std::mutex consoleMutex;

    void handleRRQ(const Address& client, const ReadRequestPacket* packet);
    void handleWRQ(const Address& client, const WriteRequestPacket* packet);
    void handlePacket(const Address& from, std::unique_ptr<Packet> packet);

public:
    explicit TftpServer(uint16_t port);
    void start();
    void stop();
};
