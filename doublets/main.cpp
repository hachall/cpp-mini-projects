#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "doublets.h"

int main()
{
  cout << "============== Dictionary lookup: ==================" << endl << endl;

  bool found = dictionary_search("ADAPTABLE");
  cout << "The word 'ADAPTABLE' is " << (found ? "" : "NOT") << "found in the dictionary." << endl << endl;

  found = dictionary_search("JUMBLEWOCK");
  cout << "The word 'JUMBLEWOCK' is " << (found ? "" : "NOT ") << "found in the dictionary." << endl << endl;

  found = dictionary_search("WHEAT");
  cout << "The word 'WHEAT' is " << (found ? "" : "NOT ") << "found in the dictionary." << endl << endl;

  cout << "====================== Section 1 ======================" << endl << endl;

  bool valid = valid_step("WHEAT", "CHEAT");
  cout << "From 'WHEAT' to 'CHEAT' is " << (valid ? "" : "NOT ") << "a valid step." << endl << endl;

  valid = valid_step("WHEAT", "WHEAD");
  cout << "From 'WHEAT' to 'WHEAD' is " << (valid ? "" : "NOT ") << "a valid step." << endl << endl;

  valid = valid_step("WHEAT", "TEA");
  cout << "From 'WHEAT' to 'TEA' is " << (valid ? "" : "NOT ") << "a valid step." << endl << endl;

  valid = valid_step("WHEAT", "CHEAP");
  cout << "From 'WHEAT' to 'CHEAP' is " << (valid ? "" : "NOT ") << "a valid step." << endl << endl;

  valid = valid_step("TEA", "SEA");
  cout << "From 'TEA' to 'SEA' is " << (valid ? "" : "NOT ") << "a valid step." << endl << endl;

  cout << "====================== Section 2 ======================" << endl << endl;

  const char *wheat_chain[] = { "WHEAT", "CHEAT", "CHEAP", "CHEEP", "CREEP", "CREED", "BREED", "BREAD", NULL };

  cout << "Displaying 7-step chain from 'WHEAT' to 'BREAD':" << endl;
  bool success = display_chain(wheat_chain, cout);
  cout << "Output " << (success ? "successful" : "failed") << "!" << endl;
  cout << endl;

  const char *tea_chain[] = {"TEA", "SEA", "SET", "SOT", "HOT", NULL };

  cout << "Writing 4-step chain from 'TEA' to 'HOT' to file 'chain.txt'..." << endl;
  ofstream out("chain.txt");
  success = display_chain(tea_chain, out);
  cout << "Output " << (success ? "successful" : "failed") << "!" << endl;
  out.close();

  cout << "====================== Section 3 ======================" << endl << endl;

  valid = valid_chain(wheat_chain);
  cout << "The chain from 'WHEAT' to 'BREAD' is " << (valid ? "" : "NOT ") << "a valid chain." << endl << endl;

  const char *repeat_chain[] = { "WHEAT", "CHEAP", "WHEAT", "CHEAP", NULL };

  valid = valid_chain(repeat_chain);
  cout << "The chain 'WHEAT->CHEAP->WHEAT->CHEAP' is " << (valid ? "" : "NOT ") << "a valid chain." << endl << endl;

  const char *short_chain[] = { "WHEAT", NULL };
  valid = valid_chain(short_chain);
  cout << "The chain 'WHEAT' is " << (valid ? "" : "NOT ") << "a valid chain." << endl << endl;

  cout << "====================== Section 4 ======================" << endl << endl;

  const char *answer[100];

  /* can we go from WET to DRY in up to 4 steps */
  success = find_chain("WET", "DRY", answer, 4);
  cout << (success ? "Found" : "Could not find") << " a chain from 'WET' to 'DRY' with up to 4 steps"<< endl;

  if (success)
    display_chain(answer, cout);
  cout << endl;

  /* can we turn a HARD problem into an EASY one in up to 5 steps */
  success = find_chain("HARD", "EASY", answer, 5);
  cout << (success ? "Found" : "Could not find") << " a chain from 'HARD' to 'EASY' with up to 5 steps"<< endl;

  if (success)
    display_chain(answer, cout);
  cout << endl;

  /* can we evolve APE into MAN in up to 6 steps? */
  success = find_chain("APE", "MAN", answer, 6);
  cout << (success ? "Found" : "Could not find") << " a chain from 'APE' to 'MAN' with up to 6 steps"<< endl;

  if (success)
    display_chain(answer, cout);
  cout << endl;

  /* can we turn BLACK into WHITE in up to 8 steps? */
  success = find_chain("BLACK", "WHITE", answer, 8);
  cout << (success ? "Found" : "Could not find") << " a chain from 'BLACK' to 'WHITE' with up to 8 steps"<< endl;
  if (success)
    display_chain(answer, cout);
  cout << endl;

  /* can we make KETTLE into HOLDER in up to 10 steps? */
  success = find_chain("KETTLE", "HOLDER", answer, 10);
  cout << (success ? "Found" : "Could not find") << " a chain from 'KETTLE' to 'HOLDER' with up to 10 steps"<< endl;
  if (success)
    display_chain(answer, cout);
  cout << endl;

  // can we turn the WITCH into a FAIRY in up to 12 steps?
  success = find_chain("WITCH", "FAIRY", answer, 12);
  cout << (success ? "Found" : "Could not find") << " a chain from 'WITCH' to 'FAIRY' with up to 12 steps"<< endl;
  if (success)
    display_chain(answer, cout);
  cout << endl;


  return 0;
}
