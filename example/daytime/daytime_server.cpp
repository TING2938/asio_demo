//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using asio::ip::tcp;

std::string make_daytime_string(char c1, char c2)
{
    using namespace std;  // For time_t, time and ctime;
    time_t now = time(0);
    return std::string("hello world, ") + std::to_string(c1 + c2) + ctime(&now);
}

int main()
{
    try {
        asio::io_context io_context;

        std::error_code ec;

        tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(tcp::v4(), 1300));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::array<char, 128> vBuffer;
            socket.read_some(asio::buffer(vBuffer));

            std::string message = make_daytime_string(vBuffer[0], vBuffer[1]);

            asio::error_code ignored_error;
            asio::write(socket, asio::buffer(message), ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}