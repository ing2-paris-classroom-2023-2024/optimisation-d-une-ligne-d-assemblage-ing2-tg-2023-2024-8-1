#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 100
#define MAX_EDGES 100
#define POPULATION_SIZE 100
#define MAX_GENERATIONS 1000

int graph[MAX_VERTICES][MAX_VERTICES];
int numVertices;
int numEdges;
int operations[MAX_VERTICES];

void readExclusions(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d %d", &graph[0][numEdges], &graph[1][numEdges]) == 2) {
        numEdges++;
    }

    fclose(file);
}

void readOperations(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d", &operations[numVertices]) == 1) {
        numVertices++;
    }

    fclose(file);
}

bool isSafe(int v, int c, int color[]) {
    for (int i = 0; i < numVertices; i++) {
        if (graph[v][i] && c == color[i]) {
            return false;
        }
    }
    return true;
}

int countConflicts(int color[]) {
    int conflicts = 0;
    for (int i = 0; i < numEdges; i++) {
        int operationA = graph[0][i];
        int operationB = graph[1][i];
        if (color[operationA] != 0 && color[operationA] == color[operationB]) {
            conflicts++;
        }
    }
    return conflicts;
}

void printSolution(int color[]) {
    printf("Optimal distribution of operations:\n");

    for (int i = 0; i < numVertices; i++) {
        printf("Operation %d assigned to Poste %d\n", operations[i], color[i]);
    }
}

void initializePopulation(int population[][MAX_VERTICES], int numWorkstations) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < numVertices; j++) {
            population[i][j] = rand() % numWorkstations + 1;
        }
    }
}

int fitness(int color[]) {
    return countConflicts(color);
}

void crossover(int parent1[], int parent2[], int child[]) {
    int crossoverPoint = rand() % numVertices;

    for (int i = 0; i < crossoverPoint; i++) {
        child[i] = parent1[i];
    }

    for (int i = crossoverPoint; i < numVertices; i++) {
        child[i] = parent2[i];
    }
}

void mutate(int child[], int numWorkstations) {
    int mutationPoint = rand() % numVertices;
    child[mutationPoint] = rand() % numWorkstations + 1;
}

void geneticAlgorithm(int numWorkstations) {
    int population[POPULATION_SIZE][MAX_VERTICES];
    int bestSolution[MAX_VERTICES];
    int bestFitness = numEdges + 1;

    initializePopulation(population, numWorkstations);

    clock_t start_time = clock();

    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            int parent1 = rand() % POPULATION_SIZE;
            int parent2 = rand() % POPULATION_SIZE;

            crossover(population[parent1], population[parent2], population[i]);

            if (rand() % 100 < 5) { // 5% chance of mutation
                mutate(population[i], numWorkstations);
            }

            int currentFitness = fitness(population[i]);
            if (currentFitness < bestFitness) {
                bestFitness = currentFitness;
                for (int j = 0; j < numVertices; j++) {
                    bestSolution[j] = population[i][j];
                }
            }
        }
    }

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printSolution(bestSolution);
    printf("Time taken: %f seconds\n", elapsed_time);
}

int main() {
    readExclusions("exclusions.txt");
    readOperations("sommets.txt");

    printf("Number of operations: %d\n", numVertices);

    printf("Calculating optimal distribution using Genetic Algorithm...\n");

    // You can adjust the number of workstations as needed
    int numWorkstations = 3;

    geneticAlgorithm(numWorkstations);

    return 0;
}
