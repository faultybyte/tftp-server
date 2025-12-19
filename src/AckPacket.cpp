#include "AckPacket.hpp"

AckPacket::AckPacket(uint16_t blockNumber) : blockNumber(blockNumber) {}

Opcode AckPacket::getOpcode() const {
    return Opcode::ACK;
}

std::vector<uint8_t> AckPacket::serialize() const {
    std::vector<uint8_t> buffer;

    uint16_t op = static_cast<uint16_t>(Opcode::ACK);
    
    buffer.push_back(op >> 8);
    buffer.push_back(op & 0xff);
    buffer.push_back(blockNumber >> 8);
    buffer.push_back(blockNumber & 0xff);

    return buffer;
}

uint16_t AckPacket::getBlockNumber() const {
    return blockNumber;
}