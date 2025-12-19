cmake_minimum_required(VERSION 3.23)

project(tftp-server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
  DOWNLOAD_EXTRACT_TIMESTAMP 1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(tftp-core STATIC
    src/Address.cpp
    src/Socket.cpp
    src/AckPacket.cpp
    src/DataPacket.cpp
    src/ReadRequestPacket.cpp
    src/PacketParser.cpp
)
target_include_directories(tftp-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tftpd main.cpp)
target_link_libraries(tftpd PRIVATE tftp-core)

enable_testing()

add_executable(unit_tests tests/test_socket.cpp tests/test_packets.cpp)
target_link_libraries(unit_tests PRIVATE 
    tftp-core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
