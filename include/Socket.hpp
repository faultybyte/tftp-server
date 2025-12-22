#pragma once

#include <cstdint>
#include <sys/types.h>

#include "Address.hpp"

class UDPSocket {
    int sockfd;

public:
    explicit UDPSocket(uint16_t port);
    UDPSocket(const UDPSocket&) = delete;
    UDPSocket& operator=(const UDPSocket&) = delete;

    ssize_t receiveFrom(void* buffer, size_t len, Address& sender);
    ssize_t sendTo(const void* buffer, size_t len, const Address& target);
    void setRecieveTimeout(int seconds);

    ~UDPSocket();
};