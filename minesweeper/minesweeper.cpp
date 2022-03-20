#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++)
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++)
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++)
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */

bool is_complete(const char mines[][9], char revealed[][9]) {

  for (int row = 0 ; row < 9 ; row++) {
    for (int col = 0 ; col < 9 ; col++) {
      if (mines[row][col] != '*' && revealed[row][col] == '?') return false;

    }
  }

  return true;
};

int count_mines(const char position[2], const char mines[][9]) {
  int row = position[0] - 'A';
  int col = position[1] - '1';

  int mine_count = 0;

  for (int i = row - 1 ; i <= row +1 ; i++) {
    for (int j = col - 1 ; j <= col +1 ; j++) {
      if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;
      if (mines[i][j] == '*') mine_count++;
    }
  }

  return mine_count;
};

MoveResult make_move(const char position[], const char mines[][9], char revealed[][9]) {

  unsigned int length = strlen(position);

  //INITIAL CHECKS
  if (position[0] > 'I' || position[0] < 'A') return INVALID_MOVE;
  if (position[1] > '9' || position[1] < '1') return INVALID_MOVE;
  if (length == 3 && position[2] != '*') return INVALID_MOVE;

  int row = position[0] - 'A';
  int col = position[1] - '1';

  if (revealed[row][col] != '?') return REDUNDANT_MOVE;

  if (length < 3 && mines[row][col] == '*') return BLOWN_UP;

  //BOARD PLACEMENTS

  //flagging a mine
  if (length == 3) {
    revealed[row][col] = '*';
    return (is_complete(mines, revealed)) ? SOLVED_BOARD : VALID_MOVE;
  }

  int count = 0;
  if ((count = count_mines(position, mines)) > 0) {
    // char ch;
    revealed[row][col] = '0' + count;
  } else {
    uncover(position, mines, revealed);
  }

  return (is_complete(mines, revealed)) ? SOLVED_BOARD : VALID_MOVE;

};

void uncover(const char position[2], const char mines[][9], char revealed[][9]) {
  int row = position[0] - 'A';
  int col = position[1] - '1';

  revealed[row][col] = ' ';

  for (int i = row - 1 ; i <= row +1 ; i++) {
    for (int j = col - 1 ; j <= col +1 ; j++) {
      
      if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;

      if (revealed[i][j] == '?') {
        int count = 0;
        char pos[2];
        pos[0] = i + 'A';
        pos[1] = j + '1';
        if ((count = count_mines(pos, mines)) > 0) {
          revealed[i][j] = '0' + count;
        } else {
          uncover(pos, mines, revealed);
        }
      }

    }
  }


};

bool find_safe_move(char revealed[][9], char move []) {

  for (int row = 0 ; row < 9 ; row++) {
    for (int col = 0 ; col < 9 ; col++) {

      if (isdigit(revealed[row][col])) { //check if bombs can be flagged around
        int count = revealed[row][col] - '0';
        int num_qs = 0;
        for (int i = row - 1 ; i <= row +1 ; i++) {
          for (int j = col - 1 ; j <= col +1 ; j++) {
            if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;
            if (revealed[i][j] == '?' || revealed[i][j] == '*') num_qs++;
          }
        }

        if (count == num_qs) {
          for (int i = row - 1 ; i <= row +1 ; i++) {
            for (int j = col - 1 ; j <= col +1 ; j++) {
              if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;
              if (revealed[i][j] == '?') {
                move[0] = i + 'A';
                move[1] = j + '1';
                move[2] = '*';
                return true;
              }
            }
          }
        }

      } else if (revealed[row][col] == '?') { //check to see if surroundings are satsfied, and can be uncovered
        bool all_satisfied = true;
        for (int i = row - 1 ; i <= row +1 ; i++) {
          for (int j = col - 1 ; j <= col +1 ; j++) {
            if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;
            if (isdigit(revealed[i][j])) {
              if (!check_satisfied(revealed, i, j, revealed[i][j] - '0')) {
                all_satisfied = false;
              }
            }
          }
        }
        if (all_satisfied) {
          move[0] = row + 'A';
          move[1] = col + '1';
          move[2] = NULL;
          return true;
        }
      } // otherwsie skip to next square

    }
  }


  return false;
};

bool check_satisfied(char revealed[][9], int row, int col, int count) {
  int num_mines = 0;
  for (int i = row - 1 ; i <= row +1 ; i++) {
    for (int j = col - 1 ; j <= col +1 ; j++) {
      if ( i < 0 || i > 8 || j < 0 || j > 8 || (i == row && j == col)) continue;
      if (revealed[i][j] == '*') num_mines++;
    }
  }
  return num_mines == count;
};
