#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "AckPacket.hpp"
#include "DataPacket.hpp"

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

TEST(PacketTests, SerializeDataPacket) {
    DataPacket packet{1, {0x0a, 0x0b, 0x0c}};

    EXPECT_EQ(packet.getOpcode(), Opcode::DATA);

    std::vector<uint8_t> expected{0x00, 0x03, 0x00, 0x01, 0x0a, 0x0b, 0x0c};
    EXPECT_EQ(packet.serialize().size(), 7);
    EXPECT_EQ(packet.serialize(), expected);
}

TEST(PacketTests, DataPacketLargePayload) {
    std::vector<uint8_t> payload(513, 0x01);

    EXPECT_THROW({
        DataPacket packet(1, payload);
    }, std::runtime_error);
}
