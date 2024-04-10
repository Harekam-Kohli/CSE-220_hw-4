#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

void fen_to_chessboard_soln(const char *fen, ChessGame *game);

class is_valid_TestSuite : public testing::Test { 
    void SetUp() override {
    }
};

TEST_F(is_valid_TestSuite, is_valid_pawn_move_white) {
    INFO("Check if a pawn move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/r1bqk1nr/pp1nbppp/2pp4/4p3/2BPP3/2N2N2/PPP2PPP/R1BQK2R_w_KQkq_-_0_1?color=white
    fen_to_chessboard_soln("r1bqk1nr/pp1nbppp/2pp4/4p3/2BPP3/2N2N2/PPP2PPP/R1BQK2R w", &game);
    EXPECT_EQ(true, is_valid_pawn_move('P', 6, 6, 4, 6, &game)) ; // valid movement
    EXPECT_EQ(true, is_valid_pawn_move('P', 6, 1, 5, 1, &game)) ; // valid movement
    EXPECT_EQ(false, is_valid_pawn_move('P', 6, 5, 4, 5, &game)) ; // invalid movement
    EXPECT_EQ(false, is_valid_pawn_move('P', 4, 4, 3, 4, &game)) ; // invalid movement
    EXPECT_EQ(true, is_valid_pawn_move('P', 4, 3, 3, 4, &game)) ; // valid movement - capture
    EXPECT_EQ(false, is_valid_pawn_move('P', 4, 3, 3, 2, &game)) ; // invalid diagonal movement
    EXPECT_EQ(false, is_valid_pawn_move('P', 4, 3, 5, 3, &game)) ; // invalid backwards
}

TEST_F(is_valid_TestSuite, is_valid_pawn_move_black) {
    INFO("Check if a pawn move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    
    // https://lichess.org/editor/r1bqk1nr/pp1nbppp/2pp4/4N3/2BPP3/2N5/PPP2PPP/R1BQK2R_w_KQkq_-_0_1?color=white
    fen_to_chessboard_soln("r1bqk1nr/pp1nbppp/2pp4/4N3/2BPP3/2N5/PPP2PPP/R1BQK2R b", &game);
    EXPECT_EQ(true, is_valid_pawn_move('p', 1, 7, 3, 7, &game)) ; // valid movement
    EXPECT_EQ(true, is_valid_pawn_move('p', 1, 7, 2, 7, &game)) ; // valid movement
    EXPECT_EQ(true, is_valid_pawn_move('p', 2, 3, 3, 3, &game)) ; // valid movement
    EXPECT_EQ(true, is_valid_pawn_move('p', 2, 3, 3, 4, &game)) ; // valid movement - capture
    EXPECT_EQ(false, is_valid_pawn_move('p', 3, 4, 4, 4, &game)) ; // invalid movement
    EXPECT_EQ(false, is_valid_pawn_move('p', 2, 2, 1, 2, &game)) ; // invalid movement backwards
}

TEST_F(is_valid_TestSuite, is_valid_rook_move_white) {
    INFO("Check if a rook move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_rook_move(7, 4, 4, 4, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_rook_move(7, 4, 7, 1, &game)) ; // invalid move
    EXPECT_EQ(true, is_valid_rook_move(5, 0, 7, 0, &game)) ; // valid move
    EXPECT_EQ(false, is_valid_rook_move(5, 0, 5, 3, &game)) ; // invalid move
    EXPECT_EQ(false, is_valid_rook_move(5, 2, 2, 6, &game)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_rook_move_black) {
    INFO("Check if a rook move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_rook_move(0, 6, 3, 6, &game)) ; // valid capture
    EXPECT_EQ(true, is_valid_rook_move(0, 6, 0, 3, &game)) ; // valid move
    EXPECT_EQ(true, is_valid_rook_move(0, 1, 2, 1, &game)) ; // valid move
    EXPECT_EQ(false, is_valid_rook_move(0, 1, 0, 5, &game)) ; // invalid move
    EXPECT_EQ(false, is_valid_rook_move(0, 1, 0, 3, &game)) ; // invalid move
    EXPECT_EQ(false, is_valid_rook_move(0, 1, 2, 3, &game)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_knight_move_white) {
    INFO("Check if a knight move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_knight_move(5, 5, 3, 4)) ; // valid capture
    EXPECT_EQ(true, is_valid_knight_move(5, 5, 6, 7)) ; // valid move
    EXPECT_EQ(false, is_valid_knight_move(5, 5, 2, 3)) ; // invalid move
    EXPECT_EQ(false, is_valid_knight_move(7, 1, 7, 3)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_knight_move_black) {
    INFO("Check if a knight move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_knight_move(4, 1, 2, 2)) ; // valid move
    EXPECT_EQ(true, is_valid_knight_move(4, 1, 5, 3)) ; // valid move
    EXPECT_EQ(false, is_valid_knight_move(4, 6, 3, 2)) ; // invalid move
    EXPECT_EQ(false, is_valid_knight_move(4, 6, 5, 3)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_bishop_move_white) {
    INFO("Check if a bishop move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_bishop_move(7, 2, 3, 6, &game)) ; // valid capture
    EXPECT_EQ(true, is_valid_bishop_move(7, 2, 5, 4, &game)) ; // valid move
    EXPECT_EQ(true, is_valid_bishop_move(0, 4, 2, 2, &game)) ; // valid move
    EXPECT_EQ(false, is_valid_bishop_move(0, 4, 5, 4, &game)) ; // invalid move
    EXPECT_EQ(false, is_valid_bishop_move(7, 2, 5, 0, &game)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_bishop_move_black) {
    INFO("Check if a bishop move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
     EXPECT_EQ(true, is_valid_bishop_move(1, 4, 3, 6, &game)) ; // valid move
     EXPECT_EQ(false, is_valid_bishop_move(1, 4, 5, 0, &game)) ; // invalid move
     EXPECT_EQ(true, is_valid_bishop_move(0, 2, 1, 3, &game)) ; // valid capture
     EXPECT_EQ(true, is_valid_bishop_move(0, 2, 2, 0, &game)) ; // valid move
     EXPECT_EQ(false, is_valid_bishop_move(0, 2, 4, 4, &game)) ; // invalid move  
}

TEST_F(is_valid_TestSuite, is_valid_queen_move_white) {
    INFO("Check if a queen move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_queen_move(5, 3, 3, 1, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_queen_move(5, 3, 2, 6, &game)) ; // invalid move
    EXPECT_EQ(true, is_valid_queen_move(5, 3, 7, 5, &game)) ; // valid move
    EXPECT_EQ(true, is_valid_queen_move(5, 3, 7, 3, &game)) ; // valid move
}

TEST_F(is_valid_TestSuite, is_valid_queen_move_black) {
    INFO("Check if a queen move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1q2P1/PnPPppn1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1q2P1/PnPPppn1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_queen_move(3, 3, 3, 6, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_queen_move(3, 3, 3, 0, &game)) ; // invalid move
    EXPECT_EQ(true, is_valid_queen_move(3, 3, 3, 5, &game)) ; // valid move
    EXPECT_EQ(true, is_valid_queen_move(3, 3, 2, 3, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_queen_move(3, 3, 6, 6, &game)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_king_move_white) {
    INFO("Check if a king move is valid. It is white's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_king_move(7, 6, 7, 7)) ; // valid move
    EXPECT_EQ(true, is_valid_king_move(7, 6, 6, 7)) ; // valid move
    EXPECT_EQ(false, is_valid_king_move(7, 6, 6, 3)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_king_move_black) {
    INFO("Check if a king move is valid. It is black's turn.");
    ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/PnPPp1nP/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/PnPPp1nP/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_king_move(1, 5, 0, 4)) ; // valid capture
    EXPECT_EQ(true, is_valid_king_move(1, 5, 2, 5)) ; // valid move
    EXPECT_EQ(false, is_valid_king_move(1, 5, 5, 2)) ; // invalid move
}

TEST_F(is_valid_TestSuite, is_valid_function) {
ChessGame game;
    initialize_game(&game);
    // https://lichess.org/editor/r1bqk1nr/pp1nbppp/2pp4/4N3/2BPP3/2N5/PPP2PPP/R1BQK2R_w_KQkq_-_0_1?color=white
    fen_to_chessboard_soln("r1bqk1nr/pp1nbppp/2pp4/4p3/2BPP3/2N2N2/PPP2PPP/R1BQK2R w", &game);
    EXPECT_EQ(true, is_valid_move('P', 6, 6, 4, 6, &game)) ; // valid movement
    EXPECT_EQ(false, is_valid_move('P', 6, 5, 4, 5, &game)) ; // invalid movement
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_move('R', 7, 4, 4, 4, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_move('R', 7, 4, 7, 1, &game)) ; // invalid move
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1qp1P1/PnPPp1n1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_move('n', 4, 1, 5, 3, &game)) ; // valid move
    EXPECT_EQ(false, is_valid_move('n', 4, 6, 3, 2, &game)) ; // invalid move
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_move('B', 7, 2, 3, 6, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_move('B', 0, 4, 5, 4, &game)) ; // invalid move
    initialize_game(&game);
    // https://lichess.org/editor/1rb3r1/2pBbk1p/8/pp1q2P1/PnPPppn1/R2Q1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb3r1/2pBbk1p/8/pp1q2P1/PnPPppn1/R2Q1N2/1P3PP1/1NB1R1K1 b", &game);
    EXPECT_EQ(true, is_valid_move('q', 3, 3, 3, 6, &game)) ; // valid capture
    EXPECT_EQ(false, is_valid_move('q', 3, 3, 3, 0, &game)) ; // invalid move
    initialize_game(&game);
    // https://lichess.org/editor/1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1_w_-_-_0_1?color=white
    fen_to_chessboard_soln("1rb1B2r/2p1bk1p/8/pp1qp1p1/Pn1Pp1nP/R1PQ1N2/1P3PP1/1NB1R1K1 w", &game);
    EXPECT_EQ(true, is_valid_move('K', 7, 6, 7, 7, &game)) ; // valid move
    EXPECT_EQ(false, is_valid_move('K', 7, 6, 6, 3, &game)) ; // invalid move

}
