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
        return std::shared_ptr<tcp_connection>(new tcp_connection(io_context));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();

        asio::async_write(socket_, asio::buffer(message_),
                          [t = shared_from_this()](const std::error_code& error, size_t bytes) {
                              t->flag++;
                              std::cout << "async_write, flag = " << t->flag << std::endl;
                          });
    }

    ~tcp_connection()
    {
        std::cout << "tcp_connection close" << std::endl;
    }

private:
    tcp_connection(asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket socket_;
    std::string message_;
    int flag = 0;
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

int main()
{
    try {
        asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}