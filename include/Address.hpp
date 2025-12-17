#pragma once

#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>

class Address {
    struct sockaddr_in address;

public:
    Address();
    explicit Address(const std::string& ip, uint16_t port);
    explicit Address(uint16_t port);
    explicit Address(const struct sockaddr_in& address);    
    ~Address();

    socklen_t getLength() const;
    std::string getIP() const;
    uint16_t getPort() const;
    const sockaddr* getRaw() const;
};
