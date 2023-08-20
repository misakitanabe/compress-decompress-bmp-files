#include <stdio.h>
#include <stdlib.h>
#include "compress.h"


void createTree(node **heap, int heapSize) {
    node *leftChild;
    node *rightChild;
    int numPop = 1;

    while (heapSize > 1) {
        node *newParent;
        numPop++;
        leftChild = popMin(heap, heapSize);
        heapSize--;

        numPop++;
        rightChild = popMin(heap, heapSize);
        heapSize--;

        newParent = (node *)malloc(sizeof(node *));
        newParent->colorVal = 0;
        newParent->freq = leftChild->freq + rightChild->freq;
        newParent->left = leftChild;
        newParent->right = rightChild;
        insertInHeap(heap, newParent, heapSize);
        heapSize++;
    }
}


void createCodesArray(node **heap, char codes[][MAX_CODE_LENGTH]) {
    int codeHolder[MAX_CODE_LENGTH];
    /* get string codes for each colorVal and store it in codes */
    getCodes(heap[1], codes, codeHolder, 0);
}

/* https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/ */
void getCodes(node* root, char codes[][MAX_CODE_LENGTH], int arr[], int level)
{
 
    /* Assign 0 to left edge and recur */
    if (root->left) {
        arr[level] = 0;
        getCodes(root->left, codes, arr, level + 1);
    }
 
    /* Assign 1 to right edge and recur */
    if (root->right) {
        arr[level] = 1;
        getCodes(root->right, codes, arr, level + 1);
    }
 
    /* leaf node is a symbol, convert the int code acquired to a string */
    if (isLeaf(root)) {
        convertToChar(arr, level, codes, (int)root->colorVal);
    }
}

int isLeaf(node* root)
{
    return !(root->left) && !(root->right);
}

/* A utility function to print an array of size n */
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
 
   printf("\n");
}

void printCodes(char codes[][MAX_CODE_LENGTH]) {
    int i;
    printf("\n");
    for (i = 0; i < NUM_CODES; i++) {
        if (*codes[i] != '\0') {
            printf("rgb %d: ", i);
            printf("%s\n", codes[i]);
        }
    }
}

void convertToChar(int arr[], int n, char codes[][MAX_CODE_LENGTH], int colorVal)
{
    int i;
    for (i = 0; i < n; ++i) {
        codes[colorVal][i] = '0' + arr[i];
    }
    codes[colorVal][++i] = '\0';
}

void deleteTree(node *root) {
    if (root == NULL) 
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}