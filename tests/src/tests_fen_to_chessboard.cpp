#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

void initialize_game_soln(ChessGame *game);
void fen_to_chessboard_soln(const char *fen, ChessGame *game);

class fen_to_chessboard_TestSuite : public testing::Test { 
};

TEST_F(fen_to_chessboard_TestSuite, fen_to_board01) {
    const char *fen = "r1bqkbnr/pp1ppp1p/2n3p1/8/3NP3/8/PPP2PPP/RNBQKB1R w";  // B32 Sicilian Defense: Accelerated Dragon
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game); 
    structs_equal(&exp_game, &act_game);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board02) {
    const char *fen = "5rk1/5ppp/p1pbr3/1p1n3q/P2P2b1/1BPQB1P1/1P1N1P1P/R3R1K1 w"; //  C89 Ruy Lopez: Marshall, Main line, Spassky variation
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game); 
    structs_equal(&exp_game, &act_game);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board03) {
    const char *fen = "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPPKPPP/RNBQ1BNR b"; // Bongcloud Attack
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game); 
    structs_equal(&exp_game, &act_game);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board04) {
    const char *fen = "p6p/8/1rp2p2/3pP3/2pPbpP1/1PKpkBr1/1RNQNPR1/q1nBnb1P b"; // Random board
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game); 
    structs_equal(&exp_game, &act_game);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board05) {
    const char *fen = "3k4/8/8/8/8/8/8/4K3 w"; // Random board
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game); 
    structs_equal(&exp_game, &act_game);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board21) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkb1r/pppp1ppp/4pn2/8/2PP4/6P1/PP2PP1P/RNBQKBNR b";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(BLACK_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board22) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(BLACK_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board23) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkb1r/pppp1ppp/5n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R b";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(BLACK_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board24) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkbnr/pppp1p1p/8/6p1/4Pp2/5N2/PPPP2PP/RNBQKB1R w";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(WHITE_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board25) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkbnr/ppp2ppp/4p3/8/3Pp3/2N5/PPP2PPP/R1BQKBNR w";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(WHITE_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board26) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPPN1PPP/R1BQKBNR b";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(BLACK_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board27) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqk1nr/ppp2ppp/4p3/3p4/1b1PP3/2N5/PPP2PPP/R1BQKBNR w";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(WHITE_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board28) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(WHITE_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board29) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkb1r/ppp1pp1p/3p1np1/8/3PP3/2N2N2/PPP2PPP/R1BQKB1R b";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(BLACK_PLAYER, act_game.currentPlayer);
}

TEST_F(fen_to_chessboard_TestSuite, fen_to_board30) {
    INFO("Convert a FEN string to a chessboard given a random board.");
    const char *fen = "rnbqkb1r/p2ppppp/5n2/1ppP4/2P5/8/PP2PPPP/RNBQKBNR w";
    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    fen_to_chessboard_soln(fen, &exp_game);
    ChessGame act_game;
    initialize_game(&act_game);
    fen_to_chessboard(fen, &act_game);
    structs_equal(&exp_game, &act_game);
    EXPECT_EQ(WHITE_PLAYER, act_game.currentPlayer);
}
