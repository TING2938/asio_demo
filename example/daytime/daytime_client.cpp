//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main(int argc, char* argv[])
{
    try {
        asio::io_context io_context;

        std::error_code ec;

        asio::ip::tcp::endpoint endpoints(asio::ip::make_address("3.2.12.23", ec), 1300);

        tcp::socket socket(io_context);

        socket.connect(endpoints);

        for (;;) {
            std::array<char, 128> buf;
            asio::error_code error;

            buf[0] = 1;
            buf[1] = 2;
            // socket.write_some(asio::buffer(buf.data(), 2));

            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof)
                break;  // Connection closed cleanly by peer.
            else if (error)
                throw asio::system_error(error);  // Some other error.

            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}