void writeFileHeader(FILE *img, FILEHEADER *fh);
void writeInfoHeader(FILE *img, INFOHEADER *ih);
void writeData(BYTE *arr, FILE *imgPtr, INFOHEADER *ih, int padding);