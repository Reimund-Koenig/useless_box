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
#define TEST_PIN_INTERRUPT 3
#define TEST_MAX_DISTANCE 100

struct SonarUnderTest : public box::Sonar {
    SonarUnderTest() : Sonar(TEST_PIN_TRIGGER,
                            TEST_PIN_ECHO,
                            TEST_PIN_INTERRUPT,
                            TEST_MAX_DISTANCE) {}
    virtual ~SonarUnderTest() = default;
};

class TestSonar : public ::testing::Test {
  protected:
    SonarUnderTest* sonarUnderTest;
    void (*dummy_function) () = NULL;
    void (*attachInterrupt_call)(int, void (*)(), int);

    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        EXPECT_CALL(*arduinoMock, attachInterrupt(TEST_PIN_INTERRUPT,_,CHANGE))
            .Times(AtLeast(1))
            .WillOnce(SaveArg<1>(&attachInterrupt_call));
        sonarUnderTest = new SonarUnderTest();
    }
    virtual void TearDown() {
        delete sonarUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestSonar, test_sonar_init) {EXPECT_TRUE(1); }

TEST_F(TestSonar, test_sonar_instance) {
    EXPECT_TRUE(nullptr != sonarUnderTest);
}

TEST_F(TestSonar, test_defaults) {
    EXPECT_FALSE(sonarUnderTest->isFinished());
    EXPECT_EQ(0, sonarUnderTest->getRange());
}

TEST_F(TestSonar, test_start_function) {
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_TRIGGER, HIGH)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_TRIGGER, LOW)).Times(1);
    EXPECT_CALL(*arduinoMock, delayMicroseconds(10)).Times(1);
    sonarUnderTest->start();
    EXPECT_FALSE(sonarUnderTest->isFinished());
}

TEST_F(TestSonar, test_interrupt_start) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_ECHO)).Times(1).WillOnce(Return(HIGH));
    attachInterrupt_call(TEST_PIN_INTERRUPT,dummy_function,0);
    EXPECT_FALSE(sonarUnderTest->isFinished());
}

TEST_F(TestSonar, test_interrupt_end) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_ECHO)).Times(1).WillOnce(Return(LOW));
    attachInterrupt_call(TEST_PIN_INTERRUPT,dummy_function,0);
    EXPECT_TRUE(sonarUnderTest->isFinished());
}
