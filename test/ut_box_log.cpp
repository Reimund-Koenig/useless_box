#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "log.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;


class TestLog : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

TEST_F(TestLog, test_wait_init) {
    const unsigned BUFFER_SIZE = 10;
    char test[BUFFER_SIZE];
    test[0] = '0';
    test[1] = '1';
    test[2] = '2';
    test[3] = '3';
    test[4] = '4';
    test[5] = '5';
    test[6] = '6';
    test[7] = '7';
    test[8] = '8';
    test[9] = '9';
    char msg[] = "abc";
    int len = (int)strlen(msg);
    int startIdx = BUFFER_SIZE-len;
    memcpy(&test[0],&test[len],startIdx);
    memcpy(&test[startIdx],msg,len);
    printf("TEST: %s\n", test);
    printf("sizeof(char): %d\n", (int)sizeof(char));
    LOG("012345");
    LOG("6789");
    printf("\nPRINT_LOGS_TEST:\n");
    PRINT_LOGS_TEST();
 }
