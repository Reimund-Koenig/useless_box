#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "box_mode_wakeup.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeWakeup_under_test : public box::ModeWakeup {
    ModeWakeup_under_test() : ModeWakeup() {}
};

class TestModeWakeup : public ::testing::Test {
  protected:
    ModeWakeup_under_test* mode_wakeup_under_test;
    virtual void SetUp() {
        mode_wakeup_under_test = new ModeWakeup_under_test();
    }
    virtual void TearDown() {
        delete mode_wakeup_under_test;
    }
};

TEST_F(TestModeWakeup, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeWakeup, test_sleep) {
    EXPECT_TRUE(mode_wakeup_under_test->run(79));
    EXPECT_FALSE(mode_wakeup_under_test->run(80));
}