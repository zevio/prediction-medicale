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


char* creermotif(int argc, char *argv[]) {
  if(argc < 3) { // cas ou le nombre d'arguments n'est pas correct. Il faut au minimum trois arguments : le nom du programme, le contexte medical, et le motif du patient
    fprintf(stderr, "Erreur : nombre d'arguments incorrect. Veuillez ecrire :\n %s contexte motif\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  char* motif = (char*)calloc(128, sizeof(char)); // motif du patient
  if(motif == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation memoire du motif du patient.\n");
    exit(EXIT_FAILURE);
  }
  for(size_t i=2; i<argc; i++) {
    strcat(motif, argv[i]); // copie du motif passe en parametre du programme
    strcat(motif, " "); // ajout d'un espace entre chaque evenement du motif
  }
  strcat(motif, "\0"); // caractere terminal pour le motif
  return motif;
}


int* liremotif(char *motif, size_t* taille) {
  char* hospitalisation = NULL; // hospitalisation courante dans le motif
  int * tab_motif = NULL; // motif sous forme de tableau d'hospitalisations
  hospitalisation = (char*)malloc(sizeof(char)*1); 
  tab_motif = (int*) malloc(sizeof(int) *2);
  hospitalisation[0]='\0';
  size_t j=0, l=1;
  for(size_t i=0; i<strlen(motif)+1; i++) { // lecture du motif
    if(motif[i]==' ' || motif[i]=='\0') { // cas ou on a fini de lire le motif, ou cas ou on lit une nouvelle hospitalisation dans le motif
      if(hospitalisation!=NULL && hospitalisation!='\0' && strcmp(hospitalisation, "")!=0) { // cas ou l'hospitalisation courante n'est pas vide
        tab_motif = (int*) realloc(tab_motif, sizeof(int)*(l+1)); // augmentation du tableau d'hospitalisations
        if(tab_motif == NULL) { // cas ou l'augmentation a echoue
          fprintf(stderr, "Erreur lors de l'allocation memoire du tableau d'hospitalisations correspondant au motif du patient (1)\n");
          exit(EXIT_FAILURE);
        }
        tab_motif[j]=atoi(hospitalisation); // ajout de l'hospitalisation dans le tableau d'hospitalisations 
        free(hospitalisation); // liberation memoire
        hospitalisation = NULL;
        hospitalisation = (char*) malloc(sizeof(char)*1); // on realloue la memoire pour pouvoir traiter une nouvelle hospitalisation
        hospitalisation[0]='\0';
        j++;l++;
      }
      free(hospitalisation); // liberation memoire
      hospitalisation = NULL;
      hospitalisation = (char*) malloc(sizeof(char)*1);
      hospitalisation[0]='\0';
    }
    else { // cas ou l'hospitalisation lue est un entier qui comporte au moins deux chiffres
      hospitalisation = (char*) realloc(hospitalisation, sizeof(char*)*(strlen(hospitalisation)+1));
      if(motif[i]!=' ') { 
        sprintf(hospitalisation, "%s%c", hospitalisation,motif[i]); // lecture de l'hospitalisation en cours
      }
    }
  }
  tab_motif = (int*) realloc(tab_motif, sizeof(int)*(l+1));
  if(tab_motif == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation memoire du tableau d'hospitalisations correspondant au motif du patient (2)\n");
    exit(EXIT_FAILURE);
  }
  tab_motif[j]='\0'; // caractere final pour le motif
  free(hospitalisation); // liberation memoire
  *taille=j;
  return tab_motif;
}

  
void limitermotif(int* tab_motif, size_t *taille) {  
  int h1=tab_motif[*taille-3]; // premiere hospitalisation
  int h2=tab_motif[*taille-2]; // seconde hospitalisation
  int h3=tab_motif[*taille-1]; // troisieme et derniere hospitalisation
  *taille=3; // on se limite aux trois dernieres hospitalisations
  tab_motif = (int*)realloc(tab_motif, sizeof(int)*3); // on realloue le tableau d'hospitalisations 
  tab_motif[0]=h1; // ajout de la premiere hospitalisation dans le tableau
  tab_motif[1]=h2; // ajout de la seconde hospitalisation dans le tableau
  tab_motif[2]=h3; // ajout de la troisieme et derniere hospitalisation dans le tableau
}
