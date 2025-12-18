#include <gtest/gtest.h>
#include "Address.hpp"
#include "Socket.hpp"

TEST(SocketTests, CanBindSocket) {
    EXPECT_NO_THROW({
        UDPSocket socket{0};
    });
}

TEST(SocketTests, SendAndRecieveLoopback) {
    std::string ip{"127.0.0.1"};
    uint16_t port = 9090;
    std::string msg{"Hello There!"};

    UDPSocket server{port};
    UDPSocket client{0};

    Address serverAddr{ip, port};

    ssize_t size = client.sendTo(msg.c_str(), msg.length(), serverAddr);
    EXPECT_EQ(size, msg.length());

    char buffer[256];
    Address sender;

    ssize_t recieved = server.receiveFrom(buffer, sizeof(buffer), sender);
    EXPECT_EQ(recieved, msg.length());
    EXPECT_EQ(std::string(buffer, recieved), msg);

    EXPECT_EQ(sender.getIP(), "127.0.0.1");
}