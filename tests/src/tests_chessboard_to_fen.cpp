#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

void initialize_game_soln(ChessGame *game);
int make_move_soln(ChessGame *game, ChessMove *move, bool is_client, bool validate_move);
void chessboard_to_fen_soln(char fen[], ChessGame *game);
void fen_to_chessboard_soln(const char *fen, ChessGame *game);

class chessboard_to_fen_TestSuite : public testing::Test { 
};

TEST_F(chessboard_to_fen_TestSuite, board_to_fen01) {
    INFO("Convert a chessboard to a FEN string after a sequence of moves.");
    ChessGame game;
    initialize_game_soln(&game);
    
    ChessMove moves[] = {
        {"e2", "e4"}, // w
        {"c7", "c6"}, // b
        {"b1", "c3"}, // w
        {"d7", "d6"}, // b
        {"d2", "d4"}, // w
        {"b8", "d7"}, // b
        {"g1", "f3"}, // w
        {"e7", "e5"}, // b
        {"f1", "c4"}, // w
        {"f8", "e7"}  // b
    };

    for (int i = 0; i < 10; i++) {
        make_move_soln(&game, &moves[i], (i + 1) % 2, 1);
    }
    
    char act_fen[BUFFER_SIZE];
    chessboard_to_fen(act_fen, &game);
    char exp_fen[BUFFER_SIZE];
    chessboard_to_fen_soln(exp_fen, &game);
    EXPECT_STREQ(exp_fen, act_fen);
}


TEST_F(chessboard_to_fen_TestSuite, board_to_fen02) {
    INFO("Convert a chessboard to a FEN string after a sequence of moves.");
    ChessGame game;
    initialize_game_soln(&game);
    
    ChessMove moves[] = {
        {"e2", "e4"},
        {"e7", "e5"},
        {"g1", "f3"},
        {"b8", "c6"},
        {"f1", "b5"},
        {"a7", "a6"},
        {"b5", "a4"},
        {"g8", "f6"}
    };

    for (int i = 0; i < 8; i++) {
        make_move_soln(&game, &moves[i], (i + 1) % 2, 1);
    }
    
    char act_fen[BUFFER_SIZE];
    chessboard_to_fen(act_fen, &game);
    char exp_fen[BUFFER_SIZE];
    chessboard_to_fen_soln(exp_fen, &game);
    EXPECT_STREQ(exp_fen, act_fen);
    // C89 Ruy Lopez: Marshall, Main line, Spassky variation (up to the castling part of the opening since castling isn't in specifications)
}

TEST_F(chessboard_to_fen_TestSuite, board_to_fen03) {
    INFO("Convert a chessboard to a FEN string after a sequence of moves.");
    ChessGame game;
    initialize_game_soln(&game);
    
    ChessMove moves[] = {
        {"e2", "e4"},
        {"e7", "e5"},
        {"g1", "f3"},
        {"b8", "c6"},
        {"f1", "b5"}
    };

    for (int i = 0; i < 5; i++) {
        make_move_soln(&game, &moves[i], (i + 1) % 2, 1);
    }
    
    char act_fen[BUFFER_SIZE];
    chessboard_to_fen(act_fen, &game);
    char exp_fen[BUFFER_SIZE];
    chessboard_to_fen_soln(exp_fen, &game);
    EXPECT_STREQ(exp_fen, act_fen);
}


TEST_F(chessboard_to_fen_TestSuite, board_to_fen04) {
    INFO("Convert a chessboard to a FEN string after a sequence of moves.");
    ChessGame game;
    initialize_game_soln(&game);
    
    ChessMove moves[] = {
        {"e2", "e4"},
        {"h7", "h5"},
        {"a2", "a4"},
        {"h8", "h7"},
        {"b2", "b4"},
        {"c7", "c5"},
        {"d2", "d4"},
        {"f7", "f5"},
        {"h2", "h4"},
        {"g8", "f6"},
        {"b1", "d2"},
        {"b8", "c6"},
        {"g1", "f3"},
        {"a8", "b8"},
        {"a1", "b1"},
        {"c6", "d4"},
        {"b1", "b3"},
        {"h7", "h8"}
    };

    for (int i = 0; i < 18; i++) {
        make_move_soln(&game, &moves[i], (i + 1) % 2, 1);
    }
    
    char act_fen[BUFFER_SIZE];
    chessboard_to_fen(act_fen, &game);
    char exp_fen[BUFFER_SIZE];
    chessboard_to_fen_soln(exp_fen, &game);
    EXPECT_STREQ(exp_fen, act_fen);
}

TEST_F(chessboard_to_fen_TestSuite, board_to_fen05) {
    INFO("Convert a chessboard to a FEN string given a random board.");
    ChessGame game;
    initialize_game_soln(&game);

    const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

    fen_to_chessboard_soln(fen, &game);

    char test_fen[BUFFER_SIZE];
    chessboard_to_fen(test_fen, &game);
    EXPECT_STREQ(fen, test_fen);
    EXPECT_EQ(WHITE_PLAYER, game.currentPlayer);
}


