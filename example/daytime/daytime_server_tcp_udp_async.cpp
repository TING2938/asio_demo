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

using asio::ip::tcp;
using asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std;  // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();

        asio::async_write(socket_, asio::buffer(message_),
                          [t = shared_from_this()](const std::error_code& error, size_t bytes) {});
    }

private:
    tcp_connection(asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    tcp_server(asio::io_context& io_context)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 1300))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(), [=](const std::error_code& error) {
            if (!error) {
                new_connection->start();
            }

            start_accept();
        });
    }

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

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
        tcp_server server1(io_context);
        udp_server server2(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}