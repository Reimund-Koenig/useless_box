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

set(UB_SRC         "${CMAKE_CURRENT_LIST_DIR}/../src")
set(UB_INCLUDE     "${CMAKE_CURRENT_LIST_DIR}/../include")

#*************************************************************************************************
# Enable debugging
#************************************************

SET(GCC_COMPILE_FLAGS "-ggdb3")
SET(CMAKE_CXX_FLAGS "-pthread")
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
    target_link_libraries(${test_name} gmock_main uselessbox systemd)
    target_include_directories(${test_name} PRIVATE ${UB_INCLUDE})
    add_test (NAME ${test_name} COMMAND ${test_name})
endmacro()
enable_testing()

#*************************************************************************************************
# Add sources to library to reduce build times
#************************************************
add_library(uselessbox
    ${UB_SRC}/ub_main.cpp
    ${UB_SRC}/ub_motor.cpp
    ${UB_SRC}/ub_switch.cpp
)
target_include_directories(uselessbox PRIVATE ${UB_INCLUDE})

#*************************************************************************************************
# Add Tests
#************************************************
register_test(test_ub_main)
register_test(test_ub_motor)
register_test(test_ub_switch)

