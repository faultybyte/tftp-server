#pragma once

#include "Packet.hpp"
#include <cstdint>
#include <vector>

class AckPacket : public Packet {
    uint16_t blockNumber;

public:
    explicit AckPacket(uint16_t blockNumber);

    Opcode getOpcode() const override;
    std::vector<uint8_t> serialize() const override;
    uint16_t getBlockNumber() const;
};
