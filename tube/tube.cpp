#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];

  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);

  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);

  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }

  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0)
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";
    for (int c=0; c<width; c++)
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID:
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n]))
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


bool get_symbol_position(char** map, int height, int width, char target, int& row, int&col) {

  for (int i = 0 ; i < height ; i++) {
    for (int j = 0 ; j < width ; j++) {
      if(map[i][j] == target) {
        row = i;
        col = j;
        return true;
      }

    }
  }

  row = -1;
  col = -1;
  return false;
};

char get_symbol_for_station_or_line(const char* name) {

  ifstream lines_in;
  lines_in.open("lines.txt");

  char* line = new char[256];
  char symbol;
  char* text = new char[256];

  lines_in.getline(line, 256);
  while (!lines_in.eof()) {

    symbol = line[0];
    text = line + 2;

    if (!strcmp(text, name)) {
      lines_in.close();
      return symbol;
    }

    lines_in.getline(line, 256);
  }
  lines_in.close();

  ifstream stations_in;
  stations_in.open("stations.txt");

  stations_in.getline(line, 256);
  while(!stations_in.eof()) {
    symbol = line[0];
    text = line + 2;

    if (!strcmp(text, name)) {
      stations_in.close();
      return symbol;
    }

    stations_in.getline(line, 256);
  }
  stations_in.close();

  return ' ';
};

int validate_route(char** map, int height, int width, const char* start_station, char* route, char* destination) {


  char symbol = get_symbol_for_station_or_line(start_station);
  if (!isalnum(symbol)) return ERROR_START_STATION_INVALID;

  int row, col;
  if (!get_symbol_position(map, height, width, symbol, row, col)) return ERROR_START_STATION_INVALID;

  //traverse route string
  Direction enum_route[100];
  int route_index = 0;
  char move[3];
  int move_index = 0;
  for (int i = 0 ; route[i] != '\0' ; i++) {

    if (route[i] != ',' ) {
      move[move_index++] = route[i];
    }

    if (route[i] == ',') {
      move[move_index] = '\0';

      //do something with move
      if ((enum_route[route_index++] = string_to_direction(move)) == INVALID_DIRECTION) return ERROR_INVALID_DIRECTION;

      //reset move
      move_index = 0;

    }

  }
  //last move
  move[move_index] = '\0';
  enum_route[route_index++] = string_to_direction(move);

  //close off route
  enum_route[route_index] = ARRAY_END;

  //traverse route
  int lines_count = 0;
  for (int j = 0 ; j < route_index ; j++) {

    char curr_symbol = map[row][col];
    int backtrack = 0;

    switch (enum_route[j]) {
      case N: //north
        row -= 1; if (enum_route[j + 1] == S) backtrack++; break;
      case S: //south
        row += 1; if (enum_route[j + 1] == N) backtrack++; break;
      case E:
        col += 1; if (enum_route[j + 1] == W) backtrack++; break;
      case W:
        col -= 1; if (enum_route[j + 1] == E) backtrack++; break;
      case NE:
        row -= 1; col += 1; if (enum_route[j + 1] == SW) backtrack++; break;
      case SE:
        row += 1; col += 1; if (enum_route[j + 1] == NW) backtrack++; break;
      case SW:
        row += 1; col -= 1; if (enum_route[j + 1] == NE) backtrack++; break;
      case NW:
        row -= 1; col -= 1; if (enum_route[j + 1] == SW) backtrack++; break;
      case INVALID_DIRECTION:
      case ARRAY_END:
        return ERROR_INVALID_DIRECTION;
    }

    if (!isalnum(curr_symbol) && backtrack > 0) return ERROR_BACKTRACKING_BETWEEN_STATIONS;

    //going out of bounds
    if (row < 0 || row > height -1 || col < 0 || col > width - 1 ) return ERROR_OUT_OF_BOUNDS;
    //going off track
    if (map[row][col] == ' ') return ERROR_OFF_TRACK;
    //line hopping
    // cout << "going from " << curr_symbol << " to " << map[row][col] << endl;
    if (!isalnum(curr_symbol) && curr_symbol != map[row][col] && !isalnum(map[row][col])) return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
    //backtracking

    //line change
    if (isalnum(curr_symbol) && !isalnum(map[row][col])) lines_count++;
  }


  ifstream stations_in;
  stations_in.open("stations.txt");

  char* line = new char[256];
  stations_in.getline(line, 512);
  while (!stations_in.eof()) {
    if (line[0] == map[row][col]) {
      strcpy(destination, line + 2);
      delete [ ] line;
      return lines_count - 1;
    }
    stations_in.getline(line, 512);
  }

  delete [ ] line;
  return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
};
