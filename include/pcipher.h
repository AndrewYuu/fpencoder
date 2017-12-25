#ifndef PCIPHER_H
#define PCIPHER_H

#include "const.h"

void createPTable(unsigned short mode);

int pEncode(unsigned short mode);

void pDecode(unsigned short mode);

void polybiusFind(int rowToSearch, int colToSearch, int rows, int cols);

void makeString(char *string, char toAppend);

int matchesWithKey(char *pTable);

#endif
