#pragma once

#include "channel.hpp"

#include <thread>

struct Actor {
    ~Actor()
    {
        this->worker.join();
    }

    auto task()
    {
        this->executed = true;
    }

    auto start() -> Sender<void>
    {
        auto [sender, receiver] = channel<void>();
        this->worker = std::thread(
        [this, rx {std::move(receiver)}] () mutable {
            rx.recv();
            this->task();
        });

        return std::move(sender);
    }

    bool executed = false;
    std::thread worker;
};
