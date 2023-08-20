#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "compress.h"

int main(int argc, char *argv[]) {
    FILE *img;
    FILE *resultImg;
    FILEHEADER imgFH;
    FILEHEADER resultFH;
    FILEHEADER *imgFHPtr = &imgFH;
    FILEHEADER *resultFHPtr = &resultFH;
    INFOHEADER imgIH;
    INFOHEADER resultIH;
    INFOHEADER *imgIHPtr = &imgIH;
    INFOHEADER *resultIHPtr = &resultIH;
    BYTE *arr;
    BYTE *resultArr;
    int padding = 0;
    int x;
    int y;
    int pid;
    LONG width;
    LONG height;
    LONG midpoint;
    struct timeval start, end;
    double timeDiff;
    int heapSize = 0;
    char *fileName;
    char newFileName[100];
    char *flag;
    int nameSize = 0;
    int bmp = 0;
    int gray = 0;
    int dataSize = 0;

    /* malloc array of pointers for heap */
    node **heap = (node **)malloc(257 * sizeof(node *));
    heap[0] = (node *)malloc(sizeof(node));
    
    /* check file name for .bmp or .cbmp */
    fileName = argv[2]; 
    nameSize = strlen(fileName);
    /* if argument is a bmp file */
    if (fileName[nameSize - 1] == 'p' && fileName[nameSize - 2] == 'm' && fileName[nameSize - 3] == 'b' && fileName[nameSize - 4] == '.') {
        bmp = 1;
        strncpy(newFileName, fileName, nameSize - 3);
        newFileName[nameSize - 3] = 'c';
        newFileName[nameSize - 2] = 'b';
        newFileName[nameSize - 1] = 'm';
        newFileName[nameSize] = 'p';
        newFileName[nameSize + 1] = '\0';
    /* if argument is a cbmp file */
    } else {
        strncpy(newFileName, fileName, nameSize - 4);
        newFileName[nameSize - 4] = 'b';
        newFileName[nameSize - 3] = 'm';
        newFileName[nameSize - 2] = 'p';
        newFileName[nameSize - 1] = '\0';
    }

    /* check flag for gray or color */
    flag = argv[1];
    if (flag[1] == 'g')
        gray = 1;

    /* open files */
    img = fopen(fileName, "rb");
    resultImg = fopen(newFileName, "wb");

    /* read headers of input file */
    readFileHeader(img, imgFHPtr);
    readInfoHeader(img, imgIHPtr);
    width = imgIHPtr->biWidth;
    height = imgIHPtr->biHeight;

    /* set FH and IH of input file to output file */
    resultFHPtr = imgFHPtr;
    resultIHPtr = imgIHPtr;

    /* if input file is a BMP file, COMPRESS */
    if (bmp == 1) {
        int *freq;
        char codes[NUM_CODES][MAX_CODE_LENGTH];
        memset(codes, '\0', sizeof(codes));

        /* mmap arrays for pixel data */
        arr = mmap(NULL, width * height * 3, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
        resultArr = mmap(NULL, width * height * 3, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
        freq = mmap(NULL, sizeof(int) * 256, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
        if (arr == MAP_FAILED || resultArr == MAP_FAILED || freq == MAP_FAILED) {
            perror("mmap failed for input pixel data\n");
            exit(EXIT_FAILURE);
        }
        memset(freq, 0, 256 * sizeof(int));

        /* read pixel data */
        padding = readData(arr, img, imgIHPtr);

        /* start clock */
        gettimeofday(&start, NULL);

        /* FORK */
        pid = fork();
        if (height % 2 == 1) 
            midpoint = (height / 2) + 1;
        else   
            midpoint = height / 2;

        /* parent - iterate through upper half of pixels and apply brightness */
        if (pid > 0) {
            for (y = midpoint; y < height; y++) {
                for (x = 0; x < width; x++) {
                    if (gray)
                        setGray(x, y, arr, resultArr, imgIHPtr, freq);
                    else 
                        setColor(x, y, arr, resultArr, imgIHPtr, freq);
                }
            }
            wait(0);
        /* child - iterate through lower half of pixels and apply brightness */
        } else if (pid == 0) {
            for (y = 0; y < midpoint; y++) {
                for (x = 0; x < width; x++) {
                    if (gray)
                        setGray(x, y, arr, resultArr, imgIHPtr, freq);
                    else 
                        setColor(x, y, arr, resultArr, imgIHPtr, freq);
                }
            }
            return 0;
        }
        heapSize = createHeap(freq, heap, heapSize);
        createTree(heap, heapSize);

        /* create codes for each RGB value */
        createCodesArray(heap, codes);

        /* end clock */
        gettimeofday(&end, NULL);

        /* write headers */
        writeFileHeader(resultImg, resultFHPtr);
        writeInfoHeader(resultImg, resultIHPtr);

        /* write compressed data to file, read different amounts in arr depending on gray or color */
        if (gray) 
            dataSize = width * height;
        else        
            dataSize = width * height * 3;
        writeCompressed(freq, resultArr, codes, resultImg, dataSize);

        /* clean up */
        fclose(img);
        fclose(resultImg);
        deleteTree(heap[1]);
        free(heap[0]);
        free(heap);
        munmap(freq, 256 * sizeof(int));
        munmap(arr, width * height * 3);
        munmap(resultArr, width * height * 3);

        /* print the time */
        timeDiff = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("total time: %f seconds\n", timeDiff);

    /* if input file is a CBMP file, DECOMPRESS */
    } else if (bmp == 0) {
        int freq[256] = {0};

        resultArr = malloc(width * height * 3);
        if (resultArr == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        /* create tree from read in freq list */
        readFreq(freq, img);
        heapSize = createHeap(freq, heap, heapSize);
        createTree(heap, heapSize);

        /* start clock */
        gettimeofday(&start, NULL);

        /* read pixel data */
        padding = readBits(resultArr, img, imgIHPtr, heap, gray);
        
        /* end clock */
        gettimeofday(&end, NULL);

        writeFileHeader(resultImg, resultFHPtr);
        writeInfoHeader(resultImg, resultIHPtr);
        writeData(resultArr, resultImg, resultIHPtr, padding);

        /* clean up */
        fclose(img);
        fclose(resultImg);
        deleteTree(heap[1]);
        free(heap[0]);
        free(heap);
        free(resultArr);
    
        /* print the time */
        timeDiff = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("total time: %f seconds\n", timeDiff);
    }
    return 0;
}

