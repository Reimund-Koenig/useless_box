#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_serial.hpp"
#include "mocks/mock_servomotor.hpp"
#include "mocks/mock_box.hpp"

#include "box_main.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Main_under_test : public box::Main {
    Main_under_test(box::Switch* box_switch,
                    box::Sonar* box_sonar,
                    box::Servomanager* box_servomanager)
                    : Main(box_switch, box_sonar,
                           box_servomanager) {}
};


class TestMain : public ::testing::Test {
      protected:
    Main_under_test* main_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_sonar_mock = new NiceMock<BoxSonarMock>;
        box_servomanager = new NiceMock<BoxServoManagerMock>;
        main_under_test = new Main_under_test((box::Switch*) box_switch_mock,
                                              (box::Sonar*) box_sonar_mock,
                                              (box::Servomanager*) box_servomanager);
    }
    virtual void TearDown() {
        delete servomotor_mock;
        delete arduino_mock;
        delete box_switch_mock;
        delete box_sonar_mock;
        delete box_servomanager;
        delete main_under_test;
    }
};

/**************************************************************************************************
 * If user gets close to the switch
 *    50% nothing happens
 *    50% useless box self switch
 *    0% useless box deke (antäuschen) self switch --> feature
 */
TEST_F(TestMain, test_main_init) { EXPECT_TRUE(true); }

// TEST_F(TestMain, test_main_reset_on_switchchange) {
//     EXPECT_CALL(*box_switch_mock, has_changed()).Times(1).WillOnce(Return(0));
//     main_under_test->run();
// }
