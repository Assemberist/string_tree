#ifndef HIDE_LIB_H
#define HIDE_LIB_H

#include <stddef.h>
#include <stdint.h>

size_t strdif(char* str1, char* str2);

#define HAVE_VALUE  2
#define HAVE_NEXT   1

int haveNext(uint8_t* ptr, size_t pos);
int haveValue(uint8_t* ptr, size_t pos);
void setFlags(uint8_t* ptr, size_t pos, uint8_t val);

#endif