#include "test_utils.h"

char *load_file(const char *filename) {
	char *buffer = 0;
	long length;
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		ERROR("Could not open file %s", filename);
		return NULL;
	}
	(void)fseek(file, 0, SEEK_END);
	length = ftell(file);
	(void)fseek(file, 0, SEEK_SET);
	buffer = (char *)malloc(length + 1);
	if (buffer != NULL) {
		(void)fread(buffer, 1, length, file);
		(void)fclose(file);
		buffer[length] = '\0';
	}
	return buffer;
}

void clear_file(const char *filename) {
    std::ofstream file(filename, std::ios::trunc);
    file.close();
}

void expect_no_valgrind_errors(int status) {
    EXPECT_EQ(0, WEXITSTATUS(status));
}

void expect_no_asan_errors(int status) {
    EXPECT_EQ(0, WEXITSTATUS(status));
}

int run_using_valgrind(const char *test_name) {
    char executable[100];
    (void)sprintf(executable, "./build/%s", test_name);
    assert(access(executable, F_OK) == 0);
    char cmd[500];
    (void)sprintf(cmd, "valgrind --quiet -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes --error-exitcode=37 %s",
	    executable);
	return system(cmd);
}

int run_using_asan(const char *test_name) {
	    char executable[100];
    (void)sprintf(executable, "./build/%s", test_name);
    assert(access(executable, F_OK) == 0);
    char cmd[500];
    (void)sprintf(cmd, "%s", executable);
	return system(cmd);
}


void generate_solution_file(const std::string& test_name, const std::string& solution_content) {
    std::string solution_filename = "./tests/expected_outputs/" + test_name + ".txt";
    std::ofstream solution_file(solution_filename);
    solution_file << solution_content;
    solution_file.close();
}

void structs_equal(ChessGame *expected, ChessGame *actual) {
	EXPECT_EQ(expected->moveCount, actual->moveCount);
	EXPECT_EQ(expected->currentPlayer, actual->currentPlayer);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			EXPECT_EQ(expected->chessboard[i][j], actual->chessboard[i][j]) << "Mismatch at " << i << ", " << j;
		}
	}
}