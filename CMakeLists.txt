cmake_minimum_required(VERSION 3.23)

project(tftp-server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tftpd)

target_include_directories(tftpd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(tftpd PRIVATE 
    main.cpp
)

