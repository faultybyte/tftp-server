#include "WriteRequestPacket.hpp"

WriteRequestPacket::WriteRequestPacket(std::string filename, std::string mode)
    : filename(std::move(filename)), mode(std::move(mode)) {}

Opcode WriteRequestPacket::getOpcode() const {
    return Opcode::WRQ;
}

std::vector<uint8_t> WriteRequestPacket::serialize() const {
    std::vector<uint8_t> buffer;

    uint16_t op = static_cast<uint16_t>(Opcode::WRQ);

    buffer.push_back(op >> 8);
    buffer.push_back(op & 0xff);
    buffer.insert(std::end(buffer), std::begin(filename), std::end(filename));
    buffer.push_back(0x00);
    buffer.insert(std::end(buffer), std::begin(mode), std::end(mode));
    buffer.push_back(0x00);

    return buffer;
}

std::string WriteRequestPacket::getFilename() const {
    return filename;
}

std::string WriteRequestPacket::getMode() const {
    return mode;
}
