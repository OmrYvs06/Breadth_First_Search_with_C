#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

struct Matris{
    int num;
    int** matris;
};
void printMatris(struct Matris*); // kullanmadığım ama dosyada olması gerektiğini düşündüğüm bir fonksiyon;
int isAdjacencyMatris(struct Matris*); // kullanmadığım ama dosyada olması gerektiğini düşündüğüm bir fonksiyon;
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
void printList(struct List*); // kullanmadığım ama dosyada olması gerektiğini düşündüğüm bir fonksiyon;
struct List* createEmptyList(int);
struct Matris * fMatrisAl(FILE*);
struct List* matrisToList(struct Matris*);

struct Queue{
    int *items;
    int son;
    int bas;
};
int isQueued(struct Queue*, int);
struct Queue* createQueue(int);
void addQueue(struct Queue*, int);
int deQueue(struct  Queue*);
void printQueue(struct Queue*);
struct Node* BFS(struct List*, int, int);

int main(){ // graphı almaya kadar temiz çalışıyor devamı bilinmiyor;
    FILE *fptr = fopen("maze.txt","r");
    if(fptr == NULL){ // dosya okunmasında sorunları kontrol ediyor,varsa dosyanın nerde olması gerektiğine bir örnek gösteriyor;
        fptr = fopen("maze.txt","w");
        printf("Dosya bulunamadi.\n"
               "Ornek amacli bir dosya olusturuldu.(maze.txt)\n"
               "Lutfen dosyayi ornek dosyanin isminde, ornek dosya klasorune kaydediniz.");
        fclose(fptr);
        getch();
        return 1;//dosya bulunamadı;
    }
    struct Matris* matrisStruct = fMatrisAl(fptr);
    if(matrisStruct == NULL){ // dosya formatını kontrol ediyor;
        printf("dosya formati kare matrisi icin uygun degil");
        getch();
        return 2;// dosya formatı yanlış;
    }
    struct List* list = matrisToList(matrisStruct);

    struct Node* sonuc = BFS(list,list->numNodes-1,0);
    // En yüksek start, en düşük (yani 0) end olacaktır;

    printf("\n");
    printf("BFS ile sayiyi ararken kontrol edilen kose'ler:\n");
    printNode(sonuc); // kontrol için daha öncesinde ekrana "queue" leride yazdırdım;
    getch();
    return 0;
}

void printMatris(struct Matris* matris){
    int i,j;
    for(i=0;i<matris->num;i++){
        for(j=0;j<matris->num;j++){
            printf("%d ",matris->matris[i][j]);
        }
        printf("\n");
    }
}
int isAdjacencyMatris(struct Matris* matris){
    int i,j,x=1;
    for (i = 0; i < matris->num; i++)
        for (j = 0; j < matris->num; j++)
            if (matris->matris[i][j] != matris->matris[j][i])
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
void printList(struct List* listRoot){
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
struct Matris* fMatrisAl(FILE *fptr){
    int x,i,j;
    char t;
    for (i = 0; !feof(fptr); i++)
        fscanf(fptr, "%d ", &x);
    x = (int) sqrt(i);
    if(i != x * x) return NULL;
    int** matris = malloc(x*sizeof(int*));
    for(i=0; i < x; i++)
        matris[i] = malloc(x*sizeof(int));

    rewind(fptr);
    for (i = 0; i < x; i++)
        for (j = 0; j < x; j++)
            fscanf(fptr, "%d ", &matris[i][j]);

    struct Matris* matrisStruct = malloc(sizeof(struct Matris));
    matrisStruct->matris = matris;
    matrisStruct->num=x;
    return matrisStruct;
}
struct List* matrisToList(struct Matris* matrisStruct){
    int i,j,flag=1;
    struct List* list = createEmptyList(matrisStruct->num);
    struct Node* temp;
    for (i = 0; i < matrisStruct->num; i++)
        for (temp = list->Nodes[i], flag = 1, j = 0; j < matrisStruct->num; j++)
            if (matrisStruct->matris[i][j] != 0)
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
    for (i = queue->bas; i <= queue->son; i++)
        if (queue->items[i] == num)
            return 1;
    return 0;
}
struct Queue* createQueue(int queueSize){
    struct Queue* q = malloc(sizeof(struct Queue));
    q->bas=-1;
    q->son=-1;
    q->items = calloc(queueSize,sizeof(int));
    return q;
}
void addQueue(struct Queue* queue, int num){
    if(queue->bas == -1){
        queue->bas = queue->son = 0;
        queue->items[0] = num;
    }
    else
        queue->items[++queue->son] = num;
}
int deQueue(struct  Queue* queue){
    if (queue->bas > queue->son){
        queue->bas = queue->son = -1;
        return -1;
    }
    int x = queue->items[queue->bas++];
    if (queue->bas > queue->son)
        queue->bas = queue->son = -1;
    return x;
}
void printQueue(struct Queue* queue){
    int i;

    printf("queue: ");
    if(queue->bas == -1){
        printf("{empty}\n");
        return;
    }
    for(i=queue->bas;i<=queue->son;i++)
        printf("%d, ",queue->items[i]);
    printf("\n");
}
struct Node* BFS(struct List* listRoot, int startIndex, int endIndex){
    struct Node* answerIter = createEmptyNode();
    struct Node* answerRoot = answerIter;
    int maxQueueSize= listRoot->numNodes - 1;
    int queueSize = maxQueueSize; // buradaki değişken yer israfını önlemek için; herhangi bir node un en fazla kenar sayısı kadar olabilir;
    struct Queue* queue = createQueue(queueSize);
    addQueue(queue,startIndex);
    int *isVisited = calloc(listRoot->numNodes,sizeof(int));
    int flag = 1;
    while(queue->bas != -1){
        printQueue(queue);
        int x = deQueue(queue); // queue'den al;

        isVisited[x] = 1; // Sıradan aldığın sayıyı ziyaret edildi olarak işaretle;

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
            if(isVisited[iter->index] == 0 && !isQueued(queue,iter->index)) // Ziyaret edilmemiş komşularını queue ye ekle;
                addQueue(queue,iter->index);                                //
            iter = iter->Next;                                              //
        }                                                                   //

    }

    return answerRoot;
}
