cmake_minimum_required(VERSION 3.16)
project(hw-01)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(${PROJECT_NAME} main.cpp)
find_library(liblz4 REQUIRED)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/zstd/build/cmake)
target_link_libraries(${PROJECT_NAME}
        PUBLIC lz4)
#target_link_libraries(${PLOT_DRAWER} PUBLIC ${PYTHON_LIBRARIES} stdc++fs)
