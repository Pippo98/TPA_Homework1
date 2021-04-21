cmake_minimum_required(VERSION 3.10)

project(Gear)

set(CMAKE_CXX_STANDARD 17)


# EXECUTABLE SECTION
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

add_executable(cmd_line cmd_line.cpp)

target_include_directories(cmd_line PRIVATE include)

target_sources(cmd_line PRIVATE src/Gear.cpp)

target_link_libraries(cmd_line)


add_executable(tests tests.cpp)

target_include_directories(tests PRIVATE include)

target_sources(tests PRIVATE src/Gear.cpp)



add_executable(connection connection.cpp)

target_include_directories(connection PRIVATE include)

target_sources(connection PRIVATE src/Gear.cpp)
