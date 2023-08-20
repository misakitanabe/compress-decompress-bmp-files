int readFileHeader(FILE *img, FILEHEADER *fh);
int readInfoHeader(FILE *img, INFOHEADER *fh);
int readData(BYTE *arr, FILE *imgPtr, INFOHEADER *ih);