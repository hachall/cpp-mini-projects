#include <iostream>
#include <fstream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

int leading_zeros(const char* digest){

  int zeroes = 0;
  bool leading_switch = true;

  for (int i = 0 ; digest[i] != '\0' ; i++) {

    if (digest[i] == '0' && leading_switch) {
      zeroes++;
    } else {

      leading_switch = false;

      if ((digest[i] > 'z' || digest[i] < 'a') && (digest[i] > '9' || digest[i] < '0')) {
        return -1;
      }


    }

  }

  return zeroes;
};


bool file_to_SHA1_digest(const char* filename, char* digest) {

  ifstream in_stream;
  in_stream.open(filename);
  if (in_stream.fail()) {
    strcpy(digest, "error");
    return false;
  };

  char* text = new char[100000];
  char ch;

  int index = 0;
  in_stream.get(ch);
  while(!in_stream.eof()) {
    text[index++] = ch;
    in_stream.get(ch);
  }

  text_to_SHA1_digest(text, digest);

  return true;
};

bool make_header(const char* recipient, const char* filename, char* header) {

  // char* header_base = new char[256] {};
  char header_base[256] = "";
  char digest[128];
  //turn file contents to digest, exit with false if failed
  if (!file_to_SHA1_digest(filename, digest)) return false;

  strcat(header_base, recipient);
  strcat(header_base, ":");
  strcat(header_base, digest);
  strcat(header_base, ":");

  char header_copy[256];
  char header_digest[256];
  strcpy(header_copy, header_base);

  char* integer_string = new char [100];
  int COUNTER_LIMIT = 10000000;
  for (int counter = 0 ; counter <= COUNTER_LIMIT ; counter++) {
    // add counter onto end of header
    sprintf(integer_string, "%d", counter);
    strcat(header_copy, integer_string);

    //hash header, and check eligibility
    text_to_SHA1_digest(header_copy, header_digest);
    if (leading_zeros(header_digest) == 5) {
      strcpy(header, header_copy);
      return true;
    }


    strcpy(header_copy, header_base);
  }

  return false;
};

MessageStatus check_header(const char* email_address, const char* header, const char* filename) {

  char email[256], msg_digest[41];
  int section = 0, index = 0;

  for (int i = 0 ; header[i] != '\0' ; i++) {
    if (header[i] == ':') {
      section++;
      index = 0;
    } else {
      switch (section) {
        case 0:
          email[index++] = header[i]; break;
        case 1:
          msg_digest[index++] = header[i]; break;
      }

    }

  }
  msg_digest[40] = '\0';

  //if not three sections seperated by ':', then invalid header
  if (section != 2) return INVALID_HEADER;

  //check if recipient is correct
  if (strcmp(email_address, email)) return WRONG_RECIPIENT;

  //check that message digest is correct
  char digest[128];
  file_to_SHA1_digest(filename, digest);
  if (strcmp(digest, msg_digest)) return INVALID_MESSAGE_DIGEST;

  //check if header digest is ok
  char header_digest[256];
  text_to_SHA1_digest(header, header_digest);
  if (leading_zeros(header_digest) != 5) return INVALID_HEADER_DIGEST;


  return VALID_EMAIL;
};
