#ifndef FCIPHER_H
#define FCIPHER_H

#include "const.h"

int fEncode(unsigned short mode);

int fDecode(unsigned short mode);

void createFMKey();

char toCipherText(char *morseBuffer);

void toPlainText(char *morseBuffer);

int isFMEquals(const char *char1, const char *char2);

int matchesWithFMKey(const char *alphabet);

#endif
