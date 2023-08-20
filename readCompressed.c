#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

#define MAX_BITS 8


int readBits(BYTE *resultArr, FILE *imgPtr, INFOHEADER *ih, node **heap, int gray) {
    int i;
    BYTE tempByte = 0;
    BYTE mask = 1;
    BYTE result = 0;
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
            result = tempByte & mask;

            /* if bit encountered is 0, go left in tree */
            if (result == 0) {
                root = root->left;
            /* if bit encountered is 1, go right in tree */
            } else if (result == 1) {
                root = root->right;
            }
            /* if landed at a leaf, get the rgb value and write it in the result arr */
            if (isLeaf(root)) {
                colorVal = root->colorVal;
                /* write in same value for all three if gray */
                if (gray) {
                    *(resultArr + index++) = colorVal;
                    *(resultArr + index++) = colorVal;
                    *(resultArr + index++) = colorVal;
                /* if color, do each unique colorValue */
                } else {
                    *(resultArr + index++) = colorVal;
                }
                root = heap[1];
            }
            tempByte = tempByte >> 1;
        }        
    } 
    return padding;
}


/* read freq array into the file */
void readFreq(int *freq, FILE *imgPtr) {
    size_t success = 0;

    if (!((success = fread(freq, 256*sizeof(int), 1, imgPtr)) > 0)) {
        printf("reading frequency table not successful\n");
        exit(0);
    }
}

