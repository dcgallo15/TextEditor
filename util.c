#include "util.h"

void removeElemFromStaticArray(char* arr, size_t arrLen, size_t index) {
    for (size_t i = index; i < arrLen; i++) {
        arr[i] = arr[i + 1];
    }
    arr[arrLen - 1] = '\0';
}

void insertIntoStaticArray(char* arr, size_t arrLen, size_t index, char* data, size_t dataLen) {
    // TODO: implement some error checking here
    memmove(arr + index + dataLen, arr + index, arrLen);
    // TODO: maybe replace with a `memcpy`
    for (size_t i = 0; i < dataLen; i++) {
        arr[index + i] = data[i];
    }
}
