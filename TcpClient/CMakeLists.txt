cmake_minimum_required(VERSION 3.31.3 FATAL_ERROR)
project(simplecalculator VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SOURCES
src/tcpclient.cpp
mainTcpClient.cpp
)


add_executable(tcpClient ${SOURCES})

target_link_libraries(tcpClient PRIVATE spdlog::spdlog)

target_include_directories(tcpClient PUBLIC inc)

