/**
 *\file predictionpartielle.c
 *\brief Prediction partielle sur un motif de patient (cas ou il n'existe pas de correspondance exacte entre le motif du patient et la base sequentielle)
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
 
#include "pile.h" 
#include "patient.h" 
#include "prediction.h"
#include "predictionpartielle.h"


int scoremax(int contexte, int* motif_1, int* motif_2, int* motif_3){
	size_t taille = 2;
	int score_max = score(contexte, motif_1, &taille); // le score maximum est le score du motif 1
	int score_2 = score(contexte, motif_2, &taille); // score du motif 2
	int score_3 = score(contexte, motif_3, &taille); // score du motif 3
	if(score_2>score_max) { // cas ou le motif 2 a un score strictement superieur au motif 1
		score_max = score_2; 
	}
	if(score_3>score_max) { // cas ou le motif 3 a un score strictement superieur au score maximum
		score_max = score_3;
	}
	return score_max; // score maximum
}


pile* remplirListePredictions2(int contexte, int* motif){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int i=0;
	int j=0;
	pile* listePredictions = NULL; // liste des predictions
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant la base sequentielle
	if (fichier != NULL) {
		char ligne [ 128 ]; // taille max d'une ligne
		while ((s= fgets(ligne, sizeof ligne, fichier))!= NULL) { // lecture d'une ligne 
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du bon contexte medical
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un contexte qui n'est pas encore le bon contexte medical
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de lecture d'un motif du contexte
				size_t taillecourant = 0;
				int* motiftemp=liremotif(ligne, &taillecourant); // lecture du motif courant dans le contexte
				int correspondance=0; // nombre de correspondances entre le motif courant dans le contexte et le motif du patient
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=0;
				int prediction=-1; // hospitalisation predite
				for(i=0;i<2;i++){
					for(j=indice;j<taillecourant;j++){
						if(motif[i]==motiftemp[j]){
							correspondance++; // calcul des correspondances entre le motif courant et le motif du patient
							indice=j+1;
						}
					}
				}
				if(correspondance==2){ // si le motif courant contient le motif du patient
					correspond=1; // le motif courant correspond
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins une autre hospitalisation a eu lieu a la suite du motif du patient)
					for(i=indice; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacune des hospitalisations ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque hospitalisation ayant eu lieu est une prediction
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return listePredictions;
}


float calculerProba2(int contexte, int pred, int* motif, int amotif){
	float proba = 0.0; // probabilite de la prediction
	int* motifp = (int*)calloc(3, sizeof(int)); // motifp : groupe de deux hospitalisations + prediction
	motifp[0]=motif[0];
	motifp[1]=motif[1];
	motifp[2]=pred;
	size_t taille_motifp = 3;
	int* amotifp = (int*)calloc(2, sizeof(int)); // amotif : hospitalisation restante + prediction
	amotifp[0]=amotif;
	amotifp[1]=pred;
	size_t taille_amotif = 2;
	size_t taille_motif = 2;
	size_t taille_hospitalisation = 1;
	float score_1 = (float)score(contexte, motifp, &taille_motifp); // score de motifp
	float score_2 = (float)score(contexte,amotifp, &taille_amotif); // score de amotif
	float score_3 = (float)score(contexte,motif, &taille_motif); // score du groupe de deux hospitalisations
	int* h = (int*)calloc(1, sizeof(int));
	h[0]=amotif;
	float score_4 = (float)score(contexte,h, &taille_hospitalisation); // score de l'hospitalisation restante
	proba = (score_1+score_2)/(score_3+score_4); // probabilite de la prediction 
	printf("Probabilite : %f\n", proba);
	return proba; // probabilite de la prediction
}


pile* remplirListePredictions1(int contexte,int* hospitalisation_1, int* hospitalisation_2, int* hospitalisation_3){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int i=0;
	int j=0;
	pile* listePredictions = NULL; // liste des predictions
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant les donnees 
	if (fichier != NULL) {
		char ligne [ 128 ]; // taille max d'une ligne
		while ((s= fgets(ligne, sizeof ligne, fichier))!= NULL) { // lecture d'une ligne 
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du bon contexte medical
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un contexte qui n'est pas encore le bon contexte medical
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de lecture d'un motif du contexte

				size_t taillecourant = 0;
				int* motiftemp=liremotif(ligne, &taillecourant);
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=-1;
				int prediction=-1; // hospitalisation predite
				for(j=0;j<taillecourant;j++){
					if(motiftemp[j]==hospitalisation_1[0] || motiftemp[j]==hospitalisation_2[0] || motiftemp[j]==hospitalisation_3[0]){ 
						correspond=1; // le motif en cours correspond
						indice=j;
						break;
					}
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice+1)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins une autre hospitalisation a eu lieu a la suite du motif du patient)
					for(i=indice+1; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacune des hospitalisations ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque hospitalisation ayant eu lieu est une prediction
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return listePredictions;
}


float calculerProba1(int contexte, int pred, int* hospitalisation_1, int* hospitalisation_2, int* hospitalisation_3){
	float proba = 0.0;
	int* motif_1 = (int*)calloc(2, sizeof(int)); // motif_1 : premiere hospitalisation + prediction
	motif_1[0]=hospitalisation_1[0];
	motif_1[1]=pred;
	int* motif_2 = (int*)calloc(2, sizeof(int)); // motif_2 : deuxieme hospitalisation + prediction
	motif_2[0]=hospitalisation_2[0];
	motif_2[1]=pred;
	int* motif_3 = (int*)calloc(2, sizeof(int)); // motif_3 : troisieme hospitalisation + prediction
	motif_3[0]=hospitalisation_3[0];
	motif_3[1]=pred;
	size_t taille_hospitalisation = 1;
	size_t taille_motif = 2;
	float score_1 = (float)score(contexte, motif_1, &taille_motif); // score du motif_1
	float score_2 = (float)score(contexte,motif_2, &taille_motif); // score du motif_2
	float score_3 = (float)score(contexte,motif_3, &taille_motif); // score du motif_3
	float score_4 = (float)score(contexte,hospitalisation_1, &taille_hospitalisation); // score de la premiere hospitalisation
	float score_5 = (float)score(contexte,hospitalisation_2, &taille_hospitalisation); // score de la deuxieme hospitalisation
	float score_6 = (float)score(contexte,hospitalisation_3, &taille_hospitalisation); // score de la troisieme hospitalisation
	proba = (score_1+score_2+score_3)/(score_4+score_5+score_6); // probabilite de la prediction 
	return proba; // probabilite de la prediction
}

void predictionpartielle_motif_3_hospitalisations(int contexte, int* motif){
	/**************** Decoupage du motif en groupes de deux hospitalisations ****************/
	printf("\n\nUne prediction partielle va etre realisee sur des groupes de deux hospitalisations.\n");
	int* motif_1 = (int*)calloc(2, sizeof(int)); // premier groupe de 2 dans le motif : si motif = 1 2 3, on obtient 1 2
	motif_1[0]=motif[0];
	motif_1[1]=motif[1];
	printf("Premier groupe de deux hospitalisations : ");
	for(int z = 0; z<2; z++) {
		printf("%d ", motif_1[z]);
	}
	printf("\n");
	int* motif_2 = (int*)calloc(2, sizeof(int));  // second groupe de 2 dans le motif : si motif = 1 2 3, on obtient 2 3
	motif_2[0]=motif[1];
	motif_2[1]=motif[2];
	printf("Deuxieme groupe de deux hospitalisations : ");
	for(int z = 0; z<2; z++) {
		printf("%d ", motif_2[z]);
	}
	printf("\n");
	int* motif_3 = (int*)calloc(2, sizeof(int)); // troisieme groupe de 2 dans le motif : si motif = 1 2 3, on obtient 1 3
	motif_3[0]=motif[0];
	motif_3[1]=motif[2];
	printf("Troisieme groupe de deux hospitalisations : ");
	for(int z = 0; z<2; z++) {
		printf("%d ", motif_3[z]);
	}
	printf("\n"); 
	int score_max = scoremax(contexte, motif_1, motif_2, motif_3); // score maximum
	int correspondance = 0; // verifie si une prediction est possible
	size_t taille_motif = 3;
	FILE* ecriture = NULL;
	char* nom_fichier = nomfichierprediction(contexte, motif, &taille_motif); // nom du fichier dans lequel on va ecrire les predictions associees au motif initial de trois hospitalisations
	ecriture = fopen(nom_fichier,"w+"); // ouverture en ecriture du fichier dans lequel on va ecrire les predictions et leur probabilite associee
	size_t taille_groupe_hospitalisations = 2;
	if(score(contexte, motif_1, &taille_groupe_hospitalisations)==score_max) { // cas ou le score du motif 1 est egal au score maximum
		printf("\nLe premier groupe d'hospitalisations est le plus recurrent dans la base.\n");
		pile* listePredictions1 = remplirListePredictions2(contexte, motif_1); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions1);
		if(!estvide(&listePredictions1)) { // cas ou liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions1)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions1); // on realise une prediction
				int amotif = motif[2]; // on recupere l'hospitalisation qui ne fait pas partie du motif 1
				float proba = calculerProba2(contexte,pred,motif_1,amotif); // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions1); // liberation de la memoire
		}
	}
	if(score(contexte, motif_2, &taille_groupe_hospitalisations)==score_max) { // cas ou le score du motif 2 est egal au score maximum
		printf("\nLe deuxieme groupe d'hospitalisations est le plus recurrent dans la base.\n");
		pile* listePredictions2 = remplirListePredictions2(contexte, motif_2); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions2);	
		if(!estvide(&listePredictions2)) { // cas ou la liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions2)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions2); // on realise une prediction
				int amotif = motif[0]; // on recupere l'hospitalisation qui ne fait pas partie du motif 2
				float proba = calculerProba2(contexte,pred,motif_2,amotif);  // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions2); // liberation de la memoire
		}
	}
	if(score(contexte, motif_3, &taille_groupe_hospitalisations)==score_max) { // cas ou le score du motif 3 est egal au score maximum
		printf("\nLe troisieme groupe d'hospitalisations est le plus recurrent dans la base.\n");
		pile* listePredictions3 = remplirListePredictions2(contexte, motif_3); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions3);
		if(!estvide(&listePredictions3)) { // cas ou la liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions3)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions3); // on realise une prediction
				int amotif = motif[1]; // on recupere l'hospitalisation qui ne fait pas partie du motif 3
				float proba = calculerProba2(contexte,pred,motif_3,amotif);  // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions3); // liberation de la memoire
		}
	}
	if(!correspondance){ // cas ou aucune correspondance n'a ete trouvee en scindant le motif en groupes de deux hospitalisations*/
		/**************** Decoupage du motif en hospitalisations prises de maniere isolee ****************/
		printf("\nUne prediction partielle va etre realisee sur chaque hospitalisation prise de maniere isolee.\n");
		printf("********* Prediction partielle : \n");
		int* hospitalisation_1 =(int*)calloc(1, sizeof(int)); // premiere hospitalisation
		hospitalisation_1[0] = motif[0];
		int* hospitalisation_2 =(int*)calloc(1, sizeof(int)); // seconde hospitalisation
		hospitalisation_2[0] = motif[1];
		int* hospitalisation_3 =(int*)calloc(1, sizeof(int)); // troisieme hospitalisation
		hospitalisation_3[0] = motif[2];
		pile* listePredictions = remplirListePredictions1(contexte,hospitalisation_1, hospitalisation_2, hospitalisation_3); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions);
		if(estvide(&listePredictions)){ // cas ou aucune hospitalisation isolee du patient associee a une prediction n'a ete trouvee dans le contexte
			printf("\nAucune occurrence d'une hospitalisation isolee du patient dans ce contexte. Aucune prediction n'a pu etre realisee.\n");
			fprintf(ecriture, "Aucune occurrence d'une hospitalisation isolee du patient dans ce contexte. Aucune prediction n'a pu etre realisee.\n");
		}
		while(!estvide(&listePredictions)){ // cas ou il reste encore des predictions possibles
			int pred = depiler(&listePredictions); // on realise une prediction
			fprintf(ecriture, "%i:%f\n", pred, calculerProba1(contexte,pred,hospitalisation_1,hospitalisation_2,hospitalisation_3)); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
		}
		fprintf(ecriture, "(prediction partielle hospitalisation par hospitalisation)\n"); // on precise qu'on a obtenu ces resultats en predisant sur chaque hospitalisation isolee du motif
	}
	else { 
		fprintf(ecriture, "(prediction partielle par groupes de deux hospitalisations)\n"); // on precise qu'on a obtenu ces resultats en predisant sur les groupes de deux hospitalisations issus du motif
	}
}

pile* remplirListePredictions(int contexte,int* hospitalisation_1, int* hospitalisation_2){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int i=0;
	int j=0;
	pile* listePredictions = NULL; // liste des predictions
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant les donnees 
	if (fichier != NULL) {
		char ligne [ 128 ]; // taille max d'une ligne
		while ((s= fgets(ligne, sizeof ligne, fichier))!= NULL) { // lecture d'une ligne 
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du bon contexte medical
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un contexte qui n'est pas encore le bon contexte medical
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de lecture d'un motif du contexte

				size_t taillecourant = 0;
				int* motiftemp=liremotif(ligne, &taillecourant);
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=-1;
				int prediction=-1; // hospitalisation predite
				for(j=0;j<taillecourant;j++){
					if(motiftemp[j]==hospitalisation_1[0] || motiftemp[j]==hospitalisation_2[0]){ 
						correspond=1; // le motif en cours correspond
						indice=j;
						break;
					}
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice+1)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins une autre hospitalisation a eu lieu a la suite du motif du patient)
					for(i=indice+1; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacune des hospitalisations ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque hospitalisation ayant eu lieu est une prediction
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return listePredictions;
}


float calculerProba(int contexte, int pred, int* hospitalisation_1, int* hospitalisation_2){
	float proba = 0.0;
	int* motif_1 = (int*)calloc(2, sizeof(int)); // motif_1 : premiere hospitalisation + prediction
	motif_1[0]=hospitalisation_1[0];
	motif_1[1]=pred;
	int* motif_2 = (int*)calloc(2, sizeof(int)); // motif_2 : deuxieme hospitalisation + prediction
	motif_2[0]=hospitalisation_2[0];
	motif_2[1]=pred;
	size_t taille_hospitalisation = 1;
	size_t taille_motif = 2;
	float score_1 = (float)score(contexte, motif_1, &taille_motif); // score du motif_1
	float score_2 = (float)score(contexte,motif_2, &taille_motif); // score du motif_2
	float score_3 = (float)score(contexte,hospitalisation_1, &taille_hospitalisation); // score de la premiere hospitalisation
	float score_4 = (float)score(contexte,hospitalisation_2, &taille_hospitalisation); // score de la deuxieme hospitalisation
	proba = (score_1+score_2)/(score_3+score_4); // probabilite de la prediction 
	printf("Probabilite : %f\n", proba);
	return proba; // probabilite de la prediction
}

void predictionpartielle_motif_2_hospitalisations(int contexte, int* motif) {
	/**************** Decoupage du motif en hospitalisations prises de maniere isolee ****************/
	printf("\nUne prediction partielle va etre realisee sur chaque hospitalisation prise de maniere isolee.\n");
	int* hospitalisation_1 =(int*)calloc(1, sizeof(int)); // premiere hospitalisation
	hospitalisation_1[0] = motif[0];
	int* hospitalisation_2 =(int*)calloc(1, sizeof(int)); // seconde hospitalisation
	size_t taille_motif = 2;
	hospitalisation_2[0] = motif[1];
	FILE* ecriture = NULL;
	char* nom_fichier = nomfichierprediction(contexte, motif, &taille_motif);
	ecriture = fopen(nom_fichier,"w+"); // ouverture en ecriture du fichier dans lequel on va ecrire les predictions et leur probabilite associee
	pile* listePredictions = remplirListePredictions(contexte,hospitalisation_1, hospitalisation_2); // on recupere la liste des predictions
	printf("********* Prediction partielle : \n");
	printf("La liste des predictions est : \n");
	visualiser(listePredictions);
	if(estvide(&listePredictions)==1){ // cas ou aucune hospitalisation isolee du patient associee a une prediction n'a ete trouvee dans le contexte
		printf("\nAucune occurrence d'une hospitalisation isolee du patient dans ce contexte. Aucune prediction n'a pu etre realisee.\n");
		fprintf(ecriture, "Aucune occurrence d'une hospitalisation isolee du patient dans ce contexte. Aucune prediction n'a pu etre realisee.\n");
	}
	while(!estvide(&listePredictions)){ // cas ou il reste encore des predictions possibles
		int pred = depiler(&listePredictions); // on realise une prediction
		fprintf(ecriture, "%i:%f\n", pred, calculerProba(contexte,pred,hospitalisation_1,hospitalisation_2)); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
	}
	fprintf(ecriture, "(prediction partielle hospitalisation par hospitalisation)\n"); // on precise qu'on a obtenu ces resultats en predisant sur chaque hospitalisation isolee du motif
}


void predictionpartielle(int contexte, int* motif, size_t* taille) {	
	if(*taille==3) { // cas ou le motif est constitue de trois hospitalisations
		predictionpartielle_motif_3_hospitalisations(contexte, motif);
	}
	else if(*taille==2) { // cas ou le motif est constitue de deux hospitalisations
		predictionpartielle_motif_2_hospitalisations(contexte, motif);
	}
}
