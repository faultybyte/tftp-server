#include "Socket.hpp"
#include <cstring>
#include <cerrno>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>

UDPSocket::UDPSocket(uint16_t port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        throw std::runtime_error("Failed to create socket");

    Address addr(port);
    if (bind(sockfd, addr.getRaw(), addr.getLength()) == -1) {
        close(sockfd);
        throw std::runtime_error(std::string("Socket creation failed: ") + strerror(errno));
    }
}

UDPSocket::~UDPSocket() {
    if (sockfd >= 0)
        close(sockfd);
}

ssize_t UDPSocket::sendTo(const void* buffer, size_t len, const Address& target) {
    return sendto(sockfd, buffer, len, 0, target.getRaw(), target.getLength());
}

ssize_t UDPSocket::receiveFrom(void* buffer, size_t len, Address& sender) {
    struct sockaddr_in rawAddr;
    socklen_t addrlen = sizeof(rawAddr);

    ssize_t size = recvfrom(sockfd, buffer, len, 0, 
            reinterpret_cast<struct sockaddr*>(&rawAddr), &addrlen);
    if (size >= 0)
        sender = Address(rawAddr);

    return size;
}