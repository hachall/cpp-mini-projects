#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words.
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

bool valid_step(const char* current_word, const char* next_word) {

  unsigned int length = strlen(current_word);

  if (length != strlen(next_word)) {
    return false;
  }

  int diff_count = 0;
  for (unsigned int i = 0 ; i < length ; i++) {
    if (current_word[i] != next_word[i]) {
      diff_count++;
    }
  }

  return (diff_count == 1 && dictionary_search(next_word));

};

bool display_chain(const char* chain[], ostream& output_stream) {
  int length = strlen(chain[0]);

  output_stream << chain[0] << endl;
  int i;
  for (i = 1 ; chain[i + 1] != NULL ; i++) {
    for (int j = 0 ; j < length ; j++) {
      char ch;
      ch = tolower(chain[i][j]);
      output_stream << ch;
    }
    output_stream << endl;
  }
  output_stream << chain[i] << endl;

  return !output_stream.fail();
};

bool valid_chain(const char* chain[]) {
  //loop chain
  for (int i = 0; chain[i + 1] != NULL ; i++) {
    //check step is valid
    if (!valid_step(chain[i], chain[i + 1])) {
      return false;
    }

    //loop back and look for repeats
    for (int j = 0 ; j < i ; j++) {
      if (!strcmp(chain[i], chain[j])) {
        return false;
      }
    }

  }

  return true;
};

bool find_chain(const char* start_word, const char* target_word, const char* answer_chain[], int max_steps) {

  answer_chain[0] = start_word;
  answer_chain[1] = target_word;
  answer_chain[2] = NULL;


  return find_chain_recursive(answer_chain, max_steps);
};

bool find_chain_recursive(const char* answer_chain[], int max_steps, int step) {
  cout << "step: " << step << endl;
  display_chain(answer_chain, cout);
  //postive base case
  if (valid_chain(answer_chain)) return true;
  //backpropogate if chain gets too long
  if (step == max_steps - 1) return false;

  //open up space in answer_chain at step + 1
  answer_chain[step + 2] = answer_chain[step + 1];
  answer_chain[step + 3] = NULL;

  //attempt valid words in the space and propogate forward
  ifstream instream;
  instream.open("words.txt");
  char* word = new char[100];

  while(!instream.eof()) {
    instream >> word;
    if (valid_step(answer_chain[step], word)){
      answer_chain[step + 1] = word;
      if (find_chain_recursive(answer_chain, max_steps, step +1)) return true;
    }

  }
  //no word works for this position, propogate backwards and change higher up chain
  //need to remove word first
  instream.close();
  answer_chain[step + 1] = answer_chain[step + 2];
  answer_chain[step + 2] = NULL;
  return false;
};
