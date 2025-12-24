#include "ErrorPacket.hpp"

ErrorPacket::ErrorPacket(uint16_t errorCode, const std::string& errorMessage) 
    : errorCode(errorCode), errorMessage(errorMessage) {}

Opcode ErrorPacket::getOpcode() const {
    return Opcode::ERROR;
}

uint16_t ErrorPacket::getErrorCode() const {
    return errorCode;
}

const std::string& ErrorPacket::getErrorMessage() const {
    return errorMessage;
}

std::vector<uint8_t> ErrorPacket::serialize() const {
    std::vector<uint8_t> buffer;

    uint16_t op = static_cast<uint16_t>(Opcode::ERROR);

    buffer.push_back(op >> 8);
    buffer.push_back(op & 0xff);
    buffer.push_back(errorCode >> 8);
    buffer.push_back(errorCode & 0xff);

    buffer.insert(std::end(buffer), std::begin(errorMessage), std::end(errorMessage));
    buffer.push_back(0x00);

    return buffer;
}
