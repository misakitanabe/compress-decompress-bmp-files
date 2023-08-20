#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

#define MAX_BITS 8

int readBits(BYTE *resultArr, FILE *imgPtr, INFOHEADER *ih, node **heap) {
    int i;
    BYTE tempByte = 0;
    BYTE mask = 1;
    BYTE result = 0;
    int bitCount = 0;
    int padding;
    size_t success = 0;
    BYTE colorVal;
    node *root = heap[1];
    int index = 0;

    if ((ih->biWidth * 3) % 4 == 0)
        padding = 0;
    else   
        padding = 4 - ((ih->biWidth * 3) % 4);

    /* keep reading a single BYTE until pointer reaches the end of file */
    while ((success = fread(&tempByte, sizeof(BYTE), 1, imgPtr)) > 0) {
        
        /* iterate through each bit of BYTE */
        for (i = 0; i < 8; i++) {
            // printf("tempByte: %u\n", tempByte);

            result = tempByte & mask;
            // printf("result = %u\n", result);
            /* if bit encountered is 0, go left in tree */
            if (result == 0) {
                root = root->left;
                // printf("Went LEFT %u: %d\n", root->colorVal, root->freq);
            /* if bit encountered is 1, go right in tree */
            } else if (result == 1) {
                root = root->right;
                // printf("went RIGHT %u: %d\n", root->colorVal, root->freq);
            }
            /* if landed at a leaf, get the rgb value and write it in the result arr */
            if (isLeaf(root)) {
                colorVal = root->colorVal;
                // printf("colorVal at LEAF: %u \n", colorVal);
                // colorVal = 40;

                *(resultArr + index++) = colorVal;
                *(resultArr + index++) = colorVal;
                *(resultArr + index++) = colorVal;

                root = heap[1];
                // printf("reset root: %d\n", root->freq);
            }
            tempByte = tempByte >> 1;
        }
        // printf("index = %d\n", index);
        
    } 
    return padding;
}


/* read freq array into the file */
void readFreq(int *freq, FILE *imgPtr) {
    size_t success = 0;

    if ((success = fread(freq, 256*sizeof(int), 1, imgPtr)) > 0) {
        printf("reading freq table successful\n\n");
    } else {
        printf("unsuccessful reading freq table ret = %zu\n", success);
        exit(0);
    }

    // for (int i = 0; i < 256; i++) {
    //     if (freq[i] != 0) {
    //         printf("rgb: %d freq: %d\n", i, freq[i]);
    //     }
    // }
}

