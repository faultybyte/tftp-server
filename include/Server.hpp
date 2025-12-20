#pragma once

#include "Socket.hpp"
#include "Address.hpp"
#include "Packet.hpp"
#include <cstdint>
#include <memory>

class TftpServer {
    UDPSocket socket;
    uint16_t port;

    void handlePacket(const Address& from, std::unique_ptr<Packet> packet);

public:
    explicit TftpServer(uint16_t port);
    void start();
};
