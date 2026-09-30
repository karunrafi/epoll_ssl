# Remote Command Server

A TCP client/server in modern C++ where the client sends shell commands and the
server executes them and streams back the output. Built to explore the core
techniques behind high-performance Linux network servers.

## Features

- Event-driven server using `epoll` with non-blocking sockets
- Thread pool (CTPL) for executing commands without blocking the event loop
- Command execution via `popen`, capturing stdout and stderr
- Robust `send`/`recv` handling for partial reads and writes
- Structured logging with spdlog
- CMake-based build

## Design

One event-loop thread waits on `epoll` for ready sockets. Complete requests are
handed to a pool of worker threads, which run the command and return the result
for sending. This avoids the one-thread-per-client model and lets a small number
of threads serve many concurrent connections.

## Dependencies

| Library | Purpose | Notes |
|---|---|---|
| [CTPL](https://github.com/vit-vit/CTPL) | Thread pool | Header-only |
| [spdlog](https://github.com/gabime/spdlog) | Logging | |
| [GoogleTest](https://github.com/google/googletest) | Unit testing | Integrated, tests in progress |

Requirements: Linux, a C++17 compiler (GCC 9+ or Clang 10+), CMake 3.14+.

## Build

```bash
git clone https://github.com/karunrafi/epoll_ssl.git
cd epoll_ssl
mkdir build && cd build
cmake ..
cmake --build .
```

## Run

Start the server (listens on port 9000):

```bash
./tcpserver
```

In another terminal, start the client:

```bash
./tcpClient
```

## Project Structure

```
.
├── CMakeLists.txt
├── include/        # headers (tcpclient.h, tcpserver.h, ...)
├── src/            # implementation files
├── third_party/    # CTPL and other vendored libraries
└── tests/          # GoogleTest unit tests (in progress)
```

## Roadmap

- [ ] Unit tests with GoogleTest
- [ ] Length-prefixed message framing
- [ ] `EPOLLONESHOT` handling for safe multi-threaded reads
- [ ] Benchmarks: concurrent clients and latency under load
- [ ] HTTP server built on the same networking core

## Security Note

This is a learning project. It executes arbitrary commands with no
authentication or encryption. Do not expose it to untrusted networks.
