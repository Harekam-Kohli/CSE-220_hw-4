#include "test_utils.h"

extern "C" {
#include "hw4.h"
}

void initialize_game_soln(ChessGame *game);
int make_move_soln(ChessGame *game, ChessMove *move, bool is_client, bool validate_move);

class make_move_complex_TestSuite : public testing::Test {
};

TEST_F(make_move_complex_TestSuite, test01) {
    INFO("Play a few turns. It is white's turn.");
        // https://lichess.org/editor/r1bqk1nr/pp1nbppp/2pp4/4p3/2BPP3/2N2N2/PPP2PPP/R1BQK2R_w_KQkq_-_0_1?color=white
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

    ChessGame exp_game;
    initialize_game_soln(&exp_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);

    ChessGame act_game;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move(&act_game, &moves[i], (i + 1) % 2, 1);

    structs_equal(&exp_game, &act_game);    
}

TEST_F(make_move_complex_TestSuite, test02) {
    INFO("Play a few turns. It is black's turn.");
    // https://lichess.org/editor/r1bqk1nr/pp1nbppp/2pp4/4N3/2BPP3/2N5/PPP2PPP/R1BQK2R_w_KQkq_-_0_1?color=white
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
        {"f8", "e7"}, // b
        {"f3", "e5"}  // w
    };

    ChessGame exp_game;
    initialize_game_soln(&exp_game);  
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);

    ChessGame act_game;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move(&act_game, &moves[i], (i + 1) % 2, 1);

    structs_equal(&exp_game, &act_game);   
}

TEST_F(make_move_complex_TestSuite, test03){
    INFO("Play a few turns and take some pieces.");
    //https://lichess.org/editor/rnb1kbnr/pp1ppppp/8/2p5/2P5/2P1P3/PP3PPP/R1BQKBNR_w_KQkq_-_0_1?color=white
    ChessMove moves[] = {
        {"c2", "c4"}, // w
        {"c7", "c5"}, // b
        {"b1", "c3"}, // w
        {"d8", "a5"}, // b
        {"e2", "e3"}, // w
        {"a5", "c3"}, // b
        {"d2", "c3"}, // w 
    };

    ChessGame exp_game;
    initialize_game_soln(&exp_game);  
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);

    ChessGame act_game;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move(&act_game, &moves[i], (i + 1) % 2, 1);

    structs_equal(&exp_game, &act_game);   
}

TEST_F(make_move_complex_TestSuite, test04){
    INFO("Move empty piece on board");
    //https://lichess.org/editor/rnb1kbnr/pp1ppppp/8/q1p5/2P5/2N5/PP1PPPPP/R1BQKBNR_w_KQkq_-_0_1?color=white
    ChessMove moves[] = {
        {"c2", "c4"}, // w
        {"c7", "c5"}, // b
        {"b1", "c3"}, // w
        {"d8", "a5"}, // b
        {"b1", "c2"}, // moving empty piece
    };

    ChessGame exp_game;
    int exp_return;
    initialize_game_soln(&exp_game);  
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        exp_return = make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);
    // the last exp_return should be moving an empty piece

    ChessGame act_game;
    int act_return;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        act_return = make_move(&act_game, &moves[i], (i + 1) % 2, 1);
    
    EXPECT_EQ(exp_return, act_return);
    structs_equal(&exp_game, &act_game);   
}

TEST_F(make_move_complex_TestSuite, test05){
    INFO("Play a lot of turns and take multiple pieces");
    //https://lichess.org/editor/rn2kb1r/p2bp2p/3p1p2/qPp3p1/4P1P1/1QNB3N/PP1P3P/R1B1K2R_w_KQkq_-_0_1?color=white
    ChessMove moves[] = {
        {"c2", "c4"}, // w
        {"c7", "c5"}, // b
        {"b1", "c3"}, // w
        {"d8", "a5"}, // b
        {"e2", "e4"}, // w
        {"d7", "d6"}, // b
        {"f2", "f3"}, // w
        {"c8", "c7"}, // b
        {"f1", "d3"}, // w
        {"g7", "g5"}, // b
        {"g2", "g4"}, // w
        {"f7", "f6"}, // b
        {"g3", "h3"}, // w
        {"g8", "h6"}, // b
        {"d1", "b3"}, // w
        {"b7", "b5"}, // b
        {"c4", "b5"}, // w capture black pawn
        {"h6", "g4"}, // b capture white pawn
        {"f3", "g4"}, // w capture black knight
    };

    ChessGame exp_game;
    initialize_game_soln(&exp_game);  
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);
    // the last exp_return should be moving an empty piece

    ChessGame act_game;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move(&act_game, &moves[i], (i + 1) % 2, 1);
    
    structs_equal(&exp_game, &act_game);   
}

TEST_F(make_move_complex_TestSuite, test06){
    INFO("Only knights move and take pieces on the other side");
    //https://lichess.org/editor/r1bqk1nr/ppp1pppN/8/8/8/8/nPPP1PPP/RN1QKB1R_w_KQkq_-_0_1?color=white
    ChessMove moves[] = {
        {"g1", "f3"}, // w
        {"b8", "c6"}, // b
        {"f3", "e5"}, // w
        {"c6", "d4"}, // b
        {"e5", "d7"}, // w
        {"d4", "e2"}, // b
        {"d7", "f8"}, // w
        {"e2", "c1"}, // b
        {"f8", "h7"}, // w
        {"c1", "a2"}, // b
    };

    ChessGame exp_game;
    initialize_game_soln(&exp_game);  
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move_soln(&exp_game, &moves[i], (i + 1) % 2, 1);
    // the last exp_return should be moving an empty piece

    ChessGame act_game;
    initialize_game(&act_game);
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++)
        make_move(&act_game, &moves[i], (i + 1) % 2, 1);
    
    structs_equal(&exp_game, &act_game);   
}