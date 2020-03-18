#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_servomotor.hpp"
#include "box_systemtest.hpp"

using ::testing::NiceMock;

struct SystemtestUnderTest : public box::Systemtest {
    SystemtestUnderTest() : Systemtest() {}
};

class TestSystemtest : public ::testing::Test {
  protected:
    SystemtestUnderTest* systemtestUnderTest;
    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        servomotor_mock = new NiceMock<ServomotorMock>;
        systemtestUnderTest = new SystemtestUnderTest();
    }
    virtual void TearDown() {
        delete servomotor_mock;
        delete arduinoMock;
        delete systemtestUnderTest;
    }
};

TEST_F(TestSystemtest, test_systemtest_init) { EXPECT_TRUE(true); }
