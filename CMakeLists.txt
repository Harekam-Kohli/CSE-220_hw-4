cmake_minimum_required(VERSION 3.10)
project(hw4 LANGUAGES C CXX)
find_package(GTest REQUIRED)

# Set the C standard for the project
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)

option(BUILD_CODEGRADE_TESTS "Build test suites into separate executables" OFF)

include_directories(include)
link_directories(lib)

# Build the main executable
add_executable(hw4_client src/hw4_client.c src/hw4.c)
add_executable(hw4_server src/hw4_server.c src/hw4.c)

# Build executables for solution we will distribute to the students. We should obfuscate the helper function names.
add_executable(hw4_client_soln src/hw4_client.c src/hw4.c)
add_executable(hw4_server_soln src/hw4_server.c src/hw4.c)
target_compile_options(hw4_client_soln PUBLIC -O3)
target_compile_options(hw4_server_soln PUBLIC -O3)

# Build executables
target_compile_options(hw4_client PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_compile_options(hw4_server PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(hw4_client PUBLIC m)
target_link_libraries(hw4_server PUBLIC m)

# Build testing scripts
set(TEST_SUITES "chessboard_to_fen" "fen_to_chessboard" "initialize_game" "is_valid" "make_move_simple" "make_move_complex" "parse_move" "save_load_game" "send_receive_command" "simulations")
# Build standalone test case suites for CodeGrade
if (BUILD_CODEGRADE_TESTS)
    foreach(TEST_SUITE IN LISTS TEST_SUITES)
        add_executable(tests_${TEST_SUITE} src/hw4.c tests/src/tests_${TEST_SUITE}.cpp tests/src/test_utils.cpp)
        target_compile_options(tests_${TEST_SUITE} PRIVATE -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
        target_include_directories(tests_${TEST_SUITE} PRIVATE ${CMAKE_SOURCE_DIR}/tests/include)
        target_link_options(tests_${TEST_SUITE} PRIVATE -no-pie)
        target_link_libraries(tests_${TEST_SUITE} PRIVATE gtest gtest_main pthread m hw4_lib)
    endforeach()
else()
    # Build a single executable with all the tests for development
    file(GLOB TEST_SOURCES tests/src/tests_*.cpp)
    add_executable(run_tests src/hw4.c ${TEST_SOURCES} tests/src/test_utils.cpp)
    target_compile_options(run_tests PRIVATE -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
    target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/include)
    target_link_options(run_tests PRIVATE -no-pie)
    target_link_libraries(run_tests PUBLIC gtest gtest_main pthread m hw4_lib)
endif()

