enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

bool is_complete(const char mines[][9], char revealed[][9]);

int count_mines(const char position[2], const char mines[][9]);

MoveResult make_move(const char position[], const char mines[][9], char revealed[][9]);

void uncover(const char position[], const char mines[][9], char revealed[][9]);

bool find_safe_move(char revealed[][9], char move []);

bool check_satisfied(char revealed[][9], int row, int col, int count); 
