cmake_minimum_required(VERSION 3.31.3 FATAL_ERROR)
project(simplecalculator VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
src/tcpserver.cpp
mainTcpServer.cpp
)

add_executable(tcpServer ${SOURCES})

target_link_libraries(tcpServer PRIVATE spdlog::spdlog)

target_link_libraries(tcpServer PRIVATE ctpl::ctpl)

target_include_directories(tcpServer PRIVATE inc)

