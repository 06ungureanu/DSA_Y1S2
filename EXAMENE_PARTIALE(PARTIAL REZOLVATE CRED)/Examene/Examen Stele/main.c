#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    NAME_BUFF = 50,
    BUFF_SIZE = 256,
    T_SIZE = 10000
};

typedef struct Planet {
    char                name[NAME_BUFF];
    float               resources;
} Planet;

typedef struct PlanetNode {
    Planet              key;
    struct PlanetNode   *next;
} PlanetNode;

typedef struct Star {
    char                name[NAME_BUFF];
    int                 distance;
    float               totalRes;
    int                 nrPlanets;
    PlanetNode          *PlanetList;
} Star;

typedef struct StarNode {
    Star            *key;
    struct StarNode *next;
} StarNode;

typedef struct Galaxy {
    char            name[NAME_BUFF];
    int             nrStars;
    int             bloomFilter[T_SIZE];
    StarNode        *StarList[T_SIZE];
} Galaxy;

typedef struct PartialSolution {

} PartialSolution;


char *getNameFromFilePath(const char *filepath) {
    if(filepath == NULL) {
        printf("Invalid filepath sent as input to getNameFromFilePath function\n");
        return NULL;
    }

    char *name = strdup(filepath);
    strtok(name,".txt");

    return name;
}


int hash(const char *s) {
    int hash = 0;
    for(; *s; s++) {
        hash += (int)*s;
        hash %= T_SIZE;
    }
    return hash;
}


PlanetNode *createNewPlanetNode(const char *name, float resources) {
    if(name == NULL) return NULL;

    PlanetNode *temp = (PlanetNode*)malloc(sizeof(PlanetNode));

    strcpy(temp->key.name, name);
    temp->key.resources = resources;
    temp->next = NULL;

    return temp;
}
void insertNodeInPlanetList(Galaxy *galaxy,PlanetNode **head, const char *name, float resources) {
    if(name == NULL) return;

    PlanetNode *temp = createNewPlanetNode(name,resources);

    temp->next = *head;
    *head = temp;

    galaxy->bloomFilter[hash(name)] = 1;
}

StarNode *createNewStarNode(Star *key) {
    if(key == NULL) {
        printf("Invalid key sent as input for createNewStarNode\n");
        return NULL;
    }

    StarNode *temp = (StarNode*)malloc(sizeof(StarNode));

    temp->key = key;
    temp->next = NULL;

    return temp;
}
void insertNodeInStarList(StarNode **head, Star *key) {
    StarNode *temp = createNewStarNode(key);

    temp->next = *head;
    *head = temp;
}


Star *createStarFromFile(Galaxy *galaxy, char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        printf("Invalid filepath sent as input for createStarFromFile\n");
        return NULL;
    }

    Star *star = (Star*)malloc(sizeof(Star));
    
    star->PlanetList = NULL;
    star->totalRes = 0;

    char *star_name = getNameFromFilePath(filepath);
    strcpy(star->name, star_name);

    fscanf(f, "%d", &star->distance);
    fscanf(f, "%d", &star->nrPlanets);

    char planet_name[NAME_BUFF];
    float planet_res;
    for(int i = 0; i < star->nrPlanets; i++) {
        fscanf(f,"%s %f", planet_name,&planet_res);
        insertNodeInPlanetList(galaxy,&star->PlanetList,planet_name,planet_res);
        star->totalRes += planet_res;
    }

    free(star_name);
    fclose(f);

    return star;
}
Galaxy *constructGalaxyFromFile(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        printf("Error while opening input filepath\n");
        return NULL;
    }

    Galaxy *galaxy = (Galaxy*)malloc(sizeof(Galaxy));

    char *galaxy_name = getNameFromFilePath(filepath);
    strcpy(galaxy->name, galaxy_name);
    
    fscanf(f,"%d", &galaxy->nrStars);

    char star_filepath[BUFF_SIZE];
    for(int i = 0; i < galaxy->nrStars; i++) {
        fscanf(f, "%s", star_filepath);
        Star *new_star = createStarFromFile(galaxy,star_filepath);

        char *star_name = getNameFromFilePath(star_filepath);
        int index = hash(star_name);

        insertNodeInStarList(&galaxy->StarList[index],new_star);
    
        free(star_name);
    }
    free(galaxy_name);
    fclose(f);

    return galaxy;
}


void printPlanetList(PlanetNode *head) {
    if(head == NULL)
        return;

    PlanetNode *curr = head;
    while(curr) {
        printf("[Name : %s, Resources %.1f], ", curr->key.name,curr->key.resources);

        curr = curr->next;
    }
}
void printStarList(StarNode *head) {
    if(head == NULL)
        return;

    StarNode *curr = head;
    while(curr) {
        Star *star = curr->key;
        printf("Star Name : %s\n", star->name);
        printf("Star Distance from Sun : %d\n", star->distance);
        printf("Star Number of Planets : %d\n", star->nrPlanets);
        printf("Star Total Resources : %.1f\n", star->totalRes);
        printf("Star Planets info : ");
        printPlanetList(star->PlanetList);
        printf("\n");

        curr = curr->next;
    }
    printf("\n");
}
void printEntireGalaxy(Galaxy *galaxy) {
    printf("Galaxy Name : %s\n", galaxy->name);
    printf("Galaxy number of Stars :%d\n\n", galaxy->nrStars);
    
    for(int i = 0; i < T_SIZE; i++) {
        if(galaxy->bloomFilter[i]) 
            printStarList(galaxy->StarList[i]);
    }
}


Star *searchStarInStarList(StarNode *head, const char *name) {
    if(head == NULL || name == NULL)
        return NULL;

    StarNode *curr = head;
    while(curr) {
        if(strcmp(curr->key->name, name) == 0)
            return curr->key;
        curr = curr->next;
    }

    return NULL;
}
void searchStarInGalaxyByName(Galaxy *galaxy, const char *lookUpStarName) {
    if(galaxy == NULL || lookUpStarName == NULL) {
        printf("Invalid parameters sent as input to SearchStarInGalaxyByName\n");
        return;
    }

    int index = hash(lookUpStarName);
    Star *lookup_star = searchStarInStarList(galaxy->StarList[index],lookUpStarName);

    if(lookup_star)
        printf("The star with name %s exist in galaxy with name %s\n", lookUpStarName, galaxy->name);
    else 
        printf("The star with name %s does not exist in galaxy with name %s\n", lookUpStarName, galaxy->name);
}


int searchPlanetInPlanetList(PlanetNode *head, const char *name) {
    if(head == NULL || name == NULL) 
        return 0;
    
    PlanetNode *curr = head;
    while(curr) {
        if(strcmp(curr->key.name,name) == 0)
            return 1;
        curr = curr->next;
    }

    return 0;
}
void searchPlanetByNameStarInGalaxy(Galaxy *galaxy, const char *lookupStarName, const char *lookupPlanetName) {
    if(galaxy == NULL || lookupPlanetName == NULL || lookupPlanetName == NULL) {
        printf("Invalid parametres sent as input to searchPlanetByNameStarInGalaxy\n");
        return;
    }

    int star_index = hash(lookupStarName);
    Star *lookup_star = searchStarInStarList(galaxy->StarList[star_index], lookupStarName);

    if(lookup_star == NULL) {
        printf("Error while searching planet in star : the star %s does not exist in galaxy %s\n", lookupStarName,galaxy->name);
        return;
    }

    if(searchPlanetInPlanetList(lookup_star->PlanetList,lookupPlanetName))
        printf("The planet with name %s exist in star %s galaxy %s\n", lookupPlanetName,lookupStarName,galaxy->name);
    else 
        printf("The planet with name %s does not exist in star %s galaxy %s\n", lookupPlanetName,lookupStarName,galaxy->name);

}



float getPlanetResource(PlanetNode *head, const char *name) {
    if(head == NULL || name == NULL) return 0;

    PlanetNode *curr =  head;
    while(curr) {
        if(strcmp(curr->key.name,name) == 0)
            return curr->key.resources;
        curr = curr->next;
    }
    return 0;
}
PlanetNode *getBeforeNode(PlanetNode *head, const char *name) {
    if(head == NULL || name == NULL) return NULL;

    PlanetNode *before = head;
    while(before->next) {
        if(strcmp(before->next->key.name,name) == 0)
            return before;
        before = before->next;
    }

    return NULL;
}
void deletePlanetInPlanetList(PlanetNode **head, const char *lookupPlanetName) {
    if(head == NULL || lookupPlanetName == NULL)
        return;
    
    
    if(strcmp((*head)->key.name,lookupPlanetName) == 0) 
    {
        PlanetNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    else 
    {
    
        PlanetNode *beforeNode = getBeforeNode(*head,lookupPlanetName);
        
        PlanetNode *temp = beforeNode->next;
        beforeNode->next = beforeNode->next->next;

        free(temp);
    }
}
void deletePlanetByStarName(Galaxy *galaxy, const char *lookupStarName, const char *lookupPlanetName) {
    if(galaxy == NULL || lookupStarName == NULL || lookupPlanetName == NULL) {
        printf("Invalid parametres sent to deletePlanetByStarName\n");
        return;
    }

    int star_index = hash(lookupStarName);
    Star *lookup_star = searchStarInStarList(galaxy->StarList[star_index], lookupStarName);
    
    if(lookup_star == NULL) {
        printf("Error while deleting planet %s from star %s galaxy %s (star was not found)\n", lookupPlanetName,lookupStarName,galaxy->name);
        return;
    }
    if(searchPlanetInPlanetList(lookup_star->PlanetList,lookupPlanetName) == 0) {
        printf("Error while deleting planet %s from star %s galaxy %s (planet was not found)\n", lookupPlanetName,lookupStarName,galaxy->name);
        return;    
    }

    lookup_star->totalRes -= getPlanetResource(lookup_star->PlanetList,lookupPlanetName);
    deletePlanetInPlanetList(&lookup_star->PlanetList,lookupPlanetName);

    printf("Star after planets after deleting %s : ",lookupPlanetName);
    printPlanetList(lookup_star->PlanetList);
    printf("\n");
}


StarNode *getBeforeStarNode(StarNode *head, Star *key) {
    if(head = NULL || key == NULL)
        return NULL;

    StarNode *curr = head;
    while(curr->next) {
        if(curr->next == key)
            return curr;
        curr = curr->next;
    }

    return NULL;
}
void deleteStarInStarList(StarNode **head, Star *key) {
    if(head == NULL || key == NULL) return;

    if((*head)->key == key) {
        StarNode *temp = (*head);
        *head = (*head)->next;
        free(temp);
    }
    else 
    {
        StarNode *before = getBeforeStarNode(*head,key);
        if(before == NULL) 
            return;

        StarNode *temp = before->next;
        before->next = before->next->next;

        free(temp);
    }
}
void deleteStarByName(Galaxy *galaxy, const char *lookupStarName) {
    if(galaxy == NULL || lookupStarName == NULL) {
        printf("Invalid parametres sent as input to deleteStarByName\n");
        return;
    }

    int index = hash(lookupStarName);
    Star *lookup_star = searchStarInStarList(galaxy->StarList[index],lookupStarName);

    if(lookup_star == NULL) {
        printf("Error while deleting star with name %s from galaxy %s (was not found)\n", lookupStarName, galaxy->name);
        return;
    }

    int star_index = hash(lookupStarName);
    if(galaxy->StarList[star_index]) {
        deleteStarInStarList(&galaxy->StarList[index],lookup_star);
        galaxy->bloomFilter[star_index] = 0;
    }
    printf("Galaxy after deleting star %s\n\n", lookupStarName);
    printEntireGalaxy(galaxy);
}


void checkPlanetInGalaxy(Galaxy *galaxy, const char *lookupPlanetName) {
    if(galaxy == NULL || lookupPlanetName == NULL) {
        printf("Invalid parameteres sent to checkPlanetinGalaxy\n");
        return;
    }

    int index = hash(lookupPlanetName);
    if(galaxy->bloomFilter[index]) 
        printf("Planet %s exist in galaxy %s\n", lookupPlanetName,galaxy->name);
    else 
        printf("Planet %s does not exist in galaxy %s\n", lookupPlanetName,galaxy->name);
}




int main() {
    const char *filename = "MilkyWay.txt";

    Galaxy *galaxy = constructGalaxyFromFile(filename);

    printEntireGalaxy(galaxy);

    searchStarInGalaxyByName(galaxy,"Sun");
    searchStarInGalaxyByName(galaxy,"Nope");

    printf("\n");

    searchPlanetByNameStarInGalaxy(galaxy,"Sun","Mercur");
    searchPlanetByNameStarInGalaxy(galaxy,"Sun","Nope");

    printf("\n");

    deletePlanetByStarName(galaxy,"Sun","Mercur");
    deleteStarByName(galaxy,"Pegasus");

    checkPlanetInGalaxy(galaxy,"Venus");
    checkPlanetInGalaxy(galaxy,"Nope");
    return 0;

}