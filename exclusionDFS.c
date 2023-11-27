#include <stdio.h>
#include <stdlib.h>

// Fonction pour lire les exclusions à partir d'un fichier texte
void readExclusionsFromFile(char* filename, int* numOperations, int*** exclusions) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de la lecture du fichier d'exclusions.\n");
        exit(EXIT_FAILURE);
    }

    int op1, op2, maxOp = 0;
    while (fscanf(file, "%d %d\n", &op1, &op2) == 2) {
        if (op1 > maxOp) maxOp = op1;
        if (op2 > maxOp) maxOp = op2;
    }

    *numOperations = maxOp;

    // Allouer de la mémoire pour la matrice d'exclusions
    *exclusions = (int**)calloc((*numOperations), sizeof(int*));
    for (int i = 0; i < *numOperations; i++) {
        (*exclusions)[i] = (int*)calloc((*numOperations), sizeof(int));
    }

    // Remplir la matrice d'exclusions
    fseek(file, 0, SEEK_SET);  // Réinitialiser la position du fichier au début
    while (fscanf(file, "%d %d\n", &op1, &op2) == 2) {
        (*exclusions)[op1 - 1][op2 - 1] = 1;
        (*exclusions)[op2 - 1][op1 - 1] = 1;  // Symétrique, car l'exclusion est réciproque
    }

    fclose(file);
}

// Fonction pour afficher la matrice de compatibilité de manière spécifiée
void printCompatibilityMatrix(int numOperations, int** exclusions) {
    int hasConstraint[numOperations];
    for (int i = 0; i < numOperations; i++) {
        hasConstraint[i] = 0;
        for (int j = 0; j < numOperations; j++) {
            if (exclusions[i][j] == 1) {
                hasConstraint[i] = 1;
                break;
            }
        }
    }

    printf("     ");
    for (int i = 0; i < numOperations; i++) {
        if (hasConstraint[i]) {
            printf("%2d ", i + 1);
        }
    }
    printf("\n");

    for (int i = 0; i < numOperations; i++) {
        if (hasConstraint[i]) {
            printf("%2d   ", i + 1);
            for (int j = 0; j < numOperations; j++) {
                if (hasConstraint[j]) {
                    printf("%2d ", exclusions[i][j]);
                }
            }
            printf("\n");
        }
    }
}

// Fonction pour libérer la mémoire allouée à la matrice d'exclusions
void freeExclusionsMatrix(int numOperations, int*** exclusions) {
    for (int i = 0; i < numOperations; i++) {
        free((*exclusions)[i]);
    }
    free(*exclusions);
}

int main() {
    int numOperations;
    int** exclusions;

    char filename[] = "exclusions.txt";

    // Lire les exclusions à partir du fichier et obtenir le nombre d'opérations
    readExclusionsFromFile(filename, &numOperations, &exclusions);

    // Afficher la matrice de compatibilité
    printCompatibilityMatrix(numOperations, exclusions);

    // Libérer la mémoire allouée à la matrice d'exclusions
    freeExclusionsMatrix(numOperations, &exclusions);

    return 0;
}
