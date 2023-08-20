#include <stdio.h>
#include "bmp.h"
#include "color.h"

BYTE getColor(BYTE *arr, INFOHEADER *ih, int x, int y, char color) {
    BYTE *pixel;
    BYTE colorVal = 0;

    pixel = arr + ((3 * y * ih->biWidth) + 3*x);

    switch(color) {
    case 'b':
        colorVal = *(pixel);
        break;
    case 'g':
        colorVal = *(pixel + 1);
        break;
    case 'r':
        colorVal = *(pixel + 2);
        break;
    }

    return colorVal;
}

/* averages RGB values to make image gray and increments value at that index in freq array */
void setGray(int x, int y, BYTE *arr, BYTE *resultArr, INFOHEADER *imgIHPtr, int *freq) {
    BYTE b;
    BYTE g;
    BYTE r;
    BYTE avg;

    b = getColor(arr, imgIHPtr, x, y, 'b');
    g = getColor(arr, imgIHPtr, x, y, 'g');
    r = getColor(arr, imgIHPtr, x, y, 'r');

    avg = (b + g + r)/3;
    freq[avg]++;
    *(resultArr + (y * imgIHPtr->biWidth) + (x)) = avg;

    return;
}

/* averages RGB values to make image gray and increments value at that index in freq array */
void setColor(int x, int y, BYTE *arr, BYTE *resultArr, INFOHEADER *imgIHPtr, int *freq) {
    BYTE b;
    BYTE g;
    BYTE r;

    b = getColor(arr, imgIHPtr, x, y, 'b');
    g = getColor(arr, imgIHPtr, x, y, 'g');
    r = getColor(arr, imgIHPtr, x, y, 'r');

    freq[b]++;
    freq[g]++;
    freq[r]++;

    *(resultArr + (3 * y * imgIHPtr->biWidth) + (3 * x)) = b;
    *(resultArr + (3 * y * imgIHPtr->biWidth) + (3 * x) + 1) = g;
    *(resultArr + (3 * y * imgIHPtr->biWidth) + (3 * x) + 2) = r;

    return;
}

