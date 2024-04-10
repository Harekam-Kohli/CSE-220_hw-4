#ifndef HW4_H
#define HW4_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define INFO(...) fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr)

#define PORT 8080
#define BUFFER_SIZE 1024

#define MAX_MOVES 512
#define MAX_CAPTURED_PIECES 16

#define WHITE_PLAYER 0
#define BLACK_PLAYER 1

#define MOVE_SUS 2
#define MOVE_OUT_OF_TURN 3
#define MOVE_NOTHING 4
#define MOVE_WRONG_COLOR 5
#define MOVE_WRONG 6
#define MOVE_NOT_A_PAWN 7
#define MOVE_MISSING_PROMOTION 8

#define PARSE_MOVE_INVALID_FORMAT 20
#define PARSE_MOVE_INVALID_DESTINATION 21
#define PARSE_MOVE_OUT_OF_BOUNDS 22
#define PARSE_MOVE_INVALID_PROMOTION 23

#define COMMAND_MOVE 1001
#define COMMAND_FORFEIT 1002
#define COMMAND_GAME 1004
#define COMMAND_IMPORT 1006
#define COMMAND_LOAD 1007
#define COMMAND_SAVE 1008
#define COMMAND_DISPLAY 1011
#define COMMAND_UNKNOWN 2001
#define COMMAND_ERROR -1

typedef struct {
    char startSquare[3]; 
    char endSquare[4];
} ChessMove;

typedef struct {
    ChessMove moves[MAX_MOVES];
    char capturedPieces[MAX_CAPTURED_PIECES];
    int moveCount;
    int capturedCount;
    int currentPlayer;
    char chessboard[8][8];
} ChessGame;

// Required functions
void initialize_game(ChessGame *game);
void chessboard_to_fen(char fen[], ChessGame *game);
void fen_to_chessboard(const char *fen, ChessGame *game);
int parse_move(const char *str, ChessMove *move);
int send_command(ChessGame *game, const char *message, int socketfd, bool is_client);
int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client);
int save_game(ChessGame *game, const char *username, const char *db_filename);
int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number);
int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move);

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col);
bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);
bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col);
bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game);

void display_chessboard(ChessGame *game);

#endif
