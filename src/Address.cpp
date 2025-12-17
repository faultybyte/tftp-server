#include "Address.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <stdexcept>

Address::Address() {
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
}

Address::Address(const std::string& ip, uint16_t port) : Address() {
    address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) != 1)
        throw std::runtime_error("Invalid IP");
}

Address::Address(uint16_t port) : Address() {
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
}

Address::Address(const struct sockaddr_in& address) : address(address) {}

std::string Address::getIP() const {
    char buffer[INET_ADDRSTRLEN];

    if (!inet_ntop(AF_INET, &address.sin_addr, buffer, INET_ADDRSTRLEN))
        throw std::runtime_error("Failed to convert address to string");

    return std::string(buffer);
}

uint16_t Address::getPort() const {
    return ntohs(address.sin_port);
}

socklen_t Address::getLength() const {
    return sizeof(address);
}

const sockaddr* Address::getRaw() const {
    return reinterpret_cast<const sockaddr*>(&address);
}
