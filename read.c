#include <stdio.h>
#include "bmp.h"

/* img points to start of FILE, then use the bfOffBits from FILEHEADER to begin the pixel data*/
int readData(BYTE *arr, FILE *imgPtr, INFOHEADER *ih) {
    int i;
    int j;
    int padding;

    if ((ih->biWidth * 3) % 4 == 0)
        padding = 0;
    else   
        padding = 4 - ((ih->biWidth * 3) % 4);
    
    /* printf("width: %d padding size: %d\n", ih->biWidth, padding); */
    for (i = 0; i < (ih->biHeight); i++) {
        for (j = 0; j < (ih->biWidth * 3); j++) {
            fread(arr + (i * 3 * ih->biWidth + j), sizeof(BYTE), 1, imgPtr);
        }
        if (padding != 0)
            fseek(imgPtr, padding, SEEK_CUR); /* move pointer over padding */
    }
    return padding;
}

int readFileHeader(FILE *img, FILEHEADER *fh) {
    int success = 0;

    success = fread(&(fh->bfType), sizeof(WORD), 1, img);
    success = fread(&(fh->bfSize), sizeof(DWORD), 1, img);
    success = fread(&(fh->bfReserved1), sizeof(WORD), 1, img);
    success = fread(&(fh->bfReserved2), sizeof(WORD), 1, img);
    success = fread(&(fh->bfOffBits), sizeof(DWORD), 1, img);

    return success;
}

int readInfoHeader(FILE *img, INFOHEADER *ih) {
    int success = 0; 
    
    success = fread(&(ih->biSize), sizeof(DWORD), 1, img);
    success = fread(&(ih->biWidth), sizeof(DWORD), 1, img);
    success = fread(&(ih->biHeight), sizeof(DWORD), 1, img);
    success = fread(&(ih->biPlanes), sizeof(WORD), 1, img);
    success = fread(&(ih->biBitCount), sizeof(WORD), 1, img);
    success = fread(&(ih->biCompression), sizeof(DWORD), 1, img);
    success = fread(&(ih->biSizeImage), sizeof(DWORD), 1, img);
    success = fread(&(ih->biXPelsPerMeter), sizeof(DWORD), 1, img);
    success = fread(&(ih->biYPelsPerMeter), sizeof(DWORD), 1, img);
    success = fread(&(ih->biClrUsed), sizeof(DWORD), 1, img);
    success = fread(&(ih->biClrImportant), sizeof(DWORD), 1, img);

    return success;
}