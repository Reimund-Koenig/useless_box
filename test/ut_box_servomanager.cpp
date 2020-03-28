#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_serial.hpp"
#include "mocks/mock_servomotor.hpp"
#include "mocks/mock_box.hpp"

#include "box_servomanager.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Servomanager_under_test : public box::Servomanager {
    Servomanager_under_test(box::Servomotor* box_lower_servo,
                            box::Servomotor* box_upper_servo)
                    : Servomanager(box_lower_servo, box_upper_servo) {}
};


class TestServomanager : public ::testing::Test {
      protected:
    Servomanager_under_test* servomanager_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        box_lower_servo_mock = new NiceMock<BoxServoMock>;
        box_upper_servo_mock = new NiceMock<BoxServoMock>;
        servomanager_under_test = new Servomanager_under_test(
                                            (box::Servomotor*) box_lower_servo_mock,
                                            (box::Servomotor*) box_upper_servo_mock);
    }
    virtual void TearDown() {
        delete servomanager_under_test;
        delete box_upper_servo_mock;
        delete box_lower_servo_mock;
        delete servomotor_mock;
        delete arduino_mock;
    }
};

/**************************************************************************************************
 * If user gets close to the switch
 *    50% nothing happens
 *    50% useless box self switch
 *    0% useless box deke (antäuschen) self switch --> feature
 */
TEST_F(TestServomanager, test_servomanager_init) { EXPECT_TRUE(true); }
