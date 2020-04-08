#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

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
    virtual void run_calculate_average_loop(int number_of_runs, int expected_result) {
        for (int i = 1; i < number_of_runs; i++) {
            EXPECT_CALL(*arduino_mock, pulseIn(_, _, _))
                            .WillOnce(Return((i*2)/0.034));
            sonar_under_test->get_average_distance_cm();
        }

        EXPECT_CALL(*arduino_mock, pulseIn(_, _, _))
                        .WillOnce(Return((number_of_runs*2)/0.034));
        EXPECT_EQ(sonar_under_test->get_average_distance_cm(), expected_result);
    }
};

TEST_F(TestSonar, test_sonar_init) {EXPECT_TRUE(1); }

TEST_F(TestSonar, test_get_distance_cm) {
    unsigned int expected_result = 5;
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_TRIGGER, HIGH)).Times(1);
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_TRIGGER, LOW)).Times(2);
    EXPECT_CALL(*arduino_mock, delayMicroseconds(10)).Times(1);
    EXPECT_CALL(*arduino_mock, delayMicroseconds(2)).Times(1);
    EXPECT_CALL(*arduino_mock, pulseIn(TEST_PIN_ECHO, HIGH, _))
                                        .Times(1)
                                        .WillOnce(Return((expected_result*2)/0.034));
    EXPECT_EQ(sonar_under_test->get_distance_cm(), expected_result);
}
TEST_F(TestSonar, test_average_part_of_first_iteration) {
    int number_of_runs = 10;
    int expected_result = 8;
    run_calculate_average_loop(number_of_runs, expected_result);
}

TEST_F(TestSonar, test_average_exactly_one_iteration) {
    int number_of_runs = 100;
    int expected_result = 98;
    run_calculate_average_loop(number_of_runs, expected_result);
}

TEST_F(TestSonar, test_average_more_then_one_iteration) {
    int number_of_runs = 300;
    int expected_result = 300; // inaccuracy of formular
    run_calculate_average_loop(number_of_runs, expected_result);
}