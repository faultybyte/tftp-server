#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "AckPacket.hpp"
#include "DataPacket.hpp"
#include "ReadRequestPacket.hpp"
#include "WriteRequestPacket.hpp"
#include "ErrorPacket.hpp"
#include "PacketParser.hpp"

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

TEST(PacketTests, SerializeRRQPacket) {
    ReadRequestPacket packet{"test.txt", "octet"};

    EXPECT_EQ(packet.getOpcode(), Opcode::RRQ);

    std::vector<uint8_t> expected{
        0x00, 0x01, 
        't', 'e', 's', 't', '.', 't', 'x', 't', 
        0x00, 
        'o', 'c', 't', 'e', 't', 
        0x00
    };

    EXPECT_EQ(packet.serialize().size(), 17);
    EXPECT_EQ(packet.serialize(), expected);
}

TEST(PacketTests, SerializeWRQPacket) {
    WriteRequestPacket packet{"test.txt", "octet"};

    EXPECT_EQ(packet.getOpcode(), Opcode::WRQ);

    std::vector<uint8_t> expected{
        0x00, 0x02, 
        't', 'e', 's', 't', '.', 't', 'x', 't', 
        0x00, 
        'o', 'c', 't', 'e', 't', 
        0x00
    };

    EXPECT_EQ(packet.serialize().size(), 17);
    EXPECT_EQ(packet.serialize(), expected);
}

TEST(PacketTests, SerializeErrorPacket) {
    ErrorPacket packet{1, "File not found"};

    EXPECT_EQ(packet.getOpcode(), Opcode::ERROR);

    std::vector<uint8_t> expected{
        0x00, 0x05, 
        0x00, 0x01, 
        'F', 'i', 'l', 'e', ' ', 'n', 'o', 't', ' ', 'f', 'o', 'u', 'n', 'd', 
        0x00
    };

    EXPECT_EQ(packet.serialize().size(), 19);
    EXPECT_EQ(packet.serialize(), expected);
}

TEST(PacketTests, ParseRRQ) {
    std::vector<uint8_t> raw{
        0x00, 0x01,
        't', 'e', 's', 't', '.', 't', 'x', 't', 
        0x00,
        'o', 'c', 't', 'e', 't', 
        0x00
    };

    std::unique_ptr<Packet> packet = PacketParser::parse(raw);

    ReadRequestPacket* rrq = dynamic_cast<ReadRequestPacket*>(packet.get());

    EXPECT_NE(rrq, nullptr);
    EXPECT_EQ(rrq->getOpcode(), Opcode::RRQ);
    EXPECT_EQ(rrq->getFilename(), "test.txt");
    EXPECT_EQ(rrq->getMode(), "octet");
    EXPECT_EQ(rrq->serialize(), raw);
}

TEST(PacketTests, ParseWRQ) {
    std::vector<uint8_t> raw{
        0x00, 0x02,
        't', 'e', 's', 't', '.', 't', 'x', 't', 
        0x00,
        'o', 'c', 't', 'e', 't', 
        0x00
    };

    std::unique_ptr<Packet> packet = PacketParser::parse(raw);

    WriteRequestPacket* wrq = dynamic_cast<WriteRequestPacket*>(packet.get());

    EXPECT_NE(wrq, nullptr);
    EXPECT_EQ(wrq->getOpcode(), Opcode::WRQ);
    EXPECT_EQ(wrq->getFilename(), "test.txt");
    EXPECT_EQ(wrq->getMode(), "octet");
    EXPECT_EQ(wrq->serialize(), raw);
}

TEST(PacketTests, ParseData) {
    std::vector<uint8_t> raw{
        0x00, 0x03, 
        0x00, 0x01, 
        0xaa, 0xbb
    };

    std::unique_ptr<Packet> packet = PacketParser::parse(raw);

    DataPacket* data = dynamic_cast<DataPacket*>(packet.get());

    EXPECT_NE(data, nullptr);
    EXPECT_EQ(data->getOpcode(), Opcode::DATA);
    EXPECT_EQ(data->getBlockNumber(), 1);
    EXPECT_EQ(data->getData(), std::vector<uint8_t>({0xaa, 0xbb}));
    EXPECT_EQ(data->serialize(), raw);
}

TEST(PacketTests, ParseAck) {
    std::vector<uint8_t> raw{
        0x00, 0x04, 
        0x00, 0x01
    };

    std::unique_ptr<Packet> packet = PacketParser::parse(raw);

    AckPacket* ack = dynamic_cast<AckPacket*>(packet.get());

    EXPECT_NE(ack, nullptr);
    EXPECT_EQ(ack->getOpcode(), Opcode::ACK);
    EXPECT_EQ(ack->getBlockNumber(), 1);
    EXPECT_EQ(ack->serialize(), raw);
}

TEST(PacketTests, ParseError) {
    std::vector<uint8_t> raw{
        0x00, 0x05, 
        0x00, 0x01, 
        'F', 'i', 'l', 'e', ' ', 'n', 'o', 't', ' ', 'f', 'o', 'u', 'n', 'd', 
        0x00
    };

    std::unique_ptr<Packet> packet = PacketParser::parse(raw);

    ErrorPacket* err = dynamic_cast<ErrorPacket*>(packet.get());

    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->getOpcode(), Opcode::ERROR);
    EXPECT_EQ(err->getErrorCode(), 1);
    EXPECT_EQ(err->getErrorMessage(), "File not found");
    EXPECT_EQ(err->serialize(), raw);
}

TEST(PacketTests, ParseInvalidOpcode) {
    std::vector<uint8_t> raw{0x00, 0x06, 0x00, 0x01};

    EXPECT_THROW({
        PacketParser::parse(raw);
    }, std::runtime_error);
}
