#pragma once

#include "Packet.hpp"
#include <cstdint>
#include <vector>

class DataPacket : public Packet {
    uint16_t blockNumber;
    std::vector<uint8_t> data;

public:
    DataPacket(uint16_t blockNumber, std::vector<uint8_t> data);

    Opcode getOpcode() const override;
    std::vector<uint8_t> serialize() const override;
    uint16_t getBlockNumber() const;
    std::vector<uint8_t> getData() const;
};