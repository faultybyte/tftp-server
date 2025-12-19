#include "PacketParser.hpp"
#include "AckPacket.hpp"
#include "DataPacket.hpp"
#include <stdexcept>

static std::unique_ptr<Packet> parseAckPacket(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 4)
        throw std::runtime_error("Invalid ACK packet");

    uint16_t blockNumber = (buffer[2] << 8) | buffer[3];
    
    return std::make_unique<AckPacket>(blockNumber);
}

static std::unique_ptr<Packet> parseDataPacket(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 4)
        throw std::runtime_error("Invalid DATA packet");

    uint16_t blockNumber = (buffer[2] << 8) | buffer[3];
    std::vector<uint8_t> data(buffer.begin() + 4, buffer.end());
    
    return std::make_unique<DataPacket>(blockNumber, data);
}

std::unique_ptr<Packet> PacketParser::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 2)
        throw std::runtime_error("Invalid packet size");

    uint16_t op = (buffer[0] << 8) | buffer[1];

    switch (op) {
        case 3:
            return parseDataPacket(buffer);
        case 4:
            return parseAckPacket(buffer);
        default:
            throw std::runtime_error("Unknown opcode");
    }
}
