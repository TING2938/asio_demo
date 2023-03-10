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

using asio::ip::udp;

int main(int argc, char* argv[])
{
    try {
        asio::io_context io_context;

        udp::endpoint receiver_endpoint(asio::ip::make_address("127.0.0.1"), 1300);

        udp::socket socket(io_context);
        socket.open(udp::v4());

        std::array<char, 1> send_buf = {{0}};
        socket.send_to(asio::buffer(send_buf), receiver_endpoint);

        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
        std::cout << sender_endpoint << std::endl;

        std::cout.write(recv_buf.data(), len);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}