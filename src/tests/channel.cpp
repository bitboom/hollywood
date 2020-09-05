#include <gtest/gtest.h>

#include "../channel.hpp"

#include <thread>

TEST(channel, oneshot)
{
    auto [tx, rx] = channel<int>();
    tx.send(10);
    EXPECT_EQ(rx.recv(), 10);
}

TEST(channel, oneshot_thread)
{
    auto [tx, rx] = channel<int>();
    std::thread task([rx {std::move(rx)}] () mutable {
        EXPECT_EQ(rx.recv(), 100);
    });

    tx.send(100);
    task.join();
}

TEST(channel, oneshot_future)
{
    auto [tx, rx] = channel<std::future<int>>();
    std::promise<int> prom;
    auto fut = prom.get_future();

    tx.send(std::move(fut));
    auto reFut = rx.recv();

    prom.set_value(100);

    reFut.wait();
    EXPECT_EQ(reFut.get(), 100);
}
