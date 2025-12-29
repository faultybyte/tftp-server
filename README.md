# TFTP Server

A TFTP server implementation in modern C++.

## Features
* RFC 1350 compliant
* Supports bidirectional transfers
* Custom Reliability Layer over UDP
* Modern C++ features

## Building
The project requires CMake to build.

```bash
cmake --build build
```

The executable `tftpd` is built in the `build/` directory.

## Run the Server
```bash
./tftpd <port>
```