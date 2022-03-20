#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }

  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */

int count_words(const char* line) {
  int count = 0;
  bool word_switch = true;

  for (int i = 0 ; line[i] != '\0' ; i++) {

    if (line[i] != ' ' && word_switch) {
      count++;
      word_switch = false;
    }

    if (line[i] == ' ') {
      word_switch = true;
    }

  }

  return count;
};

bool find_phonetic_ending(const char* word, char* phonetic_ending) {
  ifstream in_stream;
  in_stream.open("dictionary.txt");

  char* line = new char[512];
  char* dict_word = new char[256];

  //parse dictionary lines
  in_stream.getline(line, 512);
  while(!in_stream.eof()) {
    //extract a dictionary word ffrom beginning of a line
    int i;
    for (i = 0 ; line[i] != ' ' ; i++ ) {
      dict_word[i] = line[i];
    }
    dict_word[i] = '\0';

    if (!strcmp(dict_word, word)) {

      bool seen_vowel = false;

      int j;
      //get to end of line
      for ( j = 0 ; line[j] != '\0' ; j++);
      //traverse backwards and stop at first space, AFTER seeing a vowel
      for ( j -= 1 ; j > i ; j--) {
        if (line[j] == 'A' || line[j] == 'E' || line[j] == 'I' ||
          line[j] == 'O' || line[j] == 'U' ) {
            seen_vowel = true;
          }

          if (line[j] == ' ' && seen_vowel) {
            //use pointer arithemtic to copy from this point
            strcpy(phonetic_ending, line + j);
            break;
          }

      }

      //parse out all spaces
      for (int k = 0 ; phonetic_ending[k] != '\0' ; k++) {
        if (phonetic_ending[k] == ' ') {
          for (int l = k ; phonetic_ending[l] != '\0' ; l++) {
            phonetic_ending[l] = phonetic_ending[l+1];
          }
        }
      }

      in_stream.close();
      return true;
    }

    in_stream.getline(line, 512);
  }


  in_stream.close();
  return false;

};


bool find_rhyme_scheme(const char* filename, char* scheme) {
  ifstream in_stream;
  in_stream.open(filename);
  if (in_stream.fail()) return false;
  rhyming_letter(RESET);

  char line[512];
  char word[100];
  char phonetic_ending[100];
  int line_count = 0;

  in_stream.getline(line, 512);
  while (!in_stream.eof()) {
    int num_words = count_words(line);
    bool success = get_word(line, num_words , word);
    if (!success) return false;
    find_phonetic_ending(word, phonetic_ending);
    char letter = rhyming_letter(phonetic_ending);
    // letter[1] = '\0';
    scheme[line_count] = letter;
    line_count++;
    in_stream.getline(line, 512);
  }


  scheme[line_count] = '\0';
  return true;


};


char* identify_sonnet(const char* filename) {
  char scheme[100];
  char* answer = new char[50];
  if (find_rhyme_scheme(filename, scheme)) {
    if (!strcmp(scheme, "ababcdcdefefgg")) {
      strcpy(answer,"Shakesperean");
    } else if (!strcmp(scheme, "abbaabbacdcdcd")) {
      strcpy(answer,"Petrarchan");
    } else if (!strcmp(scheme, "ababbcbccdcdee")) {
      strcpy(answer,"Spenserian");
    } else {
      strcpy(answer,"Unknown");
    }
  } else {
    strcpy(answer,"Unknown");
  }

  return answer;
};
