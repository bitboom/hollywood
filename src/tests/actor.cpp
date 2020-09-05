#include <gtest/gtest.h>

#include "../actor.hpp"

#include <chrono>
#include <thread>

TEST(actor, oneshot)
{
    Actor actor;
    auto actor_ref = actor.start();
    actor_ref.send();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    EXPECT_TRUE(actor.executed);
}
