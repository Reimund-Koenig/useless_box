#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_box.hpp"
#include "box_mode_normal.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeNormal_under_test : public box::ModeNormal {
    ModeNormal_under_test(box::Servomanager* box_servomanager_mock, box::Wait* box_wait_mock) :
    ModeNormal(box_servomanager_mock, box_wait_mock) {}
};

class TestModeNormal : public ::testing::Test {
  protected:
    ModeNormal_under_test* mode_normal_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        mode_normal_under_test = new ModeNormal_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete mode_normal_under_test;
    }
};

TEST_F(TestModeNormal, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeNormal, test_sleep) {
    EXPECT_CALL(*box_wait_mock, milliseconds(50)).Times(1);
    mode_normal_under_test->run();
}

// TODO: TEST START SPEED = 4