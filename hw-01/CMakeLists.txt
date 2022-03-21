cmake_minimum_required(VERSION 3.16)
project(hw-01)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(dependencies/smhasher/src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
        PUBLIC SMHasherSupport)
