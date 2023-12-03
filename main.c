//Chaque fichier que vous pouvez trouver contient une contrainte ou une combinaison de contrainte
// La première afficher est l'exclusion


#include <stdio.h>
#include <stdlib.h>

typedef struct { // Structure d'une station
    int nb_sommets;      // Nombre de sommet
    int sommets[35];    // Liste des sommets
} Station;

//les sommets correspondent aux operations
int Exclure(int sommet, int op, int E[][2], int nbContraintes){ // Fonction qui verifie si deux sommets sont exclues l'une de l'autre
    for (int i = 0; i < nbContraintes; i++) {
        if ((E[i][0] == sommet && E[i][1] == op) || (E[i][0] == op && E[i][1] == sommet)) {  // Les sommets sont exclues si ils forment une paire
            return 1; // Retourne 1 si les sommets sont exclues
        }
    }
    return 0; // Retourne 0 si les sommets ne sont pas exclues
}

int Repartir_stations(int taille, int E[][2], int nbContraintes, int sommets[], Station stations[]){// Fonction qui va repartir les sommets dans les stations tout en respectant la contrainte d'exclsion
    int nb_stations = 0;
    for (int i = 0; i < taille; i++) {    // Parcours des sommets
        int sommet = sommets[i];
        int attribue_sommet = 0;// permet de savoir si le sommet est attribue a une station

        for (int j = 0; j < nb_stations; j++) {// parcours des stations existantes
            int interieur_station = 1;

            for (int k = 0; k < stations[j].nb_sommets; k++) {  // parcourir les stations pour voir si on peut l'ajouter quelque part
                if (Exclure(sommet, stations[j].sommets[k], E, nbContraintes)){
                    interieur_station = 0;
                    break;
                }
            }
            if (interieur_station) { // si le sommet peut etre ajoute on l'ajoute
                stations[j].sommets[stations[j].nb_sommets++] = sommet;
                attribue_sommet = 1;
                break;
            }
        }
        if (!attribue_sommet) { // si on a trouve aucune station on en cree une nouvelle
            stations[nb_stations].sommets[0] = sommet;
            stations[nb_stations].nb_sommets = 1;
            nb_stations++;
        }
    }
    return nb_stations; // Retourne le nombre total de stations créées
}

int main() {
    int taille;// Taille du graphe

    FILE *fsommet = fopen("Sommets.txt", "r");// ouvre le fichier Sommets.txt
    if (fsommet == NULL) {
        perror("Erreur lors de l'ouverture du fichier des sommets");
        return 1;
    }
    fscanf(fsommet, "%d", &taille);// lit la taille
    int sommets[taille];

    for (int i = 0; i < taille; i++) {   // lit  les sommets
        fscanf(fsommet, "%d", &sommets[i]);
    }
    fclose(fsommet);

    int E[taille * (taille - 1) / 2][2]; // Contraintes d'exclusion
    int nb_contraintes = 0;

    FILE *fexclusions = fopen("exclusions.txt", "r");// ouvre le fichier d'exclusion
    if (fexclusions == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'exclusions");
        return 1;
    }
    while (fscanf(fexclusions, "%d %d", &E[nb_contraintes][0], &E[nb_contraintes][1]) == 2) {//lit les paires qui ne peuvent pas etre dans la meme station
        nb_contraintes++;//comme on sait pas combien il y en a on le fait tant qu'on detecte de valeur à coté
    }
    fclose(fexclusions);// ferme le fichier d'exclusion

    Station stations[taille]; // Tableau pour stocker les stations avec leurs sommets

    int nbStations = Repartir_stations(taille, E, nb_contraintes, sommets, stations); // On appel la fonction qui permet de repartir les sommets


    for (int i = 0; i < nbStations; i++) {// on va afficher les sommets dans les stations
        printf("Station %d : ", i + 1);
        for (int j = 0; j < stations[i].nb_sommets; j++) {
            printf("%d ", stations[i].sommets[j]);
        }
        printf("\n");
    }
    printf("Le nombre minimal de stations est : %d\n", nbStations);// affichage du nombre minimal de station pour ce cas
    return 0;
}
