#include "test_utils.h"
#include <fstream>

extern "C" {
#include "hw4.h"
}

int make_move_soln(ChessGame *game, ChessMove *move, bool is_client, bool validate_move);
void generate_solution_file(const std::string& test_name, const std::string& solution_content);
void initialize_game_soln(ChessGame *game);
void fen_to_chessboard_soln(const char *fen, ChessGame *game);
int receive_command_soln(ChessGame *game, const char *message, int socketfd, bool is_client);

class send_receive_TestSuite : public testing::Test { 
protected:
    void SetUp() override {
        mkdir("./tests/expected_outputs/", 0700);
        mkdir("./tests/actual_outputs/", 0700);
    }

    void TearDown() override {
        clear_file("game_database.txt");
    }
};


TEST_F(send_receive_TestSuite, send_command01) {
    int fd = open("./tests/actual_outputs/send_command01.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);

    int res = send_command(&game, "/move e2e4", fd, 1);
    // write return value to file
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));

    close(fd);

    // generate the solution file
    std::string solution_content = "1001\n";
    generate_solution_file("send_command01", solution_content);

    // compare the actual output with the solution
    std::string actual_output = load_file("./tests/actual_outputs/send_command01.txt");
    std::string expected_output = load_file("./tests/expected_outputs/send_command01.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, send_command02) {
    int fd = open("./tests/actual_outputs/send_command02.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);
    int res = send_command(&game, "/forfeit", fd, 1);
    
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);
    
    std::string solution_content = "1002\n";
    generate_solution_file("send_command02", solution_content);
    
    std::string actual_output = load_file("./tests/actual_outputs/send_command02.txt");
    std::string expected_output = load_file("./tests/expected_outputs/send_command02.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, send_command03) {
    int fd = open("./tests/actual_outputs/send_command03.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);

    int res = send_command(&game, "/save user1", fd, 1);
    
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);

    std::string solution_content = "1008\n";
    generate_solution_file("send_command03", solution_content);
    
    std::string actual_output = load_file("./tests/actual_outputs/send_command03.txt");
    std::string expected_output = load_file("./tests/expected_outputs/send_command03.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, send_command04) {
    int fd = open("./tests/actual_outputs/send_command04.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);

    // Save the game first
    int save_res = send_command(&game, "/save user1", fd, 1);
    ASSERT_EQ(COMMAND_SAVE, save_res);

    // Now try to load the game
    int res = send_command(&game, "/load user1 1", fd, 1);
    
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);
    
    std::string solution_content = "1007\n";
    generate_solution_file("send_command04", solution_content);
    
    std::string actual_output = load_file("./tests/actual_outputs/send_command04.txt");
    std::string expected_output = load_file("./tests/expected_outputs/send_command04.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, send_command05) {
    int fd = open("./tests/actual_outputs/send_command05.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);
    int res = send_command(&game, "/invalid_command", fd, 1);
    
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);
    
    std::string solution_content = "2001\n";
    generate_solution_file("send_command05", solution_content);
    
    std::string actual_output = load_file("./tests/actual_outputs/send_command05.txt");
    std::string expected_output = load_file("./tests/expected_outputs/send_command05.txt");
    EXPECT_EQ(expected_output, actual_output);
}


TEST_F(send_receive_TestSuite, receive_command01) {
    std::string actual_output_filename = "./tests/actual_outputs/receive_command01.txt";
    int fd = open(actual_output_filename.c_str(), O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);

    ChessMove moves[] = {
        {"e2", "e4"},
        {"e7", "e5"},
        {"g1", "f3"},
        {"b8", "c6"}
    };

    for (int i = 0; i < 4; i++) {
        make_move(&game, &moves[i], (i + 1) % 2, 1);
    }

    // use our own make_move_soln to make the move to check if the boards are the same
    ChessGame game_soln;
    ChessMove next = {"f3", "g5"};
    initialize_game_soln(&game_soln);
    
    for (int i = 0; i < 4; i++) {
        make_move_soln(&game_soln, &moves[i], (i + 1) % 2, 1);
    }

    make_move_soln(&game_soln, &next, 1, 1);

    int res = receive_command(&game, "/move f3g5", fd, 0); // this is us artifically making the move for the opponent to receive
    display_chessboard(&game);
    ASSERT_EQ(COMMAND_MOVE, res);
    // write return value to file
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));

    // check if the boards are the same
    bool boards_equal = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.chessboard[i][j] != game_soln.chessboard[i][j]) {
                boards_equal = false;
                break;
            }
        }
    }

    if (!boards_equal) {
        INFO("Boards are not equal");
        display_chessboard(&game);
        display_chessboard(&game_soln);
    }

    ASSERT_TRUE(boards_equal);

    close(fd);

    // Generate the solution file
    std::string solution_content = "1001\n"; // COMMAND_MOVE
    generate_solution_file("receive_command01", solution_content);

    // Compare the actual output with the solution
    std::string actual_output = load_file(actual_output_filename.c_str());
    std::string expected_output = load_file("./tests/expected_outputs/receive_command01.txt");
    EXPECT_EQ(expected_output, actual_output);
}

void write_result_to_file(int fd, int result) {
    char buffer[1024];
    sprintf(buffer, "%d\n", result);
    lseek(fd, 0, SEEK_END); // Move the file position to the end of the file
    write(fd, buffer, strlen(buffer));
}

TEST_F(send_receive_TestSuite, receive_command02) {
    int fd = open("./tests/actual_outputs/receive_command02.txt", O_CREAT|O_TRUNC|O_RDWR, 0644);
    ChessGame game;
    initialize_game_soln(&game);
    int res;
    std::string output;

    // Test valid move command
    res = receive_command(&game, "/move e2e4", fd, 0);
    output += std::to_string(res) + "\n";
    ASSERT_EQ(COMMAND_MOVE, res);

    initialize_game_soln(&game);

    // Test import command (only for client)
    res = receive_command(&game, "/import rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b", fd, 1);
    output += std::to_string(res) + "\n";
    ASSERT_EQ(COMMAND_IMPORT, res);

    // Test load command with valid input

    send_command(&game, "/save user1", fd, 1);
    res = receive_command(&game, "/load user1 1", fd, 0);
    output += std::to_string(res) + "\n";
    ASSERT_EQ(COMMAND_LOAD, res);

    res = receive_command(&game, "/load user1", fd, 0);
    output += std::to_string(res) + "\n";
    ASSERT_EQ(COMMAND_ERROR, res);

    res = receive_command(&game, "/invalid", fd, 0);
    output += std::to_string(res) + "\n";
    ASSERT_EQ(-1, res);

    // Write the entire output string to the file
    write(fd, output.c_str(), output.length());
    close(fd);

    // Generate the solution file
    std::string solution_content = "1001\n1006\n1007\n-1\n-1\n";
    generate_solution_file("receive_command02", solution_content);

    // Compare the actual output with the solution
    std::string actual_output = load_file("./tests/actual_outputs/receive_command02.txt");
    std::string expected_output = load_file("./tests/expected_outputs/receive_command02.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, receive_command03) {
    std::string actual_output_filename = "./tests/actual_outputs/receive_command03.txt";
    int fd = open(actual_output_filename.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
    ChessGame game;
    initialize_game(&game);

    int res = receive_command(&game, "/forfeit", fd, 0);
    ASSERT_EQ(COMMAND_FORFEIT, res);

    // Reopen the file descriptor
    close(fd);
    fd = open(actual_output_filename.c_str(), O_WRONLY | O_APPEND);

    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);

    std::string solution_content = "1002\n";
    generate_solution_file("receive_command03", solution_content);

    std::string actual_output = load_file(actual_output_filename.c_str());
    std::string expected_output = load_file("./tests/expected_outputs/receive_command03.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, receive_command04) {
    std::string actual_output_filename = "./tests/actual_outputs/receive_command04.txt";
    int fd = open(actual_output_filename.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
    ChessGame actual;
    ChessGame expected;
    initialize_game(&actual);
    initialize_game(&expected);

    int res = receive_command(&actual, "/import r2n3r/p2k4/3p4/1PpnbQpp/1pPKqbPp/3B4/3B2PP/R2P2NR w", fd, 1);
    ASSERT_EQ(COMMAND_IMPORT, res);
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);

    receive_command_soln(&expected, "/import r2n3r/p2k4/3p4/1PpnbQpp/1pPKqbPp/3B4/3B2PP/R2P2NR w", fd, 1);

    // check if board is set up correctly.
    structs_equal(&expected, &actual);

    std::string solution_content = "1006\n";
    generate_solution_file("receive_command04", solution_content);

    std::string actual_output = load_file(actual_output_filename.c_str());
    std::string expected_output = load_file("./tests/expected_outputs/receive_command04.txt");
    EXPECT_EQ(expected_output, actual_output);
}


TEST_F(send_receive_TestSuite, receive_command05) {
    std::string actual_output_filename = "./tests/actual_outputs/receive_command05.txt";
    int fd = open(actual_output_filename.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
    ChessGame actual;
    ChessGame expected;
    initialize_game(&actual);
    initialize_game(&expected);

    // only client can receive import
    int res = receive_command(&actual, "/import r2n3r/p2k4/3p4/1PpnbQpp/1pPKqbPp/3B4/3B2PP/R2P2NR w", fd, 0);
    ASSERT_EQ(-1, res);
    char buffer[1024];
    sprintf(buffer, "%d\n", res);
    write(fd, buffer, strlen(buffer));
    close(fd);

    receive_command_soln(&expected, "/import r2n3r/p2k4/3p4/1PpnbQpp/1pPKqbPp/3B4/3B2PP/R2P2NR w", fd, 0);

    // check if board is set up correctly.
    structs_equal(&expected, &actual);

    std::string solution_content = "-1\n";
    generate_solution_file("receive_command05", solution_content);

    std::string actual_output = load_file(actual_output_filename.c_str());
    std::string expected_output = load_file("./tests/expected_outputs/receive_command05.txt");
    EXPECT_EQ(expected_output, actual_output);
}

TEST_F(send_receive_TestSuite, send_and_receive01) {
    ChessGame game;
    initialize_game_soln(&game);
    int res;
    const char *command;
    int client_out, server_out;
    int client_in, server_in;

    // White
    game.currentPlayer = WHITE_PLAYER;
    client_out = open("/tmp/client_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    command = "/move a2a3";
    res = send_command(&game, command, client_out, true);
    close(client_out);
    ASSERT_EQ(COMMAND_MOVE, res);

    server_in = open("/tmp/client_out.txt", O_RDONLY, 0644);
    res = receive_command(&game, command, server_in, false);
    close(server_in);
    ASSERT_EQ(COMMAND_MOVE, res);

    // Black
    game.currentPlayer = BLACK_PLAYER;
    server_out = open("/tmp/server_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    command = "/move f7f4";
    res = send_command(&game, command, server_out, false);
    close(server_out);
    ASSERT_EQ(COMMAND_ERROR, res);

    // Black will try again
    game.currentPlayer = BLACK_PLAYER;
    server_out = open("/tmp/server_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    command = "/move f7f6";
    res = send_command(&game, command, server_out, false);
    close(server_out);
    ASSERT_EQ(COMMAND_MOVE, res);

    client_in = open("/tmp/server_out.txt", O_RDONLY, 0644);
    res = receive_command(&game, command, client_in, true);
    close(client_in);
    ASSERT_EQ(COMMAND_MOVE, res); 
}

TEST_F(send_receive_TestSuite, send_and_receive02) {
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
    ChessGame game;
    initialize_game_soln(&game);
    int client_out, server_out;
    int client_in, server_in;
    int res;
    char command[80];
    for (unsigned int i = 0; i < sizeof(moves)/sizeof(moves[0]); i++) {
        sprintf(command, "/move %s%s", moves[i].startSquare, moves[i].endSquare);
        if (i % 2 == WHITE_PLAYER) {
            game.currentPlayer = WHITE_PLAYER;
            client_out = open("/tmp/client_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            res = send_command(&game, command, client_out, true);
            close(client_out);
            ASSERT_EQ(COMMAND_MOVE, res);

            server_in = open("/tmp/client_out.txt", O_RDONLY, 0644);
            res = receive_command(&game, command, server_in, false);
            close(server_in);
            ASSERT_EQ(COMMAND_MOVE, res);
        }
        else {
            game.currentPlayer = BLACK_PLAYER;
            server_out = open("/tmp/server_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            res = send_command(&game, command, server_out, false);
            close(server_out);
            ASSERT_EQ(COMMAND_MOVE, res);

            client_in = open("/tmp/server_out.txt", O_RDONLY, 0644);
            res = receive_command(&game, command, client_in, true);
            close(client_in);
            ASSERT_EQ(COMMAND_MOVE, res); 
        }
    }
}
