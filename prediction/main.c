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
	char *motif=creermotif(argc, argv); // permet de recuperer le motif du patient passe en parametre du programme
	if (motif != NULL) {
		int contexte = conversionchar(*argv[1]); // permet de recuperer le contexte medical passe en parametre du programme
		if((strlen(motif)/2)>3) { // cas ou le patient a subi plus de 3 hospitalisations
			printf("Le patient a subi plus de trois hospitalisations.\n La prediction ne portera que sur l'analyse des trois dernieres hospitalisations.\n");
			motif=limitermotif(motif); // on limite le motif aux trois dernieres hospitalisations du patient
			printf("La prediction porte sur le motif suivant : %s\n", motif);
		}
		int i = prediction(contexte, motif); // prediction sur le motif du patient : on recupere le nombre d'occurrences du motif du patient dans la base sequentielle 
		if(i==0) { // si le motif du patient n'existe pas dans la base sequentielle
			printf("Aucune correspondance complete n'a ete trouvee. Une prediction partielle va etre realisee.\n");
			predictionpartielle(contexte,motif); // on realise une prediction partielle
		}
	}
}
