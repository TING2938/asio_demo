#include <chrono>
#include <iostream>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

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

    std::thread thrContext([&]() { context.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("180.101.50.188", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec) {
        std::cout << "connected!" << std::endl;
    } else {
        std::cout << "Failed to connect to address: \n" << ec.message() << std::endl;
    }

    if (socket.is_open()) {
        GrabSomeData(socket);

        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: www.baidu.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        std::this_thread::sleep_for(10000ms);

        context.stop();
        if (thrContext.joinable())
            thrContext.join();
    }
}
