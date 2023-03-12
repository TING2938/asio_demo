#pragma once

#include <condition_variable>
#include <ctime>
#include <deque>
#include <mutex>
#include <queue>

struct Message
{
    std::time_t time;
    double value;
};


template <typename Message>
class Message_Queue
{
private:
    std::deque<Message> q_;
    std::mutex mtx_;
    std::condition_variable cdv_;
    int max_size_;

public:
    Message_Queue(int capa) : max_size_(capa) {}

    void push(const Message& data)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        while (q_.size() >= max_size_) {
            q_.pop_front();
        }
        q_.push_back(data);
        cdv_.notify_one();
    }

    Message pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        while (q_.empty()) {
            cdv_.wait(lock);
        }
        auto ret = std::move(q_.front());
        q_.pop_front();
        return ret;
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        return q_.size();
    }
};