cmake_minimum_required(VERSION 3.10)

project(Gear)

set(CMAKE_CXX_STANDARD 17)


# EXECUTABLE SECTION
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
add_executable(main main.cpp)

target_include_directories(main PRIVATE include)

target_sources(main PRIVATE src/Gear.cpp)

target_link_libraries(main)

add_executable(tests tests.cpp)

target_include_directories(tests PRIVATE include)

target_sources(tests PRIVATE src/Gear.cpp)

