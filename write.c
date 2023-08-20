#include <stdio.h>
#include "bmp.h"

void writeData(BYTE *arr, FILE *imgPtr, INFOHEADER *ih, int padding) {
    int i;
    BYTE null = '\0';
    BYTE *NULLptr;
    NULLptr = &null;
    
    for (i = 0; i < (ih->biWidth * 3 * ih->biHeight); i++) {
        fwrite(arr + i, sizeof(BYTE), 1, imgPtr);
        if (padding != 0 && (i + 1) % (ih->biWidth * 3) == 0) {
            fwrite(NULLptr, sizeof(BYTE), padding, imgPtr);
        }
    }
    
}

void writeFileHeader(FILE *img, FILEHEADER *fh) {
    fwrite(&(fh->bfType), sizeof(WORD), 1, img);
    fwrite(&(fh->bfSize), sizeof(DWORD), 1, img);
    fwrite(&(fh->bfReserved1), sizeof(WORD), 1, img);
    fwrite(&(fh->bfReserved2), sizeof(WORD), 1, img);
    fwrite(&(fh->bfOffBits), sizeof(DWORD), 1, img);
}

void writeInfoHeader(FILE *img, INFOHEADER *ih) {
    fwrite(&(ih->biSize), sizeof(DWORD), 1, img);
    fwrite(&(ih->biWidth), sizeof(DWORD), 1, img);
    fwrite(&(ih->biHeight), sizeof(DWORD), 1, img);
    fwrite(&(ih->biPlanes), sizeof(WORD), 1, img);
    fwrite(&(ih->biBitCount), sizeof(WORD), 1, img);
    fwrite(&(ih->biCompression), sizeof(DWORD), 1, img);
    fwrite(&(ih->biSizeImage), sizeof(DWORD), 1, img);
    fwrite(&(ih->biXPelsPerMeter), sizeof(DWORD), 1, img);
    fwrite(&(ih->biYPelsPerMeter), sizeof(DWORD), 1, img);
    fwrite(&(ih->biClrUsed), sizeof(DWORD), 1, img);
    fwrite(&(ih->biClrImportant), sizeof(DWORD), 1, img);
}
