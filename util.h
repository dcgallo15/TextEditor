#ifndef __UTIL__H
#define __UTIL__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeElemFromStaticArray(char* arr, size_t arrLen, size_t index);
void insertIntoStaticArray(char* arr, size_t arrLen, size_t index, char* data, size_t dataLen);

#endif