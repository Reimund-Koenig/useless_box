#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "box_systemtest.hpp"
#include "mocks/mock_serial.hpp"
#include "mocks/mock_arduino.hpp"
#include "mocks/mock_servomotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Systemtest_under_test : public box::Systemtest {
    Systemtest_under_test() : Systemtest() {}
};

class TestSystemtest : public ::testing::Test {
  protected:
    Systemtest_under_test* systemtest_under_test;
    virtual void SetUp() {
        serial_mock = new NiceMock<SerialMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        servomotor_mock = new NiceMock<ServomotorMock>;
        systemtest_under_test = new Systemtest_under_test();
    }
    virtual void TearDown() {
        delete serial_mock;
        delete arduino_mock;
        delete servomotor_mock;
        delete systemtest_under_test;
    }
};

// Testing if the class is compiled/linked is enough for systemtest class
TEST_F(TestSystemtest, test_systemtest_init) { EXPECT_TRUE(true); }
