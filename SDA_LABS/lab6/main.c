#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
    struct Node *prev;
} Node;

Node *createNode(int value) {
    Node *temp = (Node*)malloc(sizeof(Node*));
    if(temp == NULL) return NULL;
    
    temp->val = value;
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}
void insertNodeHeadOfList(Node **head, int value) {
    Node *temp = createNode(value);

    
    if(*head == NULL)
    {
        *head = temp;
        temp->prev = *head;
        temp->next = *head;
        return;
    }

    
    temp->next = *head;
    temp->prev = (*head)->prev;

    (*head)->prev->next = temp;
    (*head)->prev = temp;

    *head = temp;
}
void printList(Node *head) {
    if(head == NULL) return;

    Node *temp = head;
    do {
        printf("%d ", temp->val);
        temp = temp->next;
    } while(temp != head);

    printf("\n");
}
void printListRev(Node *head) {
    if(head == NULL) return;

    Node *temp = head->prev;
    do {
        printf("%d ", temp->val);
        temp = temp->prev;
    } while(temp != head->prev);
    printf("\n");
}
Node *searchNodeInList(Node *head, int key) {
    if(head == NULL) return NULL;

    Node *temp = head;
    while(temp->next != head) {
        if(temp->val == key) return temp;
        temp = temp->next;
    }

    return NULL;
}
void insertNodeBefore(Node **head, int key, int val) {
    if(*head == NULL) return;

    Node *temp = createNode(val);
    if(temp == NULL) return;

    
    
    if((*head)->val == key) {
         
        temp->next = *head;
        temp->prev = (*head)->prev;

        (*head)->prev->next = temp;
        (*head)->prev = temp;

        *head = temp;
    }
    
    Node *searchedNode = searchNodeInList(*head, key);
    if(searchedNode == NULL) return;

    temp->prev = searchedNode->prev;
    searchedNode->prev->next = temp;

    searchedNode->prev = temp;
    temp->next = searchedNode;

}
void insertNodeAfter(Node *head, int key, int val) {
    if(head == NULL) return;

    Node *temp = createNode(val);
    if(temp == NULL) return;

    Node *searched = searchNodeInList(head,key);
    if(searched == NULL) return;



    temp->next = searched->next;
    temp->prev = searched;
    searched->next = temp;

    
    if(temp->next != head) {
        temp->next->prev = temp;
    }
    
    
    if(searched->next == head) {
        searched->next = temp;
        temp->next = head;
        temp->prev = searched;
        head->prev = temp;
    }
}
void removeNodeHeadOfList(Node **head) {
    if(head == NULL ||*head == NULL) return;
   
    if(*head == (*head)->next) {
        free(*head);
        *head = NULL;
        return;
    }

    Node *temp = *head;

    *head = temp->next;
    (*head)->prev = temp->prev;
    temp->prev->next = *head;

    free(temp);
}
Node *getTail(Node *head) {
    if(head == NULL) return NULL;

    Node *tail = head;
    for(; tail->next != head; tail = tail->next);

    return tail;
}
void removeNodeTailOfList(Node *head) {
    if(head == NULL) return;
    
    
    if(head == head->next)
    {
        free(head);
        return;
    }

    Node *tail = getTail(head);
    if(tail == NULL) return;

    tail->prev->next = head;
    head->prev = tail->prev;
    free(tail);
}
void removeNodeFromList(Node **head, int key) {
    if(head == NULL || *head == NULL) return;

    if((*head)->val == key) {
        Node *temp = *head;    
        *head = temp->next;
        (*head)->prev = temp->prev;
        temp->prev->next = *head;

        free(temp);
    }

    Node *searched = searchNodeInList(*head,key);
    if(searched == NULL) return;

    searched->prev->next = searched->next;
    searched->next->prev = searched->prev;
    free(searched);
}
int checkPalandromic(Node *head) {
    if(head == NULL) return 1;

    Node *tail = getTail(head);

    Node *forword = head;
    Node *inward = tail;

    while(forword != inward && forword->prev != inward) {
        if(forword->val != inward->val) return 0;
        forword = forword->next;
        inward = inward->prev;

        if(forword == head ||inward == tail) break;
    }

    return 1;
}
void freeList(Node *head) {
    if(head == NULL) return;

    Node *curr = head;
    
    do {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    } while(curr != head);
}
int main() {
    Node *head = NULL;
    
    insertNodeHeadOfList(&head,5);
    insertNodeHeadOfList(&head,4);
    insertNodeHeadOfList(&head,3);
    insertNodeHeadOfList(&head,2);
    insertNodeHeadOfList(&head,1);

    printf("Elementele din lista : ");
    printList(head);
    printf("Elementele din lista in ordine invers : ");
    printListRev(head);
   
    freeList(head);
    
    return 0;
}
