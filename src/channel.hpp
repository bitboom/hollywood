#pragma once

#include <future>
#include <utility>

// TODO: Support multishot

template<typename T>
struct Sender;

template<typename T>
struct Receiver;

template<typename T>
auto channel() -> std::pair<Sender<T>, Receiver<T>>
{
    Sender<T> sender;
    Receiver<T> receiver = sender.pair();

    return std::pair(std::move(sender), std::move(receiver));
}

template<typename T>
struct Sender {
    auto send(T&& value)
    {
        this->tx.set_value(std::forward<T>(value));
    }

    auto pair() -> Receiver<T>
    {
        return Receiver(this->tx.get_future());
    }

private:
    std::promise<T> tx;
};

template<typename T>
struct Receiver {
    auto recv() -> T
    {
        this->rx.wait();
        return this->rx.get();
    }

private:
    friend struct Sender<T>;
    Receiver(std::future<T> rx) : rx(std::move(rx)) {}

    std::future<T> rx;
};

// Barrier
template<>
struct Receiver<void> {
    auto recv()
    {
        this->rx.wait();
    }

private:
    friend struct Sender<void>;
    Receiver(std::future<void> rx) : rx(std::move(rx)) {}

    std::future<void> rx;
};

template<>
struct Sender<void> {
    auto send()
    {
        this->tx.set_value();
    }

    auto pair() -> Receiver<void> 
    {
        return Receiver(this->tx.get_future());
    }

private:
    std::promise<void> tx;
};
