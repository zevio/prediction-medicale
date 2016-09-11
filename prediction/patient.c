/**
 *\file patient.c
 *\brief Antecedents du patient (motif)
 *\author Stella Zevio
 *\version 1.0
 *\date 2015-06-20
 *
 * Prediction medicale
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"


char *creermotif(int argc, char *argv[]){
  if(argc < 3) { // cas ou le nombre d'arguments n'est pas correct. Il faut au minimum trois arguments : le nom du programme, le contexte medical, et le motif du patient
    fprintf(stderr, "Erreur : nombre d'arguments incorrect. Veuillez ecrire :\n %s contexte motif\n", argv[0]);
    return NULL;
  }
  int context = atoi(argv[1]); // contexte medical du patient
  char* motif = (char*)calloc(128, sizeof(char)); // motif du patient
  if(motif == NULL) {
    return NULL;
  }
  for(size_t i=2; i<argc; i++) {
    strcat(motif, argv[i]); // copie du motif passe en parametre du programme
    strcat(motif, " "); // ajout d'un espace entre chaque evenement du motif
  }
  strcat(motif, "\0");
  printf("Le contexte est : %d\n", context);
  printf("Le motif est: %s", motif);
  printf("\n");
  return motif;
}


char* limitermotif(char* motif){
  char* motifpred = (char*)calloc(6, sizeof(char)); // le motif limite aux trois dernieres hospitalisations du patient
	int i;
	int debut = strlen(motif)-6; // debut de la copie du motif du patient (la 3eme hospitalisation la plus recente)
	int fin = strlen(motif); // fin de la copie du motif du patient (l'hospitalisation la plus recente)
	for (i = debut; i <= fin; i++) {
    motifpred[i-debut] = motif[i]; // copie des trois dernieres hospitalisations
  }
  motifpred[i-debut] = '\0'; // ajout du caractere de fin
  return motifpred;
}
