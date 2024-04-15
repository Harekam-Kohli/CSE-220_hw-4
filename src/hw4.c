#include "hw4.h"

void initialize_game(ChessGame *game) {
    const char *init[8] = {
        "rnbqkbnr", "pppppppp", "........",
        "........", "........", "........",
        "PPPPPPPP", "RNBQKBNR"
    };

    for (int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            game->chessboard[i][j] = init[i][j];
        }
    }
    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = WHITE_PLAYER;
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int fen_index = 0;
    char buffer[20]; 
    int row = 0;

    while (row < 8) {
        int col = 0;
        int empty_count = 0;

        while (col < 8) {
            char piece = game->chessboard[row][col];
            if (piece == '.') {
                empty_count++; 
            } else {
                if (empty_count > 0) {
                    snprintf(buffer, sizeof(buffer), "%d", empty_count);
                    int buf_index = 0;
                    while (buffer[buf_index] != '\0') {
                        fen[fen_index++] = buffer[buf_index++];
                    }
                    empty_count = 0;
                }
                fen[fen_index++] = piece;
            }
            col++;
        }
        if (empty_count > 0) {
            snprintf(buffer, sizeof(buffer), "%d", empty_count);
            int buf_index = 0;
            while (buffer[buf_index] != '\0') {
                fen[fen_index++] = buffer[buf_index++];
            }
        }
        if (row < 7) {
            fen[fen_index++] = '/';
        }
        row++;
    }

    fen[fen_index++] = ' ';
    if (game->currentPlayer == WHITE_PLAYER) {
        fen[fen_index++] = 'w';
    } else {
        fen[fen_index++] = 'b';
    }
    fen[fen_index] = '\0';  
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int direction = isupper(piece) ? -1 : 1;  
    char opponentColor = isupper(piece) ? 'a' : 'A'; 

    if (dest_col == src_col && dest_row == src_row + direction) {
        return game->chessboard[dest_row][dest_col] == '.';
    }

    if (dest_col == src_col && (src_row == 1 || src_row == 6) && dest_row == src_row + 2 * direction) {
        return game->chessboard[src_row + direction][src_col] == '.' && game->chessboard[dest_row][dest_col] == '.';
    }

    if (abs(dest_col - src_col) == 1 && dest_row == src_row + direction) {
        return game->chessboard[dest_row][dest_col] >= opponentColor &&
               game->chessboard[dest_row][dest_col] < (opponentColor + 26);
    }

    if ((dest_row == 0 || dest_row == 7) && dest_col == src_col && 
        game->chessboard[dest_row][dest_col] == '.' && strlen(game->moves[game->moveCount].endSquare) == 4) {
        char promo = game->moves[game->moveCount].endSquare[3];  // Promotion piece
        return (tolower(promo) == 'q' || tolower(promo) == 'r' || tolower(promo) == 'b' || tolower(promo) == 'n');
    }

    return false;
}



bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row != dest_row && src_col != dest_col) {
        return false; 
    }

    int row_step = (dest_row > src_row) ? 1 : (dest_row < src_row) ? -1 : 0;
    int col_step = (dest_col > src_col) ? 1 : (dest_col < src_col) ? -1 : 0;

    int check_row = src_row + row_step;
    int check_col = src_col + col_step;
    while (check_row != dest_row || check_col != dest_col) {
        if (game->chessboard[check_row][check_col] != '.') {
            return false; 
        }
        check_row += row_step;
        check_col += col_step;
    }

    char dest_piece = game->chessboard[dest_row][dest_col];
    if (dest_piece != '.' && ((isupper(dest_piece) && isupper(game->chessboard[src_row][src_col])) || 
        (islower(dest_piece) && islower(game->chessboard[src_row][src_col])))) {
        return false; 
    }

    return true;
}


bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_diff = src_row - dest_row;
    int col_diff = src_col - dest_col;
    if (row_diff < 0) row_diff = -row_diff;
    if (col_diff < 0) col_diff = -col_diff;

    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int row_diff = dest_row - src_row;
    int col_diff = dest_col - src_col;

    if (row_diff * row_diff != col_diff * col_diff)
        return 0;

    int row_step = row_diff > 0 ? 1 : -1;
    int col_step = col_diff > 0 ? 1 : -1;

    for (int i = 1; i < row_diff * row_step; i++)
        if (game->chessboard[src_row + i * row_step][src_col + i * col_step] != '.')
            return 0;

    return 1;
}


bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game) || is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_diff = src_row > dest_row ? src_row - dest_row : dest_row - src_row;
    int col_diff = src_col > dest_col ? src_col - dest_col : dest_col - src_col;
    return (row_diff <= 1 && col_diff <= 1 && (row_diff || col_diff));
}


bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (piece == 'P' || piece == 'p') {
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
    } else if (piece == 'R' || piece == 'r') {
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
    } else if (piece == 'N' || piece == 'n') {
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
    } else if (piece == 'B' || piece == 'b') {
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
    } else if (piece == 'Q' || piece == 'q') {
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
    } else if (piece == 'K' || piece == 'k') {
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
    } else {
        return false;
    }
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int row = 0, col = 0;
    int i = 0;
    char ch;

    while ((ch = fen[i]) != '\0' && ch != ' ') {
        if (isdigit(ch)) {
            int empty_spaces = ch - '0';
            int j = 0;
            while (j < empty_spaces) {
                game->chessboard[row][col++] = '.';
                j++;
            }
        } else if (ch == '/') {
            row++;
            col = 0;
        } else {
            game->chessboard[row][col++] = ch;
        }
        i++;
    }

    if (fen[strlen(fen) - 1] == 'b') {
        game->currentPlayer = BLACK_PLAYER;
    } else {
        game->currentPlayer = WHITE_PLAYER;
    }
}

int parse_move(const char *str, ChessMove *move) {
    int len = strlen(str);
    if (len != 4 && len != 5)
        return PARSE_MOVE_INVALID_FORMAT;

    if (str[0] < 'a' || str[0] > 'h' || str[2] < 'a' || str[2] > 'h')
        return PARSE_MOVE_INVALID_FORMAT;

    if (str[1] < '1' || str[1] > '8' || str[3] < '1' || str[3] > '8')
        return PARSE_MOVE_INVALID_FORMAT;

    move->startSquare[0] = str[0];
    move->startSquare[1] = str[1];
    move->startSquare[2] = '\0';

    move->endSquare[0] = str[2];
    move->endSquare[1] = str[3];
    move->endSquare[2] = '\0';

    if (len == 5) {
        move->endSquare[2] = str[4]; 
        move->endSquare[3] = '\0';

        if (str[1] == '7' && str[3] == '8' && (str[4] == 'q' || str[4] == 'r' || str[4] == 'b' || str[4] == 'n'))
            return 0;  
        else if (str[1] == '2' && str[3] == '1' && (str[4] == 'q' || str[4] == 'r' || str[4] == 'b' || str[4] == 'n'))
            return 0;  
        else
            return PARSE_MOVE_INVALID_PROMOTION;
    }

    return 0;
}


int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    if (validate_move) {
        if (game->currentPlayer == WHITE_PLAYER && is_client == false) return MOVE_OUT_OF_TURN;
        if (game->currentPlayer == BLACK_PLAYER && is_client == true) return MOVE_OUT_OF_TURN;
        char piece = game->chessboard[move->startSquare[1] - '1'][move->startSquare[0] - 'a'];
        if (piece == '.') return MOVE_NOTHING;
        bool is_white_piece = piece >= 'A' && piece <= 'Z';
        if (is_white_piece && is_client == false) return MOVE_WRONG_COLOR;
        if (!is_white_piece && is_client == true) return MOVE_WRONG_COLOR;
        char target = game->chessboard[move->endSquare[1] - '1'][move->endSquare[0] - 'a'];
        if (target != '.' && ((target >= 'A' && target <= 'Z') == is_white_piece)) return MOVE_SUS;
        if (move->endSquare[2] && (piece != 'P' && piece != 'p')) return MOVE_NOT_A_PAWN;
        if (!move->endSquare[2] && ((move->endSquare[1] - '1' == 7 || move->endSquare[1] - '1' == 0) && (piece == 'P' || piece == 'p'))) return MOVE_MISSING_PROMOTION;
        if (!is_valid_move(piece, move->startSquare[1] - '1', move->startSquare[0] - 'a', move->endSquare[1] - '1', move->endSquare[0] - 'a', game)) return MOVE_WRONG;
    }
    game->chessboard[move->endSquare[1] - '1'][move->endSquare[0] - 'a'] = game->chessboard[move->startSquare[1] - '1'][move->startSquare[0] - 'a'];
    game->chessboard[move->startSquare[1] - '1'][move->startSquare[0] - 'a'] = '.';
    if (move->endSquare[2]) {
        char promotion = move->endSquare[3];
        game->chessboard[move->endSquare[1] - '1'][move->endSquare[0] - 'a'] = promotion;
    }
    game->currentPlayer = is_client ? BLACK_PLAYER : WHITE_PLAYER;
    game->moveCount++;
    return 0;
}


int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
     ChessMove move;

    if (strncmp(message, "/move ", 6) == 0) {
        if (parse_move(message + 6, &move) == 0 && make_move(game, &move, is_client, true) == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_MOVE;
        }
        return COMMAND_ERROR;
    } else if (strncmp(message, "/forfeit", 8) == 0) {
        send(socketfd, message, strlen(message), 0);
        return COMMAND_FORFEIT;
    } else if (strncmp(message, "/chessboard", 11) == 0) {
        display_chessboard(game);
        return COMMAND_DISPLAY;
    } else if (strncmp(message, "/import ", 8) == 0 && !is_client) {
        fen_to_chessboard(message + 8, game);
        send(socketfd, message, strlen(message), 0);
        return COMMAND_IMPORT;
    } else if (strncmp(message, "/load ", 6) == 0) {
        char username[100];
        int save_number;
        sscanf(message + 6, "%s %d", username, &save_number);
        if (load_game(game, username, "game_database.txt", save_number) == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_LOAD;
        }
        return COMMAND_ERROR;
    } else if (strncmp(message, "/save ", 6) == 0) {
        char username[100];
        sscanf(message + 6, "%s", username);
        if (save_game(game, username, "game_database.txt") == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_SAVE;
        }
        return COMMAND_ERROR;
    }

    return COMMAND_UNKNOWN;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    if (strncmp(message, "/move ", 6) == 0) {
        ChessMove move;
        if (parse_move(message + 6, &move) == 0 && make_move(game, &move, is_client, false) == 0) {
            return COMMAND_MOVE;
        }
        return COMMAND_ERROR;
    } else if (strncmp(message, "/forfeit", 8) == 0) {
        close(socketfd);
        return COMMAND_FORFEIT;
    } else if (strncmp(message, "/import ", 8) == 0 && is_client) {
        fen_to_chessboard(message + 8, game);
        return COMMAND_IMPORT;
    } else if (strncmp(message, "/load ", 6) == 0) {
        char username[100];
        int save_number;
        sscanf(message + 6, "%s %d", username, &save_number);
        if (load_game(game, username, "game_database.txt", save_number) == 0) {
            return COMMAND_LOAD;
        }
        return COMMAND_ERROR;
    }
    return COMMAND_UNKNOWN;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    if (strchr(username, ' ') != NULL) {
        fprintf(stderr, "Invalid username: usernames cannot contain spaces.\n");
        return -1;
    }

    FILE *db_file = fopen(db_filename, "a");
    if (!db_file) {
        perror("Failed to open database file");
        return -1;
    }

    char fen[100];
    chessboard_to_fen(fen, game);

    if (fprintf(db_file, "%s:%s\n", username, fen) < 0) {
        perror("Failed to write to database file");
        fclose(db_file);
        return -1;
    }

    fclose(db_file);
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    FILE *db;
    char line[1024];
    int count = 0;
    db = fopen(db_filename, "r");
    if (!db) return -1;

    while (fgets(line, sizeof(line), db)) {
        char *line_username = strtok(line, ":");
        if (strcmp(line_username, username) == 0) {
            ++count;
            if (count == save_number) {
                char *fen = strtok(NULL, "\n");
                fen_to_chessboard(fen, game);
                fclose(db);
                return 0;
            }
        }
    }
    fclose(db);
    return -1;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
