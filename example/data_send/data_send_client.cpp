#include <iostream>

#include <asio.hpp>

#include "message.hpp"

using asio::ip::tcp;

int main()
{
    try {
        asio::io_context io_context;

        std::error_code ec;

        tcp::endpoint endpoints(asio::ip::make_address("127.0.0.1", ec), 1300);

        tcp::socket socket(io_context);

        socket.connect(endpoints, ec);

        if (!ec) {
            std::cout << "connected!" << std::endl;
        } else {
            std::cout << "Failed to connect to address: \n" << ec.message() << std::endl;
        }

        for (;;) {
            Message msg;

            auto len = socket.read_some(asio::buffer(&msg, sizeof(msg)), ec);

            if (ec == asio::error::eof) {
                continue;
            } else if (ec) {
                throw asio::system_error(ec);
            }

            std::cout << msg.time << " " << msg.value << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}