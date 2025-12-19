#pragma once

#include "Packet.hpp"
#include <cstdint>
#include <vector>
#include <memory>

class PacketParser {
    static std::unique_ptr<Packet> parse(const std::vector<uint8_t>& buffer);
};
