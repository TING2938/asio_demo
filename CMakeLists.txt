cmake_minimum_required(VERSION 3.0.0)
project(asio_demo VERSION 0.1.0)

add_subdirectory(third_party)

add_executable(asio_demo main.cpp)
target_link_libraries(asio_demo asio)

add_subdirectory(example)