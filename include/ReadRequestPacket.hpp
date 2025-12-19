#pragma once

#include "Packet.hpp"
#include <cstdint>
#include <vector>
#include <string>

class ReadRequestPacket : public Packet {
    std::string filename;
    std::string mode;

public:
    ReadRequestPacket(std::string filename, std::string mode = "octet");

    Opcode getOpcode() const override;
    std::vector<uint8_t> serialize() const override;
    std::string getFilename() const;
    std::string getMode() const;
};
