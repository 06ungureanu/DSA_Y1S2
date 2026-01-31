#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char         key;
    struct Node *next;

} Node;

typedef struct List {
    Node        *head;
    Node        *tail;
} List;

typedef struct PartialSolutionList {
    List    *l;
    char    M[3][4];
    int     step;
    int     choice;
} PartialSolutionList;


Node *create_list_node(const char key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if(temp == NULL) 
        return NULL;

    temp->key = key;
    temp->next = NULL;

    return temp;
}
List *create_empty_list() {
    List *l = (List*)malloc(sizeof(List));
    if(l == NULL)
        return NULL;

    l->head = NULL;
    l->tail = NULL;

    return l;
}
void insert_tail_of_list(List *l, const char key) {
    if(l == NULL) return;

    Node *temp = create_list_node(key);

    if(l->head == NULL && l->tail == NULL) {
        l->head = temp;
        l->tail = temp;
        return;
    }

    l->tail->next = temp;
    l->tail = temp;
}


PartialSolutionList initPartialSolution_List() {
    PartialSolutionList PS;


}
void backtrackingList(PartialSolutionList PS) {
    if(canRejectList(PS))
        return;
    if(isSolutionList(PS))
        printSolutionList(PS);
    PS = increaseStepList(PS);
    while(hasChoiceAtStepList(PS)) {
        PS = getChoiceAtStep(PS);
        backtrackingList(PS);
    }
}
int main() {
    char M[3][4] = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };

    char words[5][10] = {"ABCCED","SEE","ABC","ADEE"};
    return 0;
}