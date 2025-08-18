// TaskQueue.h
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

class TaskQueue {
public:
    void push(int v) {
        {
            std::lock_guard<std::mutex> lk(m_);
            q_.push(std::move(v));
        }
        cv_.notify_one();
    }

    // 阻塞弹出；当停止时返回空
    int pop(bool& stopFlag) {
        std::unique_lock<std::mutex> lk(m_);
        if (stopFlag || q_.empty()) return -1;  // 空就直接返回
        int v = std::move(q_.front());
        q_.pop();
        return v;
    }


    void stop() {
        {
            std::lock_guard<std::mutex> lk(m_);
            stop_ = true;
        }
        cv_.notify_all();
    }

    bool stopped() const { return stop_; }

private:
    std::queue<int> q_;
    mutable std::mutex m_;
    std::condition_variable cv_;
    bool stop_ = false;
};
