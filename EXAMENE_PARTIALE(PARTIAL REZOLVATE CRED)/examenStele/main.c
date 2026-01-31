#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum {
	NAME_SIZE = 50,
	BUFF_SIZE = 256,
	T_SIZE = 10000
};

typedef struct Planet {
	char planetName[NAME_SIZE];
	int	 resources;
} Planet;

typedef struct PlanetNode {
	Planet *key;
	struct PlanetNode *next;
} PlanetNode;

typedef struct Star {
	char starName[NAME_SIZE];
	int distance;
	int totalResources;
	int numPlanets;
	PlanetNode *planetList;
} Star;

typedef struct StarNode {
	Star *key;
	struct StarNode *next;
} StarNode;

typedef struct Galaxy {
	char galaxyName[NAME_SIZE];
	int numStars;
	StarNode *starList[T_SIZE];
	bool bloomFilter[T_SIZE];
} Galaxy;

typedef struct Vertex {
	Star *star;
	struct Vertex **neighbors;
	int numNeigbors;
} Vertex;

typedef struct GraphVertex {
	Vertex **neighbors;
	int numNodes;
	int **weights;
} GraphVertex;

	

int hash(const char* s) {
	int hash = 0;
	for (; *s; s++) {
		hash += (int)*s;
		hash %= T_SIZE;
	}
	return hash;
}

bool* constructBloomFromHashTable(Galaxy* galaxy) {
	if(!galaxy)
		return NULL;
	bool *bloomFIlter = (bool*)calloc(T_SIZE, sizeof(bool));

	for (int i = 0; i < T_SIZE; i++) {
		StarNode *curr = galaxy->starList[i];

		while (curr) {
			Star *currStar = curr->key;

			PlanetNode *currPlanetNode = currStar->planetList;

			while (currPlanetNode) {
				Planet *currPlanet = currPlanetNode->key;
				bloomFIlter[hash(currPlanet->planetName)] = 1;
				currPlanetNode = currPlanetNode->next;
			}

			curr = curr->next;
		}
	}

	return bloomFIlter;
}

Planet* createNewPlanet(const char* name, int resources) {
	if(!name)
		return NULL;
	Planet *temp = (Planet*)malloc(sizeof(Planet));
	temp->resources = resources;
	strcpy(temp->planetName, name);

	return temp;
}
PlanetNode* createNewPlanetNode(Planet* key) {
	if(!key)
		return NULL;
	PlanetNode *temp = (PlanetNode*)malloc(sizeof(PlanetNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void insertPlanetinPlanetList(PlanetNode** head, Planet* key) {
	if(!key)
		return;
	PlanetNode *temp = createNewPlanetNode(key);
	temp->next = *head;
	*head = temp;
}
void printPlanetList(PlanetNode* head) {
	if(!head)
		return;
	PlanetNode *curr = head;
	while (curr) {
		Planet *currPlanet = curr->key;
		printf("%s %d, ", currPlanet->planetName, currPlanet->resources);
		curr = curr->next;
	}
}


Star* createNewStar(const char* name, const char* filepath) {
	if(!name) 
		return NULL;
	FILE *f = fopen(filepath, "r");
	if (!f) {
		printf("Invalid filepath %s sent as input\n", filepath);
		return NULL;
	}

	Star *temp = (Star*)malloc(sizeof(Star));
	
	strcpy(temp->starName, name);

	temp->planetList = NULL;
	temp->totalResources = 0;

	fscanf(f, "%d", &temp->distance);
	fscanf(f, "%d", &temp->numPlanets);

	for (int i = 0; i < temp->numPlanets; i++) {
		char planetName[NAME_SIZE];
		int resources;

		fscanf(f, "%s %d", planetName, &resources);
		Planet *newPlanet = createNewPlanet(planetName,resources);
		insertPlanetinPlanetList(&temp->planetList, newPlanet);
		
		temp->totalResources += newPlanet->resources;
	}

	fclose(f);
	return temp;
}
StarNode* createStarNode(Star* key) {
	if(!key)
		return NULL;
	StarNode *temp = (StarNode*)malloc(sizeof(StarNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void inserStarInList(StarNode** head, Star* key) {
	if(!key)
		return;
	StarNode *temp = createStarNode(key);
	temp->next = *head;
	*head = temp;
}
void printStarList(StarNode* head) {
	if(!head)
		return;
	StarNode *curr = head;
	while (curr) {
		Star *currentStar = curr->key;
		printf("Star name : %s\n", currentStar->starName);
		printf("Star num of planets : %d\n", currentStar->numPlanets);
		printf("Star total resources : %d\n", currentStar->totalResources);
		printf("Print Distance to Sun : %d\n", currentStar->distance);
		printf("Planet list : ");
		printPlanetList(currentStar->planetList);
		printf("\n\n");

		curr = curr->next;
	}
}


Galaxy* createGalaxyFromFile(const char* filepath) {
	FILE *f = fopen(filepath, "r");
	if (!f) {
		printf("Invalid filepath sent as input\n");
		return NULL;
	}

	Galaxy *galaxy = (Galaxy*)malloc(sizeof(Galaxy));
	memset(galaxy->starList,0, sizeof(galaxy->starList));

	char galaxyName[NAME_SIZE];
	strcpy(galaxyName,filepath);
	strtok(galaxyName,".txt");
	strcpy(galaxy->galaxyName,galaxyName);
	
	fscanf(f,"%d", &galaxy->numStars);


	char starName[NAME_SIZE];
	char starFilePath[NAME_SIZE];

	for (int i = 0; i < galaxy->numStars; i++) {
		fscanf(f, "%s", starName);

		strcpy(starFilePath,starName);
		strcat(starFilePath, ".txt");

		int index = hash(starName);

		Star *newStar = createNewStar(starName,starFilePath);
		inserStarInList(&galaxy->starList[index],newStar);
	}

	fclose(f);
	return galaxy;
}
void printEntireGalaxy(Galaxy* galaxy) {
	if(!galaxy || !galaxy->starList)
		return;
	printf("Showing detalis for galaxy %s\n", galaxy->galaxyName);

	for (int i = 0; i < T_SIZE; i++) {
		StarNode *curr = galaxy->starList[i];
		if(curr)
			printStarList(curr);
	}
}

#pragma region SearchFunctions


StarNode* searchStarInList(StarNode* head, const char* name) {
	if (!head || !name)
		return NULL;
	StarNode* temp = head;
	while (temp) {
		if (strcmp(temp->key->starName, name) == 0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}
Star* searchStarByName(Galaxy* galaxy, const char* name) {
	if(!galaxy || !galaxy->starList)
		return NULL;
	if (!name)
	{
		printf("Invalid name sent as input for searchStarByName function\n");
		return NULL;
	}
	int index = hash(name);
	StarNode *searched = searchStarInList(galaxy->starList[index],name);
	
	return searched ? searched->key : NULL;
}
StarNode* getBeforeStarNode(StarNode* head, const char* name) {
	if(!head || !name)
		return NULL;
	StarNode *curr = head;
	while (curr->next) {
		if(strcmp(curr->next->key->starName,name) == 0)
			return curr;
		curr = curr->next;
	}
	return NULL;
}


PlanetNode* searchPlanetInList(PlanetNode* head, const char* name) {
	if(!head || !name)
		return NULL;
	PlanetNode *curr = head;
	while (curr) {
		if(strcmp(curr->key->planetName, name) == 0)
			return curr;
		curr = curr->next;
	}
	return NULL;
}
PlanetNode* getBeforePlanetNode(PlanetNode* head, const char* name) {
	if(!head || !name)
		return NULL;
	PlanetNode *curr = head;
	while (curr->next) {
		if(strcmp(curr->next->key->planetName, name) == 0)
			return curr;
		curr = curr->next;
	}
	return curr;
}
Planet* searchPlanetByPlanetStarName(Galaxy* galaxy, const char* starName, const char* planetName) {
	if(!galaxy || !galaxy->starList)
		return NULL;
	if (!starName || !planetName)
	{
		printf("Invalid star name | planet name sent as input to searchPlanetByPlanetStarName function\n");
		return NULL;
	}

	Star *lookupStar = searchStarByName(galaxy,starName);
	if (!lookupStar) {
		printf("Star %s was not found in galaxy (searchPlanetByPlanetStarName function)\n", starName);
		return NULL;
	}

	PlanetNode *lookupPlanet = searchPlanetInList(lookupStar->planetList,planetName);
	
	return lookupPlanet ? lookupPlanet->key : NULL;
}

#pragma endregion

#pragma region freeFunction
void freePlaneList(PlanetNode* head) {
	while (head) {
		PlanetNode *temp = head;
		head = head->next;
		free(temp->key);
		free(temp);
	}
}
void freeStar(Star *star) {
	if(!star)
		return;
	freePlaneList(star->planetList);
	free(star);
}
#pragma endregion

#pragma region DeleteFunctions
void deletePlanetbyStarPlanetName(Galaxy* galaxy, const char* starName, const char* planetName) {
	if(!galaxy || !galaxy->starList)
		return;
	if (!starName || !planetName)
	{
		printf("Invalid star name or planet name sent as input to deletePlanetbyStarPlanetName function\n");
		return;
	}

	Star *lookupStar = searchStarByName(galaxy,starName);
	if(!lookupStar) {
		printf("Star %s was not found in galaxy (deletePlanetbyStarPlanetName function)\n",starName);
		return;
	}

	PlanetNode* prevNode = getBeforePlanetNode(lookupStar->planetList, planetName);
	PlanetNode* toDelete = NULL;

	if (prevNode == NULL) {
		toDelete = lookupStar->planetList;
		if (toDelete == NULL)
			return; 
		lookupStar->planetList = toDelete->next;
	}
	else {
		toDelete = prevNode->next;
		if (toDelete == NULL)
			return; 
		prevNode->next = toDelete->next;
	}

	lookupStar->totalResources -= toDelete->key->resources;

	free(toDelete->key);
	free(toDelete);
}
void deleteStarFromGalaxy(Galaxy* galaxy, const char* name) {
	if(!galaxy || !galaxy->starList)
		return;
	if (!name)
	{
		printf("Invalid star name sent as input to deleteStarFromGalaxy function\n");
		return;
	}

	int index = hash(name);
	StarNode *head = galaxy->starList[index];

	
	if (strcmp(head->key->starName, name) == 0) {
		galaxy->starList[index] = head->next;
		freeStar(head->key);
		free(head);
		galaxy->numStars -= 1;
		return;
	}

	
	StarNode *beforeNode = getBeforeStarNode(head,name);

	StarNode *temp = beforeNode->next;
	temp->next = temp->next->next;

	freeStar(temp->key);
	free(temp);
	galaxy->numStars -= 1;
}
#pragma endregion


#pragma region Graph
GraphVertex* readGraphVertexFromFile(Galaxy* galaxy, const char* fileapth) {

}

#pragma endregion
int main() {
	const char *filepath = "MilkyWay.txt";
	const char *graph_filepath = "graph.txt";
	Galaxy *galaxy = createGalaxyFromFile(filepath);

	Star *searched = searchStarByName(galaxy,"Sun");
	Star *searched_1 = searchStarByName(galaxy, "Nope");

	printEntireGalaxy(galaxy);

	searched ? printf("Steaua cu numele sun exista in galaxy\n") : printf("Steaua cu numele sunt nu exista in galaxie\n");
	searched_1 ? printf("Steaua cu numele Nope exista in galaxy\n") : printf("Steaua cu numele Nope nu exista in galaxie\n");

	Planet *searchedPlanet = searchPlanetByPlanetStarName(galaxy,"Sun","Mercur");
	Planet *searchedPlanet_1 = searchPlanetByPlanetStarName(galaxy,"Sun","Maxima");


	searchedPlanet ? printf("\nPlaneta Mercur exista in steaua Sun\n") : printf("\nPlaneta Mercur nu exista in Sun\n");
	searchedPlanet_1 ? printf("Planeta Maxima exista in steaua Sun\n") : printf("Planeta Maxima nu exista in Sun\n");

	printf("\n");

	deletePlanetbyStarPlanetName(galaxy, "Sun", "Mercur");
	deletePlanetbyStarPlanetName(galaxy, "Sun", "Terra");

	printEntireGalaxy(galaxy);

	bool *bloomFliter = constructBloomFromHashTable(galaxy);
	
	bloomFliter[hash("Venus")] ? printf("Planeta Venus exista in galaxie\n") : printf("Planeta Venus nu exista in galaxie\n");
	bloomFliter[hash("Mercurs")] ? printf("Planeta Mercur exista in galaxie\n") : printf("Planeta Mercur nu exista in galaxie\n");

	return 0;
}