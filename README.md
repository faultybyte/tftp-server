# TFTP Server
![CI Status](https://github.com/faultybyte/tftp-server/actions/workflows/ci.yml/badge.svg)

A high performance TFTP server implementation from scratch in modern C++.

## Features
* RFC 1350 compliant
* Supports bidirectional transfers
* Custom Reliability Layer over UDP
* Supports concurrent file transfers through multithreading
* Built using Modern C++

## Building
The project requires CMake and a compiler that supports C++20 to build.

```bash
cmake --build build
```

The executable `tftpd` is built in the `build/` directory.

## Run the Server
```bash
./tftpd <port>
```

## Running the Tests
In the `build/` directory run:
```bash
ctest
```

## License
```
MIT License

Copyright (c) 2025-2026 Johan Emmanuel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
