cmake_minimum_required(VERSION 3.21)
project(snakeproject)

add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_20)
set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED graphics network audio window system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(snakeproject main.cpp)

target_link_libraries(snakeproject PUBLIC default sfml-graphics sfml-window sfml-system)