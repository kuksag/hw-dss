cmake_minimum_required(VERSION 3.16)
project(hw-02)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} zstd lz4 stdc++fs)
