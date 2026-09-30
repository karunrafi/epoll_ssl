cmake_minimum_required(VERSION 3.31.3 FATAL_ERROR)
project(simplecalculator VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.0.tar.gz)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(Gtest 
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0)
FetchContent_MakeAvailable(Gtest)


FetchContent_Declare(ctpl
  GIT_REPOSITORY https://github.com/vit-vit/CTPL.git
  GIT_TAG        ctpl_v.0.0.2)
FetchContent_MakeAvailable(ctpl)

find_package(Threads REQUIRED)

add_library(ctpl INTERFACE)
target_include_directories(ctpl INTERFACE ${ctpl_SOURCE_DIR})
target_link_libraries(ctpl INTERFACE Threads::Threads)
add_library(ctpl::ctpl ALIAS ctpl)


add_subdirectory(TcpClient)
add_subdirectory(Tcpserver)
