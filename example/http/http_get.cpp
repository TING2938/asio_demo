#include <wchar.h>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

std::vector<char> vBuffer(1 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), [&](std::error_code ec, std::size_t length) {
        if (!ec) {
            std::cout << "\n\nRead " << length << " bytes\n\n";
            for (int i = 0; i < length; i++) {
                std::cout << vBuffer[i];
            }
            GrabSomeData(socket);
        }
    });
}

int main()
{
    std::error_code ec;

    asio::io_context context;

    asio::io_context::work idleWork(context);

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("180.101.50.188", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec) {
        std::cout << "connected!" << std::endl;
    } else {
        std::cout << "Failed to connect to address: \n" << ec.message() << std::endl;
    }

    std::string sRequest = "GET / HTTP/1.1\r\n\r\n";

    std::array<char, 204800> vBuffer;
    socket.write_some(asio::buffer(sRequest), ec);

    auto len = socket.read_some(asio::buffer(vBuffer), ec);

    std::cout.write(vBuffer.data(), len);
}
