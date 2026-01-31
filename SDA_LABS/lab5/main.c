#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

typedef struct Node {
    int val;
    struct Node *next;
} Node;

typedef struct strNode {
    char string[BUFFER_SIZE];
    struct strNode *next;
} strNode;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

typedef struct List {
    Node *head;
    Node *tail;
} List;

typedef struct strList {
    strNode *head;
    strNode *tail;
} strList;

void freeStringList(strList *l) {
    if(l == NULL || l->head == NULL) return;

    strNode *curr = l->head;
    while(curr != NULL) {
        strNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l);
}
void freeList(List *l) {
    if(l == NULL || l->head == NULL) return;

    Node *curr = l->head;
    while(curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l);
    l = NULL;
}
void freeQueue(Queue *q) {
    if(q == NULL || q->front == NULL) return;

    Node *curr = q->front;
    while(curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(q);
    q = NULL;
}
void freeStack(Node *top) {
    if(top == NULL) return;


    Node *curr = top;
    while(curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(top);
    top = NULL;
}

Node *createNode(int value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if(n == NULL)
        return n;

    n->val = value;
    n->next = NULL;
    return n;
}
List *createEmptyList()
{
    List *l = (List*)malloc(sizeof(List));
    if(l == NULL)
        return NULL;

    l->head = NULL;
    l->tail = NULL;
    return l;
}
Queue *createEmptyQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if(q == NULL)
        return NULL;

    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(Queue *queue, int value) {
    if(queue == NULL)
        return;

    Node *temp = createNode(value);
    if(temp == NULL)
        return;

    if(queue->front == NULL && queue->rear == NULL)
    {
        queue->front = temp;
        queue->rear = temp;
        return;
    }

    queue->rear->next = temp;
    queue->rear = temp;
}
int dequeue(Queue *queue) {
    if(queue == NULL || queue->front == NULL)
        return -1;

    Node *current = queue->front;
    queue->front = queue->front->next;
    
    if(queue->front == NULL)
        queue->rear = NULL;

    int value = current->val;
    free(current);

    return value;
}
void push(Node **top, int value) {
    Node *temp = createNode(value);
    if(temp == NULL)
        return;

    temp->next = *top;
    *top = temp;
}
int pop(Node **top) {
    if(*top == NULL)
        return -1;

    Node *TOS = *top;
    *top = (*top)->next;
    
    int value = TOS->val;
    free(TOS);

    return value;

}
void printQueue(Queue *queue) {
    if(queue == NULL || queue->front == NULL)
        return;

    printf("Elemente din coada sunt : ");
    while(queue->front != NULL)
        printf("%d ", dequeue(queue));
    printf("\n");
}
void printStack(Node *top) {
    if(top == NULL)
        return;

    printf("Elementele din stiva sunt : ");
    while(top != NULL)
        printf("%d ",pop(&top));
    printf("\n");
}
int JosephusQueue(int n, int k) {
    
    Queue *queue = createEmptyQueue();
    if(queue == NULL)
        return -1;
    
    for(int i = 1; i <= n; i++)
        enqueue(queue,i);

    while(queue->front != queue->rear)
    {
        for(int i = 1; i < k; i++)
            enqueue(queue,dequeue(queue));
        dequeue(queue);
    }

    int winner = queue->front->val;
    freeQueue(queue);
    return winner;
}
int EmptyStack(Node *top)
{
    return top == NULL;
}
int validateParentheses(char ch1, char ch2) {
    return ((ch1 == '{' && ch2 == '}') || (ch1 == '[' && ch2 == ']') || (ch1 == '(' && ch2 == ')'));
}
int goodExpresion(const char *expresion) {
    Node *stack = NULL;
    int len = strlen(expresion);

    for(int i = 0; i < len; i++)
    {
        if(expresion[i] == '{' || expresion[i] == '[' || expresion[i] == '(')
            push(&stack, expresion[i]);
        else if(expresion[i] == '}' || expresion[i] == ']' || expresion[i] == ')')
        {
            if(EmptyStack(stack))
                return 0;
            if(!validateParentheses(pop(&stack), expresion[i]))
               return 0;
        }
    }
    return EmptyStack(stack);
}
Node *createCircularList(int n) {
    if(n <= 0)
        return NULL;

    Node *head = createNode(1);
    Node *prev = head;

    for(int i = 2; i <= n; i++) {
        Node *temp = createNode(i);
        prev->next = temp;
        prev = temp;
    }

    prev->next = head;
    return head;
}
int JosephusList(int n, int k) {
    Node *list = createCircularList(n);
    if(list == NULL)
        return -1;

    Node *prev = NULL;
    Node *curr = list;

    while(curr->next != curr) {
        for(int i = 1; i < k; i++) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr);
        curr = prev->next;
    }
    int winner = curr->val;
    free(curr);
    
    return winner;
}
strList *createStringList() {
    strList *l = (strList*)malloc(sizeof(strList));
    if(l == NULL) return NULL;

    l->tail = NULL;
    l->head = NULL;

    return l;
}
strNode *createStringNode(const char *str) {
    strNode *temp = (strNode*)malloc(sizeof(strNode));
    if(temp == NULL) return NULL;

    strcpy(temp->string,str);
    temp->next = NULL;

    return temp;
}
strList *InsertWordsFromFile(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) return NULL;

    strList *l = createStringList();
    if(l == NULL) {
        fclose(f);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    while(fscanf(f, "%s", buffer) == 1) {
        strNode *temp = createStringNode(buffer);
        if(temp == NULL) {
            freeStringList(l);
            fclose(f);
            return NULL;
        }    
        if(l->head == NULL && l->tail == NULL) {
            l->head = temp;
            l->tail = temp;
        }
        else {
            strNode *current = l->head;
            while(current->next != NULL && strcmp(buffer,l->head->string) > 0) 
                current = current->next;

            temp->next = current->next;
            current->next = temp;
        }

    }
    return l;
}
void printStringList(strList *l) {
    if(l == NULL || l->head == NULL) return;

    for(strNode *p = l->head; p != NULL; p = p->next) 
        printf("%s ", p->string);
    printf("\n");
}
int main() {

    Queue *queue = createEmptyQueue();
    Node *stack = NULL;
    const char *words = "words.txt";
    
    for(int i = 0; i < 5; i++)
    {
        enqueue(queue,i);
        push(&stack,i);
    }
    
    printQueue(queue);
    printStack(stack);
   
    int JosephusWinner = JosephusQueue(6,2);
    printf("Castigatorul Josephus (Queue) pentru n = 6 si k = 2 : %d\n", JosephusWinner);

    char *expresion = "{a + [b - (c + d)]}";
    if(goodExpresion(expresion))
        printf("Expresia %s este echilibarata\n", expresion);
    else 
        printf("Expresia %s nu este echilibara\n", expresion);
    int JosephusWinnerList = JosephusList(6,2);
    printf("Castigatorul Josephus (List) pentru n = 6 si k = 2 :  %d\n", JosephusWinnerList);
    
    strList *fileWordsList = InsertWordsFromFile(words);
    printf("Lista ce contine cuvintele din fisierul %s in ordine alfabetica : ", words);
    printStringList(fileWordsList);

    freeQueue(queue);
    freeStack(stack);
    freeStringList(fileWordsList);

    return 0;
}
