#include <cstdio>
#include <future>
#include "gtest/gtest.h"
#include <memory>

#include "mocks/mock_arduino.hpp"
#include "box_sonar.hpp"


using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Ne;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Throw;

#define TEST_PIN_TRIGGER 1
#define TEST_PIN_ECHO 2

struct SonarUnderTest : public box::Sonar {
    SonarUnderTest() : Sonar(TEST_PIN_TRIGGER,
                            TEST_PIN_ECHO) {}
    virtual ~SonarUnderTest() = default;
};

class TestSonar : public ::testing::Test {
  protected:
    SonarUnderTest* sonarUnderTest;
    void (*echo_isr_callback)();

    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        sonarUnderTest = new SonarUnderTest();
    }
    virtual void TearDown() {
        delete sonarUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestSonar, test_sonar_init) {EXPECT_TRUE(1); }

TEST_F(TestSonar, test_get_distance_cm) {
    unsigned int expected_result = 5;
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_TRIGGER, HIGH)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_TRIGGER, LOW)).Times(2);
    EXPECT_CALL(*arduinoMock, delayMicroseconds(10)).Times(1);
    EXPECT_CALL(*arduinoMock, delayMicroseconds(2)).Times(1);
    EXPECT_CALL(*arduinoMock, pulseIn(TEST_PIN_ECHO, HIGH, _))
                                        .Times(1)
                                        .WillOnce(Return((expected_result*2)/0.034));
    EXPECT_EQ(sonarUnderTest->get_distance_cm(), expected_result);
}
