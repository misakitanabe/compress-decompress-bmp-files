#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

int createHeap(int *freq, node **heap, int heapSize) {
    int i;

    for (i = 0; i < 256; i++) {
        /* only insert in heap if that RGB value occurs */
        if (freq[i] != 0) {
            node *newNode = (node *)malloc(sizeof(node));
            newNode->colorVal = i;
            newNode->freq = freq[i];
            insertInHeap(heap, newNode, heapSize);
            heapSize++;
        }
    }
    return heapSize;
}

void insertInHeap(node **heap, node *newNode, int heapSize) {
    node *oldParent;
    int index = heapSize + 1;
    heap[index] = newNode;

    /* while parent's frequency greater, swap */
    while (index > 1 && (heap[(int)(index / 2)])->freq > newNode->freq) {
        oldParent = heap[(int)(index / 2)];
        heap[(int)(index / 2)] = heap[index];
        heap[index] = oldParent;
        index = (int)(index/2);
    }
}

node* popMin(node **heap, int heapSize) {
    int index = 1;
    int minIndex;
    node *min = heap[1];
    node *left;
    node *right;

    /* set last node to the root node and free root node */
    heap[index] = heap[heapSize];
    heapSize--;

    /* percolate new root down */
    while (index <= heapSize) {
        left = NULL;
        right = NULL;
        
        if (2*index <= heapSize) {
            left = (heap[2*index]);
        }
            
        if (2*index+1 <= heapSize) {
            right = (heap[2*index+1]);
        }
        
        /* find the index of the child with the lesser freq */
        if ((right == NULL && left != NULL) || (left != NULL && right != NULL && left->freq < right->freq))
            minIndex = 2*index;
        else if ((left == NULL && right != NULL) || (left != NULL && right != NULL && left->freq >= right->freq)) {
            minIndex = 2*index+1;
        }
        else if (left == NULL && right == NULL) {
            return min;
        }
        
        /* if min child is less than parent, swap */
        if (heap[minIndex]->freq != 0 && heap[minIndex]->freq < heap[index]->freq) {
            node *temp = heap[minIndex];
            heap[minIndex] = heap[index];
            heap[index] = temp;
            index = minIndex;
        /* if lesser of children is greater than parent, root now in the right place */
        } else {
            return min;
        }
    }  
    return min;
}

