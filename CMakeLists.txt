cmake_minimum_required(VERSION 3.10)

project(Gear)

set(CMAKE_CXX_STANDARD 17)


# EXECUTABLE SECTION
add_executable(main main.cpp)

target_include_directories(main PRIVATE include)

target_sources(main PRIVATE src/Gear.cpp)

target_link_libraries(main)
