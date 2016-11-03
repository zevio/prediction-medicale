/**
 *\file main.c
 *\brief Fonction principale du programme
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
#include "contextemedical.h"
#include "prediction.h"
#include "predictionpartielle.h"


int main(int argc, char *argv[]) {	
	char* motif=creermotif(argc, argv); // permet de recuperer le motif du patient passe en parametre du programme
	if (motif != NULL) {
		int contexte = lirecontexte(*argv[1]); // permet de recuperer le contexte medical passe en parametre du programme
		size_t taille=0; // taille du motif du patient
		int* tab_motif = NULL; // motif du patient sous forme de tableau d'entiers : chaque entier correspond a l'identifiant d'une hospitalisation
		tab_motif = liremotif(motif, &taille); // permet de remplir le tableau d'hospitalisations en lisant le motif passe en parametre du programme
		if(taille>3) { // cas ou le patient a subi plus de 3 hospitalisations
			printf("Le patient a subi plus de trois hospitalisations.\n La prediction ne portera que sur l'analyse des trois dernieres hospitalisations.\n");
			limitermotif(tab_motif,&taille); // on limite le motif aux trois dernieres hospitalisations du patient
		} 
		printf("******************** Prediction ********************\n");
		int i = prediction(contexte, tab_motif, &taille); // prediction sur le motif du patient : on recupere le nombre d'occurrences du motif du patient dans la base sequentielle
		if(i==0) { // si aucune prediction n'est possible sur le motif du patient (si aucune correspondance exacte du motif du patient n'est suivie d'une nouvelle hospitalisation dans la base sequentielle)
			printf("Aucune correspondance complete n'a ete trouvee. Une prediction partielle va etre realisee.\n");
			predictionpartielle(contexte, tab_motif, &taille); // on realise une prediction partielle (sur des correspondances partielles avec le motif du patient)
		}
		if(tab_motif != NULL) {
			free(tab_motif); // liberation memoire
		}
		free(motif); // liberation memoire
	}
	return 0;
}
