#pragma once

#include <vector>
#include <cstdint>
#include <string>

enum class Opcode : uint16_t {
    RRQ = 1,
    WRQ = 2,
    DATA = 3,
    ACK = 4,
    ERROR = 5
};

class Packet {
public:
    virtual ~Packet() = default;

    virtual Opcode getOpcode() const = 0;
    virtual std::vector<uint8_t> serialize() const = 0;
};
