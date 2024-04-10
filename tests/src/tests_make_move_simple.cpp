#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

void initialize_game_soln(ChessGame *game);

class make_move_simple_TestSuite : public testing::Test { 
};

TEST_F(make_move_simple_TestSuite, make_move_valid) {
    INFO("Test a valid move");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    parse_move("e2e4", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(0, result);
    EXPECT_EQ('P', game.chessboard[4][4]);
    EXPECT_EQ('.', game.chessboard[6][4]);
    EXPECT_EQ(1, game.moveCount);
    EXPECT_EQ(BLACK_PLAYER, game.currentPlayer);
}

TEST_F(make_move_simple_TestSuite, make_move_valid_but_then_make_move_invalid) {
    INFO("Test a valid move followed by an invalid move");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    // Make a valid move
    parse_move("e2e4", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(0, result);
    EXPECT_EQ('P', game.chessboard[4][4]);
    EXPECT_EQ('.', game.chessboard[6][4]);
    EXPECT_EQ(1, game.moveCount);
    EXPECT_EQ(BLACK_PLAYER, game.currentPlayer);
    // Make an invalid move
    parse_move("e4e5", &move);
    result = make_move(&game, &move, 0, 1);
    EXPECT_EQ(MOVE_WRONG_COLOR, result);
    EXPECT_EQ(1, game.moveCount);
    EXPECT_EQ(BLACK_PLAYER, game.currentPlayer);
}

TEST_F(make_move_simple_TestSuite, make_move_capture_opponent_piece) {
    INFO("Test capturing an opponent's piece");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    // Make a series of moves to set up the capture
    parse_move("e2e4", &move);
    make_move(&game, &move, 1, 1);
    parse_move("d7d5", &move);
    make_move(&game, &move, 0, 1);
    // Capture the opponent's pawn
    parse_move("e4d5", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(0, result);
    EXPECT_EQ('P', game.chessboard[3][3]);
    EXPECT_EQ('.', game.chessboard[4][4]);
    EXPECT_EQ(3, game.moveCount);
    EXPECT_EQ(BLACK_PLAYER, game.currentPlayer);
    EXPECT_EQ(1, game.capturedCount);
    EXPECT_EQ('p', game.capturedPieces[0]);
}

TEST_F(make_move_simple_TestSuite, make_move_nothing) {
    INFO("Test moving from an empty square");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    parse_move("e4e5", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(MOVE_NOTHING, result);
    EXPECT_EQ(0, game.moveCount);
    EXPECT_EQ(WHITE_PLAYER, game.currentPlayer);
}

TEST_F(make_move_simple_TestSuite, make_move_wrong_color) {
    INFO("Test moving the opponent's piece");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    parse_move("e7e5", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(MOVE_WRONG_COLOR, result);
    EXPECT_EQ(0, game.moveCount);
    EXPECT_EQ(0, game.currentPlayer);
}

TEST_F(make_move_simple_TestSuite, make_move_out_of_turn) {
    INFO("Test moving out of turn");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    parse_move("e2e4", &move);
    int result = make_move(&game, &move, 0, 1);
    EXPECT_EQ(MOVE_OUT_OF_TURN, result);
    EXPECT_EQ(0, game.moveCount);
    EXPECT_EQ(0, game.currentPlayer);
}

TEST_F(make_move_simple_TestSuite, make_move_capture_own_piece) {
    INFO("Test capturing own piece");
    ChessGame game;
    initialize_game_soln(&game);
    ChessMove move;
    parse_move("e2e1", &move);
    int result = make_move(&game, &move, 1, 1);
    EXPECT_EQ(MOVE_SUS, result);
    EXPECT_EQ(0, game.moveCount);
    EXPECT_EQ(0, game.currentPlayer);
}


TEST_F(make_move_simple_TestSuite, make_move_promote) {
    INFO("Make move cause promotion of white piece to queen");
    ChessGame game;
    initialize_game_soln(&game);
    fen_to_chessboard("8/7P/8/8/8/8/8/8 w", &game);
    
    ChessMove move;
    parse_move("h7h8q", &move);
    int result = make_move(&game, &move, 1, 1); 
    EXPECT_EQ(0, result);
    char resultPiece = game.chessboard[0][7];
    EXPECT_EQ(resultPiece, 'Q');
}
