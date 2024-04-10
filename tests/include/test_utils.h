#ifndef TESTS_AUX_H
#define TESTS_AUX_H

#include <gtest/gtest.h>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

extern "C" {
#include "hw4.h"
}

#define PORT 8080

#define INFO(...) fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr)
#define ERROR(...) fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr)

char *load_file(const char *filename);
void clear_file(const char *filename);
void expect_no_valgrind_errors(int status);
void expect_no_asan_errors(int status);
int run_using_valgrind(const char *test_name);
int run_using_asan(const char *test_name);
void structs_equal(ChessGame *expected, ChessGame *actual);

#endif