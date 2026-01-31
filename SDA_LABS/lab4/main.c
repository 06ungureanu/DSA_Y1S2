#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;

node *createNode(int data)
{
    node *temp = (node*)malloc(sizeof(node));
    if(temp == NULL)
        return NULL;

    temp->data = data;
    temp->next = NULL;
    
    return temp;
}
void insertNodeHeadOfList(node **head, int data)
{
    node *temp = createNode(data);
    if(temp == NULL)
        return;

    temp->next = *head;
    *head = temp;
}
void printList(node *head)
{
    while(head != NULL)
    {
        printf("%d ", head->data);
        head = head->next;
    }

    printf("\n");
}

node *searchNodeInList(node *head, int key)
{
    while(head != NULL)
    {
        if(head->data == key)
            return head;
        head = head->next;
    }
    return NULL;
}
node *searchBeforeNode(node *head, int key)
{
    if(head == NULL || head->next == NULL)
        return NULL;

    while(head->next != NULL)
    {
        if(head->next->data == key)
            return head;
        head = head->next;
    }

    return NULL;
}
void insertNodeBefore(node **head, int data, int key)
{
    
    if(*head == NULL)
        return;

    
    if((*head)->data == key)
    {
        node *temp = createNode(data);
        if(temp == NULL)
            
            return;
        temp->next = *head;
        *head = temp;
        return;
    }
    
    
    node *beforeNode = searchBeforeNode(*head,key);
    if(beforeNode == NULL) 
        
        return;

    node *temp = createNode(data);
    if(temp == NULL)
        
        return;

    temp->next = beforeNode->next;
    beforeNode->next = temp;

    
}
void insertNodeAfter(node *head, int data, int key)
{
    
    if(head == NULL)
        return;

    node *searched = searchNodeInList(head,key);
    if(searched == NULL)
        
        return;

    node *temp = createNode(data);
    if(temp == NULL)
        return;

    temp->next = searched->next;
    searched->next = temp;
}
node *getTailOfList(node *head)
{
    if(head == NULL)
        return NULL;

    while(head->next != NULL)
        head = head->next;

    return head;
}
void insertNodeTailOfList(node **head, int data)
{
    
    node *temp = createNode(data);
    if(temp == NULL)
        return;

    
    if(*head == NULL)
    {
        temp->next = *head;
        *head = temp;
        return;
    }

    node* tail = getTailOfList(*head);
    tail->next = temp;
}
void removeNodeHeadOfList(node **head)
{
    if(*head == NULL)
        return;

    node *temp = *head;
    *head = (*head)->next;
    free(temp);
}
node *getLastBeforeTail(node *head)
{
    
    if(head == NULL || head->next == NULL)
        return NULL;

    while(head->next->next != NULL) 
        head = head->next;

    return head;
}
void removeNodeTailOfList(node **head)
{
   if(*head == NULL)
        return;

    if((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    node *prev = getLastBeforeTail(*head);
    node *temp = prev->next;
    prev->next = NULL;
    free(temp);
}
void removeNodeFromList(node **head, int key)
{
    
    if(*head == NULL)
        return;

    

    if((*head)->data == key)
    {
        node *temp = *head;
        *head = (*head)->next;
        free(*head);
    }
    
    node *prev = searchBeforeNode(*head,key);
    if(prev == NULL) 
        return;

    node *temp = prev->next;
    prev->next = prev->next->next;
    free(temp);
}
void removeList(node **head)
{
    if(head == NULL)
        return;

    node *temp = NULL;
    while(*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    *head = NULL;
}
node *getMid(node *head)
{
    if(head == NULL)
        return NULL;

    node *slow = head;
    node *fast = head->next;

    while(fast != NULL && fast->next != NULL)
    {
        
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
int main()
{
    node *list = NULL;

    for(int i = 5; i > 0; i--)
        insertNodeHeadOfList(&list,i);
   
    removeNodeTailOfList(&list);    
    printList(list);
    
    node *searched = searchNodeInList(list,4);
    if(searched)
        printf("Nodul cu valoarea %d se afla n lista\n", searched->data);
    else 
        printf("Nodul cautat nu se afla n lista\n");
    
    node *mid = getMid(list);
    if(mid)
        printf("Valorea din mijloc : %d\n", mid->data);

    
    removeList(&list);
    return 0;
}
