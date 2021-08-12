cmake_minimum_required(VERSION 3.2.2)

project(UselessBoxUnitTest)
include(CTest)
add_compile_options(-std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable)

#*************************************************************************************************
# Download googletest
#************************************************

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

add_subdirectory(   "${CMAKE_BINARY_DIR}/googletest-src"
                    "${CMAKE_BINARY_DIR}/googletest-build")

#*************************************************************************************************
# Set sources
#************************************************

set(BOX_SRC         "${CMAKE_CURRENT_LIST_DIR}/../src")
set(BOX_INCLUDE     "${CMAKE_CURRENT_LIST_DIR}/../include")
set(BOX_MOCKS       "${CMAKE_CURRENT_LIST_DIR}/mocks")
set(BOX_STUBS       "${CMAKE_CURRENT_LIST_DIR}/stubs")

#*************************************************************************************************
# Enable debugging
#************************************************

SET(GCC_COMPILE_FLAGS "-ggdb3")
SET(CMAKE_CXX_FLAGS "-pthread -DUNITTEST")
add_definitions(${GCC_COMPILE_FLAGS})
add_definitions(${CMAKE_CXX_FLAGS})

#*************************************************************************************************
# Include test directories
#************************************************

include_directories(
  ${gtest_SOURCE_DIR}/include
  ${gmock_SOURCE_DIR}/include
)

#*************************************************************************************************
# Macro to register the tests
#************************************************

macro(register_test test_name)
    add_executable(${test_name} ${test_name}.cpp ${ARGN})
    target_link_libraries(${test_name} gmock_main uselessbox)
    target_include_directories(${test_name} PRIVATE ${BOX_MOCKS} ${BOX_STUBS} ${BOX_INCLUDE})
    add_test (NAME ${test_name} COMMAND ${test_name})
endmacro()
enable_testing()

#*************************************************************************************************
# Add sources to library to reduce build times
#************************************************
add_library(uselessbox
    ${BOX_SRC}/box_main.cpp
    ${BOX_SRC}/box_controller.cpp
    ${BOX_SRC}/manager/box_servo_manager.cpp
    ${BOX_SRC}/manager/box_mode_manager.cpp
    ${BOX_SRC}/manager/box_wait_manager.cpp
    ${BOX_SRC}/peripheral/box_sonar.cpp
    ${BOX_SRC}/peripheral/box_switch.cpp
    ${BOX_SRC}/peripheral/box_servomotor.cpp
    ${BOX_SRC}/modes/box_submode_function_jitter.cpp
    ${BOX_SRC}/modes/box_mode_awareness.cpp
    ${BOX_SRC}/modes/box_mode_reset.cpp
    ${BOX_SRC}/modes/box_mode_startup.cpp
)
target_include_directories(uselessbox PRIVATE ${BOX_MOCKS} ${BOX_STUBS} ${BOX_INCLUDE})

#*************************************************************************************************
# Add Tests
#************************************************
register_test(ut_box_controller)
register_test(ut_box_servomotor)
register_test(ut_box_servomanager)
register_test(ut_box_switch)
register_test(ut_box_sonar)
register_test(ut_subbox_mode_function_jitter)
register_test(ut_box_mode_awareness)
register_test(ut_box_mode_reset)
register_test(ut_box_mode_startup)
register_test(ut_box_wait)
