enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION, ARRAY_END};

/* error codes for Section 3 */
#define ERROR_START_STATION_INVALID -1
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* print the tube map */
void print_map(char **m, int height, int width);

/* describe errors for Section 3 */
const char *error_description(int code);

/* converting string to Direction enum */
Direction string_to_direction(const char *token);

bool get_symbol_position(char** map, int height, int width, char target, int& row, int&col);

char get_symbol_for_station_or_line(const char* name);

int validate_route(char** map, int height, int width, const char* start_station, char* route, char* destination);
