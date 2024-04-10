#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

int make_move_soln(ChessGame *game, ChessMove *move, int isClient, bool validateMove);

class initialization_testSuite : public testing::Test { 
};

TEST_F(initialization_testSuite, initialization) {
    ChessGame game;

    // fill fields with garbage
    game.moveCount = 21323123;
    game.capturedCount = 123123;
    game.currentPlayer = 123123;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            game.chessboard[i][j] = 'X';
        }
    }

    
    initialize_game(&game);

    
    EXPECT_EQ(game.moveCount, 0);
    EXPECT_EQ(game.capturedCount, 0);
    EXPECT_EQ(game.currentPlayer, 0); // White's turn
    
    char expectedBoard[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            EXPECT_EQ(game.chessboard[i][j], expectedBoard[i][j]);
        }
    }
}


