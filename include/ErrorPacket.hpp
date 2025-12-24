#pragma once

#include "Packet.hpp"
#include <string>
#include <vector>

class ErrorPacket : public Packet {
    uint16_t errorCode;
    std::string errorMessage;

public:
    ErrorPacket(uint16_t errorCode, const std::string& errorMessage);

    Opcode getOpcode() const override;
    uint16_t getErrorCode() const;
    const std::string& getErrorMessage() const;
    std::vector<uint8_t> serialize() const override;
};
