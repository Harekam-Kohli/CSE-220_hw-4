#include "test_utils.h"
#include <fstream>
extern "C" {
#include "hw4.h"
}
int make_move_soln(ChessGame *game, ChessMove *move, int isClient, bool validateMove);

class save_load_game_testSuite : public testing::Test {
    void SetUp() override {
    }

    void TearDown() override {
        clear_file("game_database.txt");
    }
};

#define PORT 8080

// Testing:
/*
save_game - 3
load_game - 3
sync - 2
*/

TEST_F(save_load_game_testSuite, save_game_valid) {
    ChessGame game;
    initialize_game(&game);

    // Make some moves
    ChessMove move1 = {"e2", "e4"};
    make_move(&game, &move1, true, true);
    ChessMove move2 = {"e7", "e5"};
    make_move(&game, &move2, false, true);

    // Save the game
    const char *username = "testuser";
    const char *db_filename = "game_database.txt";
    int result = save_game(&game, username, db_filename);

    ASSERT_EQ(result, 0);

    // Check if the game was saved correctly
    std::ifstream file(db_filename);
    std::string line;
    std::getline(file, line);
    file.close();

    std::string expected_line = "testuser:rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w";
    ASSERT_EQ(line, expected_line);
}

TEST_F(save_load_game_testSuite, save_game_invalid_username) {
    ChessGame game;
    initialize_game(&game);

    // Save the game with an invalid username (containing spaces)
    const char *username = "test user";
    const char *db_filename = "game_database.txt";
    int result = save_game(&game, username, db_filename);

    ASSERT_EQ(result, -1);
}

TEST_F(save_load_game_testSuite, load_game_valid) {
    ChessGame game;
    initialize_game(&game);

    // Save a game
    const char *username = "testuser";
    const char *db_filename = "game_database.txt";
    save_game(&game, username, db_filename);

    ChessMove move1 = {"e2", "e4"};
    make_move(&game, &move1, true, true);
    ChessMove move2 = {"e7", "e5"};
    make_move(&game, &move2, false, true);

    // Save the game again
    save_game(&game, username, db_filename);

    // Load the second save file
    ChessGame loaded_game;
    int save_number = 2;
    int result = load_game(&loaded_game, username, db_filename, save_number);

    ASSERT_EQ(result, 0);

    // Check if the loaded game state matches the expected state
    char expected_board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', '.', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', 'p', '.', '.', '.'},
        {'.', '.', '.', '.', 'P', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', '.', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    display_chessboard(&loaded_game);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ASSERT_EQ(loaded_game.chessboard[i][j], expected_board[i][j]);
        }
    }
}

TEST_F(save_load_game_testSuite, load_game_invalid_username) {
    ChessGame game;
    initialize_game(&game);

    // Load a game with an invalid username
    const char *username = "invaliduser";
    const char *db_filename = "game_database.txt";
    int save_number = 1;
    int result = load_game(&game, username, db_filename, save_number);

    ASSERT_EQ(result, -1);
}

TEST_F(save_load_game_testSuite, load_game_invalid_save_number) {
    ChessGame game;
    initialize_game(&game);

    // Save a game
    const char *username = "testuser";
    const char *db_filename = "game_database.txt";
    save_game(&game, username, db_filename);

    // Load a game with an invalid save number
    int save_number = 100;
    int result = load_game(&game, username, db_filename, save_number);

    ASSERT_EQ(result, -1);
}
