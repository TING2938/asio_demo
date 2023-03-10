//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std;  // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class udp_server
{
public:
    udp_server(asio::io_context& io_context) : socket_(io_context, udp::endpoint(udp::v4(), 1300))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                                   [&](const std::error_code& error, size_t bytes) {
                                       if (!error) {
                                           std::shared_ptr<std::string> message(new std::string(make_daytime_string()));

                                           socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                                                                 [](const std::error_code& error, size_t bytes) {});

                                           start_receive();
                                       }
                                   });
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
};

int main()
{
    try {
        asio::io_context io_context;
        udp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}