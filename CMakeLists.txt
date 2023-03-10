cmake_minimum_required(VERSION 3.0.0)
project(asio_demo VERSION 0.1.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE asio-1.24.0/include)
target_link_libraries(asio INTERFACE Threads::Threads)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

include(FetchContent)

FetchContent_Declare(
    asio
    URL ${CMAKE_SOURCE_DIR}/third_party/asio-1.24.0.zip
)
FetchContent_GetProperties(asio)
if (NOT asio_POPULATED)
    FetchContent_Populate(asio)
    target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/include)
endif()

add_executable(asio_demo main.cpp)
target_link_libraries(asio_demo asio)

add_subdirectory(example)