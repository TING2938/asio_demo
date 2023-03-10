#include <cmath>
#include <iostream>

#include <asio.hpp>

#include "message.hpp"

using asio::ip::tcp;

class Message_Generator
{
public:
    const Message& get_next()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        x += dx;
        msg.value = std::sin(x);
        time(&msg.time);
        return msg;
    }

private:
    Message msg;
    double dx = 0.1;
    double x  = 0;
};

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    using pointer = std::shared_ptr<tcp_connection>;

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
        auto& msg = msg_gen.get_next();

        auto self = shared_from_this();
        asio::async_write(socket_, asio::buffer(&msg, sizeof(msg)),
                          [this, self](const std::error_code& error, size_t bytes) {
                              if (!error) {
                                  start();
                              }
                          });
    }

private:
    tcp_connection(asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket socket_;
    Message_Generator msg_gen;
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
        auto connection = tcp_connection::create(io_context_);
        acceptor_.async_accept(connection->socket(), [this, connection](const std::error_code& error) {
            if (!error) {
                connection->start();
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
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}