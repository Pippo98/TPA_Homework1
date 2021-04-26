cmake_minimum_required(VERSION 3.10)

project(Gear)

set(CMAKE_CXX_STANDARD 17)


include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external_libs
  )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(
  ${PROJECT_NAME} SHARED
  src/Gear.cpp
  external_libs/EB_Device.cpp
  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(tests scripts/tests.cpp)
target_link_libraries(tests ${PROJECT_NAME})

add_executable(cmd_line scripts/cmd_line.cpp)
target_link_libraries(cmd_line ${PROJECT_NAME})

add_executable(connection scripts/connection.cpp)
target_link_libraries(connection ${PROJECT_NAME})

add_executable(machine scripts/machine.cpp)
target_link_libraries(machine ${PROJECT_NAME})
