#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

void initialize_game_soln(ChessGame *game);
int parse_move_soln(const char *move, ChessMove *parsed_move);

class parse_move_TestSuite : public testing::Test { 
};


TEST_F(parse_move_TestSuite, make_move_invalid_format1) {
    const char *move = "e2e456";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    ChessMove exp_move;
    int exp_ret = parse_move_soln(move, &exp_move);
    ChessGame act_game;
    initialize_game_soln(&act_game);
    ChessMove act_move;
    int act_ret = parse_move(move, &act_move);
    EXPECT_EQ(exp_ret, act_ret);
}


TEST_F(parse_move_TestSuite, parse_move_invalid_format1) {
    const char *move = "e2e9";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_OUT_OF_BOUNDS, ret);
}

TEST_F(parse_move_TestSuite, parse_move_invalid_format2) {
    const char *move = "9e2e4";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_FORMAT, ret);
}

TEST_F(parse_move_TestSuite, parse_move_out_of_bounds1) {
    const char *move = "e10e1";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_FORMAT, ret);
}

TEST_F(parse_move_TestSuite, parse_move_out_of_bounds2) {
    const char *move = "e10e";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_FORMAT, ret);
}

TEST_F(parse_move_TestSuite, parse_move_invalid_destination1) {
    const char *move = "e1e5q";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_DESTINATION, ret);
}

TEST_F(parse_move_TestSuite, parse_move_invalid_destination2) {
    const char *move = "e0e4q";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_OUT_OF_BOUNDS, ret);
}

TEST_F(parse_move_TestSuite, parse_move_invalid_promotion1) {
    const char *move = "e7e81";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_PROMOTION, ret);
}

TEST_F(parse_move_TestSuite, parse_move_invalid_promotion2) {
    const char *move = "a2a1;";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(PARSE_MOVE_INVALID_PROMOTION, ret);
}

TEST_F(parse_move_TestSuite, parse_move_valid_move1) {
    const char *move = "d1d7";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(0, ret);
    EXPECT_STREQ("d1", parsed_move.startSquare);
    EXPECT_STREQ("d7", parsed_move.endSquare);
}

TEST_F(parse_move_TestSuite, parse_move_valid_move2) {
    const char *move = "g1f7";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(0, ret);
    EXPECT_STREQ("g1", parsed_move.startSquare);
    EXPECT_STREQ("f7", parsed_move.endSquare);
}

TEST_F(parse_move_TestSuite, parse_move_valid_promotion1) {
    const char *move = "d7d8b";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(0, ret);
    EXPECT_STREQ("d7", parsed_move.startSquare);
    EXPECT_STREQ("d8b", parsed_move.endSquare);
}

TEST_F(parse_move_TestSuite, parse_move_valid_promotion2) {
    const char *move = "a2a1r";
    ChessMove parsed_move;
    int ret = parse_move(move, &parsed_move);
    EXPECT_EQ(0, ret);
    EXPECT_STREQ("a2", parsed_move.startSquare);
    EXPECT_STREQ("a1r", parsed_move.endSquare);
}


TEST_F(parse_move_TestSuite, parse_move_combined_invalid_cases) {
    const char *moves[] = {
        "e2e456",
        "e2",
        "e9e4",
        "g2e9",
        "e7e5q",
        "e2e4q",
        "e7e8z",
        "a2a1p"
    };

    int expected_return_codes[] = {
        PARSE_MOVE_INVALID_FORMAT,
        PARSE_MOVE_INVALID_FORMAT,
        PARSE_MOVE_OUT_OF_BOUNDS,
        PARSE_MOVE_OUT_OF_BOUNDS,
        PARSE_MOVE_INVALID_DESTINATION,
        PARSE_MOVE_INVALID_DESTINATION,
        PARSE_MOVE_INVALID_PROMOTION,
        PARSE_MOVE_INVALID_PROMOTION
    };

    for (int i = 0; i < static_cast<int>(sizeof(moves) / sizeof(moves[0])); ++i) {
        ChessMove parsed_move;
        int ret = parse_move(moves[i], &parsed_move);
        EXPECT_EQ(expected_return_codes[i], ret) << "Failed on move: " << moves[i];
    }
}

TEST_F(parse_move_TestSuite, parse_move_combined_valid_cases) {
    const char *moves[] = {
        "e2e4",
        "g1f3",
        "e7e8q",
        "a2a1n"
    };

    const char *expected_start_squares[] = {
        "e2",
        "g1",
        "e7",
        "a2"
    };

    const char *expected_end_squares[] = {
        "e4",
        "f3",
        "e8q",
        "a1n"
    };

    for (int i = 0; i < static_cast<int>(sizeof(moves) / sizeof(moves[0])); ++i) {
        ChessMove parsed_move;
        int ret = parse_move(moves[i], &parsed_move);
        EXPECT_EQ(0, ret) << "Failed on move: " << moves[i];
        EXPECT_STREQ(expected_start_squares[i], parsed_move.startSquare) << "Failed on move: " << moves[i];
        EXPECT_STREQ(expected_end_squares[i], parsed_move.endSquare) << "Failed on move: " << moves[i];
    }
}

TEST_F(parse_move_TestSuite, parse_move_mixed_valid_invalid_cases) {
    const char *moves[] = {
        "e2e4",
        "aaskldjfklasjdlkfjasl;dkjkla;sjd",
        "g1f3",
        "e9e4",
        "e7e8q",
        "e7e5q",
        "a2a1n",
        "a2a1p"
    };

    int expected_return_codes[] = {
        0,
        PARSE_MOVE_INVALID_FORMAT,
        0,
        PARSE_MOVE_OUT_OF_BOUNDS,
        0,
        PARSE_MOVE_INVALID_DESTINATION,
        0,
        PARSE_MOVE_INVALID_PROMOTION
    };

    const char *expected_start_squares[] = {
        "e2",
        "",
        "g1",
        "",
        "e7",
        "",
        "a2",
        ""
    };

    const char *expected_end_squares[] = {
        "e4",
        "",
        "f3",
        "",
        "e8q",
        "",
        "a1n",
        ""
    };

    for (int i = 0; i < static_cast<int>(sizeof(moves) / sizeof(moves[0])); ++i) {
        ChessMove parsed_move;
        int ret = parse_move(moves[i], &parsed_move);
        EXPECT_EQ(expected_return_codes[i], ret) << "Failed on move: " << moves[i];

        if (expected_return_codes[i] == 0) {
            EXPECT_STREQ(expected_start_squares[i], parsed_move.startSquare) << "Failed on move: " << moves[i];
            EXPECT_STREQ(expected_end_squares[i], parsed_move.endSquare) << "Failed on move: " << moves[i];
        }
    }
}
