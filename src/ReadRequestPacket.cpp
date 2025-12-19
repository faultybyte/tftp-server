#include "ReadRequestPacket.hpp"

ReadRequestPacket::ReadRequestPacket(std::string filename, std::string mode)
    : filename(std::move(filename)), mode(std::move(mode)) {}

Opcode ReadRequestPacket::getOpcode() const {
    return Opcode::RRQ;
}

std::vector<uint8_t> ReadRequestPacket::serialize() const {
    std::vector<uint8_t> buffer;

    uint16_t op = static_cast<uint16_t>(Opcode::RRQ);

    buffer.push_back(op >> 8);
    buffer.push_back(op & 0xff);
    buffer.insert(std::end(buffer), std::begin(filename), std::end(filename));
    buffer.push_back(0x00);
    buffer.insert(std::end(buffer), std::begin(mode), std::end(mode));
    buffer.push_back(0x00);

    return buffer;
}

std::string ReadRequestPacket::getFilename() const {
    return filename;
}

std::string ReadRequestPacket::getMode() const {
    return mode;
}
