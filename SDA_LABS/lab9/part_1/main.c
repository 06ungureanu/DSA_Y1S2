#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

enum {
    CHILD_BUFF = 128,
    NAME_BUFF = 64,
    PHONE_BUFF = 11
};

typedef struct trieNode {
    struct trieNode *children[CHILD_BUFF];
    char phone[PHONE_BUFF];
    bool terminal;
} trieNode;

typedef struct Contact {
    char name[NAME_BUFF];
    char phone[PHONE_BUFF];
} Contact;

int countFileLines(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        printf("Error while opening input file\n");
        exit(EXIT_FAILURE);
    }

    char ch;
    int lines = 0;
    while((ch = fgetc(f)) != EOF) {
        if(ch == '\n')
        lines++;
    }
    fclose(f);
    return lines;
}
Contact *readContactsFromFile(const char *filepath, int *total_contacts) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        printf("Error while opening input file\n");
        exit(EXIT_FAILURE);
    }

    int total = countFileLines(filepath);
    Contact *contactList = (Contact*)malloc(total * sizeof(Contact));
    if(contactList == NULL) return NULL;

    char buffer[256];
    int i = 0;
    while(fgets(buffer,256,f) && i < total) {
        strtok(buffer,"\n");
        char name[NAME_BUFF];
        char phone[PHONE_BUFF];

        sscanf(buffer,"%[^,],%s",name,phone);
        strcpy(contactList[i].name,name);
        strcpy(contactList[i].phone,phone);
        i++;
    }

    *total_contacts = i;
    fclose(f);
    return contactList;
}
trieNode *createNode() {
    trieNode *temp = (trieNode*)malloc(sizeof(trieNode));
    if(temp == NULL) return NULL;

    temp->terminal = false;
    for(int i = 0; i < CHILD_BUFF; i++)
        temp->children[i] = NULL;
    
    return temp;
}
void insert(trieNode *root, Contact contact) {
    trieNode *curr = root;
    int lenght = strlen(contact.name);

    for(int i = 0; i < lenght; i++) {
        uint8_t index = contact.name[i];

        if(curr->children[index] == NULL)
            curr->children[index] = createNode();
        curr = curr->children[index];
    }
    curr->terminal = true;
    strcpy(curr->phone,contact.phone);
}
char *search(trieNode *root, const char *name) {
    if(root == NULL) return NULL;

    int lenght = strlen(name);
    trieNode *curr = root;
    for(int i = 0; i < lenght; i++) {
        uint8_t index = name[i];
        if(curr->children[index] == NULL)
            return 0; 
        curr = curr->children[index];
    }

    return (curr->terminal) ? curr->phone : NULL;
}
void printTrieHelper(trieNode *root, char *buffer, int depth) {
    if(root == NULL) return;

    if(root->terminal){
        buffer[depth] = '\0';
        printf("%s -> %s\n", buffer,root->phone);
    }

    for(int i = 0; i < CHILD_BUFF; i++) {
        if(root->children[i] != NULL) {
            buffer[depth] = (char)i;
            printTrieHelper(root->children[i],buffer,depth + 1);
        }
    }
}
void printTrie(trieNode *root) {
    printf("The agenda sorted alphabeticaly\n");
    char buffer[NAME_BUFF];
    printTrieHelper(root,buffer,0);
}
int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Usage : ./program <agenda_file_path>\n");
        exit(EXIT_FAILURE);
    }
    
    const char *filepath = argv[1];
    
    int total_contacts;
    Contact *agenda = readContactsFromFile(filepath,&total_contacts);
    
    trieNode *trie = createNode();
    for(int i = 0; i < total_contacts; i++)
        insert(trie,agenda[i]);
    
    char name[NAME_BUFF];
    printf("Introduce persoane pe care vrei sa o cauti : ");
    fgets(name,NAME_BUFF,stdin);
    strtok(name,"\n");
    
    char *result = search(trie,name);
    if(result)
        printf("Person with name %s has phone number %s\n\n",name,result);
    else 
        printf("Person was not found in agenda\n\n");

    printTrie(trie);
    free(agenda);
    return 0;
}
