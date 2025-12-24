#include "PacketParser.hpp"
#include "AckPacket.hpp"
#include "DataPacket.hpp"
#include "ReadRequestPacket.hpp"
#include "ErrorPacket.hpp"
#include <algorithm>
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

static std::unique_ptr<Packet> parseReadRequestPacket(const std::vector<uint8_t>& buffer) {
    auto filenameEnd = std::find(buffer.begin() + 2, buffer.end(), 0x00);
    if (filenameEnd == buffer.end())
        throw std::runtime_error("Invalid RRQ packet");

    std::string filename(buffer.begin() + 2, filenameEnd);

    auto modeEnd = std::find(filenameEnd + 1, buffer.end(), 0x00);
    if (modeEnd == buffer.end())
        throw std::runtime_error("Invalid RRQ packet");

    std::string mode(filenameEnd + 1, modeEnd);

    return std::make_unique<ReadRequestPacket>(filename, mode);
}

static std::unique_ptr<Packet> parseErrorPacket(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 5)
        throw std::runtime_error("Invalid ERROR packet size");

    uint16_t errorCode = (buffer[2] << 8) | buffer[3];
    auto msgEnd = std::find(buffer.begin() + 4, buffer.end(), 0x00);
    if (msgEnd == buffer.end())
        throw std::runtime_error("Invalid ERROR packet");
        
    std::string errorMessage(buffer.begin() + 4, msgEnd);

    return std::make_unique<ErrorPacket>(errorCode, errorMessage);
}

std::unique_ptr<Packet> PacketParser::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 2)
        throw std::runtime_error("Invalid packet size");

    uint16_t op = (buffer[0] << 8) | buffer[1];

    switch (op) {
        case 1:
            return parseReadRequestPacket(buffer);
        case 3:
            return parseDataPacket(buffer);
        case 4:
            return parseAckPacket(buffer);
        case 5:
            return parseErrorPacket(buffer);
        default:
            throw std::runtime_error("Unknown opcode");
    }
}
