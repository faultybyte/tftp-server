#include "DataPacket.hpp"
#include <stdexcept>

DataPacket::DataPacket(uint16_t blockNumber, std::vector<uint8_t> data) 
    : blockNumber(blockNumber), data(std::move(data)) {
    if (this->data.size() > 512)
        throw std::runtime_error("Data size greater than 512 bytes");
}

Opcode DataPacket::getOpcode() const {
    return Opcode::DATA;
}

std::vector<uint8_t> DataPacket::serialize() const {
    std::vector<uint8_t> buffer;

    uint16_t op = static_cast<uint16_t>(Opcode::DATA);

    buffer.push_back(op >> 8);
    buffer.push_back(op & 0xff);
    buffer.push_back(blockNumber >> 8);
    buffer.push_back(blockNumber & 0xff);

    buffer.insert(std::end(buffer), std::begin(data), std::end(data));

    return buffer;
}

uint16_t DataPacket::getBlockNumber() const {
    return blockNumber;
}

std::vector<uint8_t> DataPacket::getData() const {
    return data;
}
