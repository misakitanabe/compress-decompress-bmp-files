#define MAX_CODE_LENGTH 100
#define NUM_CODES 256

void createTree(node **heap, int heapSize);
void printArr(int arr[], int n);
void getCodes(node* root, char codes[][MAX_CODE_LENGTH], int arr[], int level);
int isLeaf(node* root);
void convertToChar(int arr[], int n, char codes[][MAX_CODE_LENGTH], int colorVal);
void createCodesArray(node **heap, char codes[][MAX_CODE_LENGTH]);
void printCodes(char codes[][MAX_CODE_LENGTH]);
void deleteTree(node *root);