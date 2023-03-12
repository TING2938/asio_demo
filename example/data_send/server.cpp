#include "data_send_server_async.hpp"

int main()
{
    Message_Queue<Message> msg_queue(10);
    Server_Runner<Message> runner(msg_queue);
    std::thread([&]() { runner.run(); }).detach();

    // msg gen
    Message msg;

    double x  = 0;
    double dx = 0.1;
    while (true) {
        time(&msg.time);
        msg.value = std::sin(x);
        msg_queue.push(msg);
        x += dx;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}