#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

struct Matrix{
    int num;
    int** matrix;
};
void printMatrix(struct Matrix*); // a function I don't use but think it should be in the file;
int isAdjacencyMatrix(struct Matrix*); // a function I don't use but think it should be in the file;
struct Node{
    int index;
    struct Node* Next;
};
struct Node* createEmptyNode();
void printNode(struct Node*);
struct List{
    int numNodes;
    struct Node** Nodes;
};
void printList(struct List*); // a function I don't use but think it should be in the file;
struct List* createEmptyList(int);
struct Matrix * fGetMatrix(FILE*);
struct List* matrixToList(struct Matrix*);

struct Queue{
    int *items;
    int last;
    int head;
};
int isQueued(struct Queue*, int);
struct Queue* createQueue(int);
void addQueue(struct Queue*, int);
int deQueue(struct  Queue*);
void printQueue(struct Queue*);
struct Node* BFS(struct List*, int, int);

int main(){
    FILE *fptr = fopen("maze.txt","r");
    if(fptr == NULL){// It checks for problems in reading the file, and shows an example of where the file should be, if any;
        fptr = fopen("maze.txt","w");
        printf("File not found.\n"
               "For an example file has been created.(maze.txt)\n"
               "Please save the file in the sample file name, in the sample file folder.");
        fclose(fptr);
        getch();
        return 1; //File not found;
    }
    struct Matrix* matrixStruct = fGetMatrix(fptr);
    if(matrixStruct == NULL){ // checks the file format;
        printf("File format not suitable for square matrix");
        getch();
        return 2; // the file format is wrong;
    }
    struct List* list = matrixToList(matrixStruct);

    struct Node* answer = BFS(list, list->numNodes - 1, 0);
    // by default the highest will be the start, the lowest (i.e. 0) will be the end;

    printf("\n"
           "\n"
           "Checked vertices when searching for number with BFS:\n");
    printNode(answer); // I also printed the "queue"s to the screen beforehand for control.
    getch();
    return 0;
}

void printMatrix(struct Matrix* matrix){ // a function I don't use but think it should be in the file;
    int i,j;
    for(i=0; i < matrix->num; i++){
        for(j=0; j < matrix->num; j++){
            printf("%d ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}
int isAdjacencyMatrix(struct Matrix* matrixStruct){ // a function I don't use but think it should be in the file;
    int i,j;
    for (i = 0; i < matrixStruct->num; i++)
        for (j = 0; j < matrixStruct->num; j++)
            if (matrixStruct->matrix[i][j] != matrixStruct->matrix[j][i])
                return 0;
    return 1;
}
struct Node* createEmptyNode(){
    struct Node* node = malloc(sizeof(struct Node));
    node->index=-1;
    node->Next=NULL;
    return node;
}
void printNode(struct Node* node){
    while(node){
        printf("%d%s",node->index,node->Next==NULL ? "":" , ");
        node = node->Next;
    }
    printf("\n");
}
void printList(struct List* listRoot){ // a function I don't use but think it should be in the file;
    int i;
    struct Node* temp;
    for(i=0 ; i < listRoot->numNodes ; i++){
        temp= listRoot->Nodes[i];
        printf("Node %d: ",i);
        while(temp){
            printf("%d, ",temp->index);
            temp = temp->Next;
        }
        printf("\n");
    }
}
struct List* createEmptyList(int num){
    int i;
    struct List* list = malloc(sizeof(struct List));
    list->numNodes = num;
    list->Nodes = malloc(num*sizeof(struct Node*));
    for (i = 0; i < num; i++)
        list->Nodes[i] = createEmptyNode();
    return list;
}
struct Matrix* fGetMatrix(FILE* fptr){
    int x,i,j;
    for (i = 0; !feof(fptr); i++)
        fscanf(fptr, "%d ", &x);
    x = (int) sqrt(i);
    if(i != x * x) return NULL;
    int** matrix = malloc(x * sizeof(int*));
    for(i=0; i < x; i++)
        matrix[i] = malloc(x * sizeof(int));

    rewind(fptr);
    for (i = 0; i < x; i++)
        for (j = 0; j < x; j++)
            fscanf(fptr, "%d ", &matrix[i][j]);

    struct Matrix* matrixStruct = malloc(sizeof(struct Matrix));
    matrixStruct->matrix = matrix;
    matrixStruct->num=x;
    return matrixStruct;
}
struct List* matrixToList(struct Matrix* matrixStruct){
    int i,j,flag;
    struct List* list = createEmptyList(matrixStruct->num);
    struct Node* temp;
    for (i = 0; i < matrixStruct->num; i++)
        for (temp = list->Nodes[i], flag = 1, j = 0; j < matrixStruct->num; j++)
            if (matrixStruct->matrix[i][j] != 0)
                if (flag) {
                    temp->index = j;
                    flag = 0;
                } else {
                    temp->Next = createEmptyNode();
                    temp = temp->Next;
                    temp->index = j;
                }
    return list;
}
int isQueued(struct Queue* queue,int num){
    int i;
    for (i = queue->head; i <= queue->last; i++)
        if (queue->items[i] == num)
            return 1;
    return 0;
}
struct Queue* createQueue(int queueSize){
    struct Queue* q = malloc(sizeof(struct Queue));
    q->head=-1;
    q->last=-1;
    q->items = calloc(queueSize,sizeof(int));
    return q;
}
void addQueue(struct Queue* queue, int num){
    if(queue->head == -1){
        queue->head = queue->last = 0;
        queue->items[0] = num;
    }
    else
        queue->items[++queue->last] = num;
}
int deQueue(struct  Queue* queue){
    if (queue->head > queue->last){
        queue->head = queue->last = -1;
        return -1;
    }
    int x = queue->items[queue->head++];
    if (queue->head > queue->last)
        queue->head = queue->last = -1;
    return x;
}
void printQueue(struct Queue* queue){
    int i;

    printf("queue: ");
    if(queue->head == -1){
        printf("{empty}\n");
        return;
    }
    for(i=queue->head; i <= queue->last; i++)
        printf("%d, ",queue->items[i]);
    printf("\n");
}
struct Node* BFS(struct List* listRoot, int startIndex, int endIndex){
    struct Node* answerIter = createEmptyNode();
    struct Node* answerRoot = answerIter;
    int maxQueueSize= listRoot->numNodes - 1;
    int queueSize = maxQueueSize; // to avoid wasting variable space here; the maximum number of queues can be the number of edges of the node with the most edges;
    struct Queue* queue = createQueue(queueSize);
    addQueue(queue,startIndex);
    int *isVisited = calloc(listRoot->numNodes,sizeof(int));
    int flag = 1;
    while(queue->head != -1){
        printQueue(queue);
        int x = deQueue(queue); // get next from queue;

        isVisited[x] = 1; // Mark the number you received ordinary as visited;

        if(!flag){
            answerIter->Next = createEmptyNode();
            answerIter = answerIter->Next;
        }
        else
            flag = 0;

        answerIter->index = x;

        if(x == endIndex) break;

        struct Node* iter = listRoot->Nodes[x];                             //
        while(iter != NULL){                                                //
            if(isVisited[iter->index] == 0 && !isQueued(queue,iter->index)) // Add your unvisited neighbors to queue;
                addQueue(queue,iter->index);                                //
            iter = iter->Next;                                              //
        }                                                                   //

    }

    return answerRoot;
}