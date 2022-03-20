#ifndef PLAYFAIR_H
#define PLAYFAIR_H

void prepare(const char input[], char output[]);

void grid(const char codeword[], char grid[][6]);

void bigram(const char grid[][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2);

void encode(const char grid[][6], const char prepared[], char encoded[] );

void encode_rec(const char grid[][6], const char prepared[], char encoded[], unsigned int pos);

#endif
