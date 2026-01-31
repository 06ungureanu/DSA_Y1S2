#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_SIZE 10

typedef struct Node {
    char *key;
    struct Node *next;
} Node;

Node *create_node(const char *key) {
    if(key == NULL) return NULL;
    
    Node *tmp = (Node*)malloc(sizeof(Node));
    if(tmp == NULL) {
        printf("Error while creating memory for a new node\n");
        return NULL;
    }

    tmp->key = strdup(key);
    tmp->next = NULL;
    return tmp;
}
int hash(const char *str) {
    if(str == NULL) {
        printf("Invalid string\n");
        return -1;
    }
   
    int hash = 0;
    for(; *str; str++) {
        hash += (int)*str;
        hash %= T_SIZE;
    }
    return hash;
}
void insertNodeInList(Node **head, const char *key) {
    if(head == NULL || key == NULL) return;

    if(*head == NULL) 
    {
        Node *tmp = create_node(key);
        tmp->next = *head;
        *head = tmp;
        return;
    }
    else
    {
        Node *tail = *head;
        for(; tail->next != NULL; tail = tail->next);
        tail->next = create_node(key);
    }
}
void insertKeyVal(Node *HT[T_SIZE],const char *key) {
    if(key == NULL) return;
    int index = hash(key);
    
    if(index == -1) {
        printf("Error while generating hash key\n");
        return;
    } else insertNodeInList(&HT[index], key);
}
Node *searchList(Node *head, const char *key) {
    if(head == NULL || key == NULL) return NULL;
    
    return (strcmp(head->key, key) == 0) ? head : searchList(head->next, key);
}
Node *getValue(Node *HT[T_SIZE], const char *key) {
    if(HT == NULL || key == NULL) return NULL;
    
    int index = hash(key);
    if(!HT[index])
        return NULL;
    else 
        return searchList(HT[index],key);
}
int main() {
    Node *HT[T_SIZE] = {NULL};

    const char *s1 = "Mirela";
    const char *s2 = "Cristina";
    const char *s3 = "Cristiana";

    insertKeyVal(HT,s1);
    insertKeyVal(HT,s2);
    insertKeyVal(HT,s3);
    
    return 0;
}
