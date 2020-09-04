#pragma once

#include <future>
#include <utility>

template <typename T>
struct Sender;

template <typename T>
struct Receiver;

template <typename T>
auto channel() -> std::pair<Sender<T>, Receiver<T>>
{
    Sender<T> sender;
    Receiver<T> receiver = sender.pair();

    return std::pair(std::move(sender), std::move(receiver));
}

template <typename T>
struct Sender {
    auto send(T&& value) {
        this->tx.set_value(std::forward<T>(value));
        this->tx = std::promise<T>();
    }

    auto pair() -> Receiver<T> {
        return Receiver(this->tx.get_future());
    }

private:
    std::promise<T> tx;
};

template <typename T>
struct Receiver {
    auto recv() -> T {
        this->rx.wait();
        return this->rx.get();
    }

private:
    friend struct Sender<T>;
    Receiver(std::future<T> rx) : rx(std::move(rx)) {}

    std::future<T> rx;
};
