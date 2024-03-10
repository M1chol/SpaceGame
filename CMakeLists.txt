#[[ This cmake file is used for building binaries for Linux ]]

cmake_minimum_required(VERSION 3.28)

project(mygame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} "include/headers")

file(GLOB SOURCES "src/*.cpp")

add_executable(game ${SOURCES})
set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

target_link_libraries(game ${SDL2_LIBRARIES})
