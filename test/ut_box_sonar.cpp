#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "peripheral/box_sonar.hpp"


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

struct Sonar_under_test : public box::Sonar {
    Sonar_under_test() : Sonar(TEST_PIN_TRIGGER,
                            TEST_PIN_ECHO) {}
    virtual ~Sonar_under_test() = default;
};

class TestSonar : public ::testing::Test {
  protected:
    Sonar_under_test* sonar_under_test;
    void (*echo_isr_callback)();

    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        sonar_under_test = new Sonar_under_test();
    }
    virtual void TearDown() {
        delete sonar_under_test;
        delete arduino_mock;
    }
};

TEST_F(TestSonar, test_sonar_init) { EXPECT_TRUE(1); }

TEST_F(TestSonar, test_get_distance_cm) {
    unsigned int expected_result = 5;
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_TRIGGER, HIGH)).Times(1);
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_TRIGGER, LOW)).Times(2);
    EXPECT_CALL(*arduino_mock, delayMicroseconds(10)).Times(1);
    EXPECT_CALL(*arduino_mock, delayMicroseconds(2)).Times(1);
    EXPECT_CALL(*arduino_mock, pulseIn(TEST_PIN_ECHO, HIGH, _))
                                        .Times(1)
                                        .WillOnce(Return((expected_result*2)/0.034));
    EXPECT_EQ(expected_result, sonar_under_test->get_distance_cm());
}

TEST_F(TestSonar, test_average) {
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((5*2)/0.034));
    int expected_distance = 5; // 5 = 5/1
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((7*2)/0.034));
    expected_distance = 6; // 5 7 = 12/2
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((3*2)/0.034));
    expected_distance = 5; // 5 7 3 = 15/3
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((25*2)/0.034));
    expected_distance = 10; // 5 7 3 25 = 40/4
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((10*2)/0.034));
    expected_distance = 10; // 5 7 3 25 10 = 50/5
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((4*2)/0.034));
    expected_distance = 9; // 5 7 3 25 10 4 = 54/6
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((2*2)/0.034));
    expected_distance = 8; // 5 7 3 25 10 4 2 = 56/7
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
    // next
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((8*2)/0.034));
    expected_distance = 8; // 5 7 3 25 10 4 2 8 = 64/8
    EXPECT_EQ(expected_distance, sonar_under_test->get_average_distance_cm());
}


TEST_F(TestSonar, test_median) {
    int expected_distance = 50;
    for(int i=0;i<NUMBER_OF_MEDIAN_VALUES;i++) {
        EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((i*2)/0.034));
        sonar_under_test->get_median_distance_cm();
    }
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((1*2)/0.034));
    EXPECT_EQ(expected_distance, sonar_under_test->get_median_distance_cm());
    for(int i=0;i<NUMBER_OF_MEDIAN_VALUES;i++) {
        EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((22*2)/0.034));
        sonar_under_test->get_median_distance_cm();
    }
    expected_distance = 22;
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((400*2)/0.034));
    EXPECT_EQ(expected_distance, sonar_under_test->get_median_distance_cm());
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((1*2)/0.034));
    EXPECT_EQ(expected_distance, sonar_under_test->get_median_distance_cm());
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((2*2)/0.034));
    EXPECT_EQ(expected_distance, sonar_under_test->get_median_distance_cm());
    EXPECT_CALL(*arduino_mock, pulseIn(_, _, _)).WillOnce(Return((3*2)/0.034));
    EXPECT_EQ(expected_distance, sonar_under_test->get_median_distance_cm());
}
