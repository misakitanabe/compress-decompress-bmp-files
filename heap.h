typedef struct node node;

struct node
{
    node *left;
    node *right;
    int freq;
    BYTE colorVal;
};

int createHeap(int *freq, node **heap, int heapSize);
void insertInHeap(node **heap, node *newNode, int heapSize);
node* popMin(node **heap, int heapSize);
void setChildren(int insertedIndex, node **heap, int heapSize);