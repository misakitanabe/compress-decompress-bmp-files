#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

#define MAX_BITS 8

void writeCompressed(int *freq, BYTE *arr, char codes[][MAX_CODE_LENGTH], FILE *imgPtr, int dataSize) {
    writeFreq(freq, arr, imgPtr);
    writeBits(arr, codes, imgPtr, dataSize);
}

void writeBits(BYTE *arr, char codes[][MAX_CODE_LENGTH], FILE *imgPtr, int dataSize) {
    int i;
    BYTE tempByte = 0;
    int bitCount = 0;
    char code[MAX_CODE_LENGTH];
    BYTE colorVal = 0;

    for (i = 0; i < dataSize; i++) {
        int j;
        char currentChar;
        colorVal = arr[i];
        strcpy(code, codes[(int)colorVal]);

        /* traverse through each 0 or 1 in the code */
        for (j = 0; j < strlen(code); j++) {
            currentChar = code[j];

            /* if char is 1, set that bit using OR */
            if (currentChar == '1') {
                tempByte = tempByte | (1 << bitCount);
            }
            bitCount++;

            if (bitCount == MAX_BITS) {
                fwrite(&tempByte, sizeof(BYTE), 1, imgPtr);
                tempByte = 0;
                bitCount = 0;
            }
        } 
    } 
    /* at the very last pixel, if it hasn't reached a full byte, write it in */
    if (bitCount != MAX_BITS && bitCount != 0) {
        fwrite(&tempByte, sizeof(BYTE), 1, imgPtr);
    }
}

/* write freq array into the file */
void writeFreq(int *freq, BYTE *arr, FILE *imgPtr) {
    fwrite(freq, 256*sizeof(int), 1, imgPtr);
}

