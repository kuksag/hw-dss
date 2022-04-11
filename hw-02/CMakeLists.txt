cmake_minimum_required(VERSION 3.16)
project(hw-02 CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
SET(PLOT_DRAWER "${PROJECT_NAME}_plot_drawer")
add_executable(${PLOT_DRAWER} diagram_drawer.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} zstd lz4 stdc++fs)
target_link_libraries(${PLOT_DRAWER} PUBLIC ${PYTHON_LIBRARIES} stdc++fs)