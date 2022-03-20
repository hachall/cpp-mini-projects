#include <cstring>
#include <iostream>
#include <cctype>
#include "playfair.h"

using namespace std;

void prepare(const char input[], char output[]) {
  int length = strlen(input);
  int j = 0;
  for (int i = 0 ; i < length ; i++) {
    char ch = input[i];
    if (isalnum(ch)) {

      if (ch >= 'a' && ch <= 'z') {
        //lowercase letter
        ch -= 32;
        output[j] = ch;
        j++;
      } else {
        //uppercase letter or number
        output[j] = ch;
        j++;
      }

    }
    //punctuation or space ignored
  }

  if (j % 2 == 1) {
    output[j] = 'X';
  }

};


void grid(const char codeword[], char grid[][6]) {
  //ascii bitmap
  bool used_map[90] {};

  int grid_pos = 0;
  char char_order[36] {};

  int code_len = strlen(codeword);

  //put in codeword
  for (int i = 0 ; i < code_len ; i++) {
    int ch = codeword[i];
    if (!used_map[ch]) {
      char_order[grid_pos] = ch;
      used_map[ch] = true;
      grid_pos++;
    }
  }

  //add rest of alphabet
  for (int i = 65 ; i <= 90 ; i++) {
    if (!used_map[i]) {
      char_order[grid_pos] = i;
      grid_pos++;
    }
  }

  //add numbers
  for (int i = 48 ; i <= 57 ; i++) {
    if (!used_map[i]) {
      char_order[grid_pos] = i;
      grid_pos++;
    }
  }

  //put into grid
  grid_pos = 0;
  for (int i = 0 ; i < 6 ; i++) {
    for (int j = 0 ; j < 6 ; j++) {
      grid[i][j] = char_order[grid_pos];
      grid_pos++;
    }
  }
};


void bigram(const char grid[][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2) {
  int char1_col, char1_row, char2_col, char2_row;

  for (int row = 0 ; row < 6 ; row++) {
    for (int col = 0 ; col < 6 ; col++) {
      if (grid[row][col] == inchar1) {
        char1_col = col;
        char1_row = row;
      } else if (grid[row][col] == inchar2) {
        char2_row = row;
        char2_col = col;
      }
    }
  }

  outchar1 = grid[char1_row][char2_col];
  outchar2 = grid[char2_row][char1_col];

};

void encode(const char grid[][6], const char prepared[], char encoded[]) {

  encode_rec(grid, prepared, encoded, 0);
};

void encode_rec(const char grid[][6], const char prepared[], char encoded[], unsigned int pos) {
  if (pos == strlen(prepared)) return;

  char char1, char2;
  bigram(grid, *(prepared + pos), *(prepared + pos + 1), char1, char2);
  encoded[pos] = char1;
  encoded[pos + 1] = char2;
  encode_rec(grid, prepared, encoded, pos + 2);

}
