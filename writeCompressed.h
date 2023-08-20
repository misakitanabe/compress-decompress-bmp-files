#define MAX_BITS 8

void writeBits(BYTE *arr, char codes[][MAX_CODE_LENGTH], FILE *imgPtr, int dataSize);
void writeFreq(int *freq, BYTE *arr, FILE *imgPtr);
void writeCompressed(int *freq, BYTE *arr, char codes[][MAX_CODE_LENGTH], FILE *imgPtr, int dataSize);