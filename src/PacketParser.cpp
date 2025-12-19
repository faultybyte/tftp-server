#include "PacketParser.hpp"
#include "AckPacket.hpp"
#include <stdexcept>

static std::unique_ptr<Packet> parseAckPacket(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 4)
        throw std::runtime_error("Invalid ACK packet");

    uint16_t blockNumber = (buffer[2] << 8) | buffer[3];
    
    return std::make_unique<AckPacket>(blockNumber);
}

std::unique_ptr<Packet> PacketParser::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 2)
        throw std::runtime_error("Invalid packet size");

    uint16_t op = (buffer[0] << 8) | buffer[1];

    switch (op) {
        case 4:
            return parseAckPacket(buffer);
        default:
            throw std::runtime_error("Unknown opcode");
    }
}
