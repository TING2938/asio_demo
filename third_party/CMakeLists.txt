
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(asio INTERFACE)
target_link_libraries(asio INTERFACE Threads::Threads)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
if(WIN32)
   target_compile_definitions(asio INTERFACE _WIN32_WINNT=0x0601 _CRT_SECURE_NO_WARNINGS) 
endif()

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