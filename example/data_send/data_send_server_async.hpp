#pragma once

#include <cmath>

#include <asio.hpp>

#include "message.hpp"

using asio::ip::tcp;

template <typename Message>
class tcp_connection : public std::enable_shared_from_this<tcp_connection<Message>>
{
public:
    using pointer = std::shared_ptr<tcp_connection>;

    static pointer create(asio::io_context& io_context, Message_Queue<Message>& msg_queue)
    {
        return pointer(new tcp_connection(io_context, msg_queue));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        auto msg = msg_queue_.pop();

        auto self = this->shared_from_this();
        asio::async_write(socket_, asio::buffer(&msg, sizeof(msg)),
                          [this, self](const std::error_code& error, size_t bytes) {
                              if (!error) {
                                  start();
                              }
                          });
    }

private:
    tcp_connection(asio::io_context& io_context, Message_Queue<Message>& msg_queue)
        : socket_(io_context), msg_queue_(msg_queue)
    {
    }

    tcp::socket socket_;
    Message_Queue<Message>& msg_queue_;
};

template <typename Message>
class tcp_server
{
public:
    tcp_server(asio::io_context& io_context, Message_Queue<Message>& msg_queue)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 1300)), msg_queue_(msg_queue)
    {
        start_accept();
    }

private:
    void start_accept()
    {
        auto connection = tcp_connection<Message>::create(io_context_, msg_queue_);
        acceptor_.async_accept(connection->socket(), [this, connection](const std::error_code& error) {
            if (!error) {
                connection->start();
            }
            start_accept();
        });
    }

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    Message_Queue<Message>& msg_queue_;
};

template <typename Message>
class Server_Runner
{
public:
    Server_Runner(Message_Queue<Message>& msg_queue) : server(io_context, msg_queue) {}

    void run()
    {
        io_context.run();
    }

private:
    asio::io_context io_context;
    tcp_server<Message> server;
};
