#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int val;
    struct node *next;
} node;

typedef struct List 
{
    node *head;
    node *tail;
} List;

node *createNode(int val)
{
    node *temp = (node*)malloc(sizeof(node));
    if(temp == NULL)
        return NULL;

    temp->val = val;
    temp->next = NULL;

    return temp;
}
List *createEmptyList()
{
    List *list = (List*)malloc(sizeof(List));
    if(list == NULL)
        return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}
void insertNodeHeadOfList(List *list, int val)
{
    if(list == NULL)
        return;

    node *temp = createNode(val);
    if(temp == NULL)
        return;

    if(list->head == NULL && list->tail == NULL)
    {
        list->head = temp;
        list->tail = temp;
        return;
    }

    temp->next = list->head;
    list->head = temp;
}
void printList(List *list)
{
    if(list == NULL)
        return;

    for(node *p = list->head; p != NULL; p = p->next)
        printf("%d ", p->val);
    printf("\n");
}
node *searchNodeInList(List *list, int key)
{
    if(list == NULL || list->head == NULL)
        return NULL;

    node *temp = list->head;
    while(temp != NULL)
    {
        if(temp->val == key)
            return temp;
        temp = temp->next;
    }
    
    return NULL;
}
node *getPrevNode(List *list, int key)
{
    if(list == NULL || list->head == NULL)
        return NULL;

    node *temp = list->head;
    while(temp->next != NULL)
    {
        if(temp->next->val == key)
            return temp;

        temp = temp->next;
    }

    return NULL;
}
void insertNodeBefore(List *list, int val, int key)
{
    if(list == NULL || list->head == NULL)
        return;

    node *temp = createNode(val);
    if(temp == NULL)
        return;

    if(list->head->val == key)
    {
        temp->next = list->head;
        list->head = temp;
        return;
    }

    node *prev = getPrevNode(list,key);
    if(prev == NULL)
    {
        free(temp);
        return;
    }

    temp->next = prev->next;
    prev->next = temp;
}
void insertNodeAfter(List *list, int val, int key)
{
    if(list == NULL || list->head == NULL)
        return;

    node *curr = searchNodeInList(list,key);
    if(curr == NULL)
        return;
    
    node *temp = createNode(val);
    if(temp == NULL)
        return;
    
    temp->next = curr->next;
    curr->next = temp;

    if(list->tail == curr)
        list->tail = temp;
}
void insertNodeTailOfList(List *list, int val)
{
    if(list == NULL)
        return;

    node *temp = createNode(val);
    if(temp == NULL)
        return;

    if(list->head == NULL && list->tail == NULL)
    {
        list->head = temp;
        list->tail = temp;
        return;
    }

    list->tail->next = temp;
    list->tail = temp;
}
void removeNodeHeadOfList(List *list)
{
    if(list == NULL || list->head == NULL)
        return;

    node *temp = list->head;
    list->head = list->head->next;
    
    free(temp);
    if(list->head == NULL)
        list->tail = NULL;
}
node *getPrevTailNode(List *list)
{
    if(list == NULL || list->head == NULL)
        return NULL;

    node *lastPrevTail = list->head;
    for(;lastPrevTail->next != NULL && lastPrevTail->next->next != NULL; lastPrevTail = lastPrevTail->next);
    return lastPrevTail;
}
void removeNodeTailOfList(List *list)
{
    if(list == NULL || list->head == NULL)
        return;

    if(list->head == list->tail)
    {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return;
    }

    node *prev = getPrevTailNode(list);
    if(prev == NULL)
        return;

    prev->next = NULL;
    free(list->tail);
    list->tail = prev;
}
void removeNodeFromList(List *list, int key)
{
    if(list == NULL || list->head == NULL)
        return;
    
    
    if(list->head->val == key)
    {
        node *temp = list->head;
        list->head = list->head->next;

        if(list->head == NULL)
            list->tail = NULL;

        free(temp);
        return;
    }

    node *prev = getPrevNode(list,key);
    if(prev == NULL)
        return;

    node *temp = prev->next;
    prev->next = prev->next->next;

    
    if(prev->next == NULL)
        list->tail = prev;

    free(temp);
}
void removeList(List *list)
{
    if(list == NULL || list->head == NULL)
        return;

    while(list->head != NULL)
    {
        node *temp = list->head;
        free(temp);
        list->head = list->head->next;
    }

    list->tail = NULL;
    free(list);
}
node *getMid(List *list)
{
    if(list == NULL || list->head == NULL)
        return NULL;

    
    node *slow = list->head;
    node *fast = list->head->next; 
    
    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
int main(void)
{
    List *list = createEmptyList();
    
    insertNodeHeadOfList(list,5);
    insertNodeHeadOfList(list,4);
    insertNodeHeadOfList(list,3);
    insertNodeHeadOfList(list,2);
    insertNodeHeadOfList(list,1);

    printList(list);
    
    node *mid = getMid(list);
    printf("List mid value : %d\n", mid->val);

    removeList(list);
    return 0;
}
