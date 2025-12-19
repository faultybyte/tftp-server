#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include "AckPacket.hpp"

TEST(PacketTests, SerializeAckPacket) {
    AckPacket packet{1};

    EXPECT_EQ(packet.getOpcode(), Opcode::ACK);

    std::vector<uint8_t> expected{0x00, 0x04, 0x00, 0x01};
    EXPECT_EQ(packet.serialize().size(), 4);
    EXPECT_EQ(packet.serialize(), expected);
}

TEST(PacketTests, SerializeAckPacketLargeBlock) {
    AckPacket packet{256};

    std::vector<uint8_t> expected{0x00, 0x04, 0x01, 0x00};
    EXPECT_EQ(packet.serialize(), expected);
}
