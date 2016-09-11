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
#include "prediction.h" 

#include "predictionpartielle.h"


char* decoupermotif(char* motif, int debut, int fin){
	char* nmotif = NULL; // sous motif qu'on souhaite extraire
	if (motif != NULL && debut < fin) {
		nmotif = malloc (sizeof (*nmotif) * (fin - debut + 2)); // allocation memoire
		if (nmotif != NULL) {
			int i;
			for (i = debut; i <= fin; i++) {
				nmotif[i-debut] = motif[i]; // copie de la sous chaine de debut a fin
			}
			nmotif[i-debut] = '\0'; // caractere final
		}
	}
	return nmotif;
}


int scoremax(int contexte, char* motif_1, char* motif_2, char* motif_3){
	int score_max = score(contexte, motif_1); // le score maximum est le score du motif 1
	int score_2 = score(contexte, motif_2); // score du motif 2
	int score_3 = score(contexte, motif_3); // score du motif 3
	if(score_2>score_max) { // cas ou le motif 2 a un score strictement superieur au motif 1
		score_max = score_2; 
	}
	if(score_3>score_max) { // cas ou le motif 3 a un score strictement superieur au score maximum
		score_max = score_3;
	}
	return score_max; // score maximum
}


pile* remplirListePredictions2(int contexte, char* motif){
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
		int* tmotif=conversionmotif(motif); // conversion du motif passe en parametre en tableau d'entiers 
		int taillemotif = strlen(motif); // taille du motif 
		int taillepatient=((taillemotif+1)/2); // taille du tableau d'entiers correspondant au motif
		FILE* ecriture = NULL;
		ecriture = fopen("resultats/predictions.txt","w+"); // fichier dans lequel on va ecrire les predictions et leur probabilite associee
		while ((s= fgets(ligne, sizeof ligne, fichier))!= NULL) { // lecture d'une ligne 
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du bon contexte medical
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un contexte qui n'est pas encore le bon contexte medical
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de lecture d'un motif du contexte
				int* motiftemp=conversionmotif(ligne); // conversion du motif en cours en tableau d'entiers 
				/*printf("\n Motif %d : %s", cptmotif, ligne);*/ // affichage des motifs sur le terminal 
				int tailletemp = strlen(ligne); // taille du motif en cours 
				int taillecourant=((tailletemp+1)/2); // taille du tableau tmotif 
				int correspondance=0; // nombre de correspondances entre le motif courant dans le contexte et le motif du patient
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=-1;
				int prediction=-1; // evenement predit
				for(i=0;i<taillepatient;i++){
					for(j=indice;j<taillecourant;j++){
						if(tmotif[i]==motiftemp[j]){ 
							correspondance++; // calcul des correspondances entre le motif courant et le motif du patient
							indice=j;
						}
					}
				}
				if(correspondance==taillepatient){ // si le motif courant contient le motif du patient
					correspond=1; // le motif courant correspond
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice+1)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins un autre evenement a eu lieu a la suite du motif du patient)
					for(i=indice+1; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacun des evenements ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque evenement ayant eu lieu est une prediction
						printf("Prediction : %d\n",prediction);
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
		fclose(ecriture); // fermeture du fichier dans lequel on ecrit
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return listePredictions;
}


float calculerProba2(int contexte, int pred, char* motif, char* amotif){
	float proba = 0.0; // probabilite de la prediction
	char* motifp = (char*)calloc(6, sizeof(char)); // motifp : groupe de deux hospitalisations + prediction
	strcpy(motifp,motif);
	strcat(motifp,conversionint(pred));
	char* amotifp = (char*)calloc(4, sizeof(char)); // amotif : hospitalisation restante + prediction
	strcpy(amotifp,amotif);
	strcat(amotifp,conversionint(pred));
	float score_1 = (float)score(contexte, motifp); // score de motifp
	printf("score 1 : %f du motif %s\n",score_1, motifp);
	float score_2 = (float)score(contexte,amotifp); // score de amotif
	printf("score 2 : %f du motif %s\n",score_2,amotifp); 
	float score_3 = (float)score(contexte,motif); // score du groupe de deux hospitalisations
	printf("score 3 : %f du motif %s\n",score_3, motif); 
	float score_4 = (float)score(contexte,amotif); // score de l'hospitalisation restante
	printf("score 4 : %f du motif %s\n",score_4, amotif); 
	proba = (score_1+score_2)/(score_3+score_4); // probabilite de la prediction 
	printf("Proba : %f\n", proba);
	return proba; // probabilite de la prediction
}


pile* remplirListePredictions1(int contexte,char* evenement_1, char* evenement_2, char* evenement_3){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int i=0;
	int j=0;
	pile* listePredictions = NULL; // liste des predictions
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant les donnees 
	int* ev1 = conversionmotif(evenement_1); // premiere hospitalisation
	int* ev2 = conversionmotif(evenement_2); // deuxieme hospitalisation
	int* ev3 = conversionmotif(evenement_3); // troisieme hospitalisation
	if (fichier != NULL) {
		char ligne [ 128 ]; // taille max d'une ligne
		FILE* ecriture = NULL;
		ecriture = fopen("resultats/predictions.txt","w+"); // fichier dans lequel on va ecrire les predictions et leur probabilite associee
		while ((s= fgets(ligne, sizeof ligne, fichier))!= NULL) { // lecture d'une ligne 
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du bon contexte medical
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un contexte qui n'est pas encore le bon contexte medical
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de lecture d'un motif du contexte
				int* motiftemp=conversionmotif(ligne); // conversion du motif en cours en tableau d'entiers 
				/*printf("\n Motif %d : %s", cptmotif, ligne);*/ // affichage des motifs sur le terminal 
				int tailletemp = strlen(ligne); // taille du motif en cours 
				int taillecourant=((tailletemp+1)/2); // taille du tableau tmotif 
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=-1;
				int prediction=-1; // evenement predit
				printf("Motif en cours : %s\n", ligne);
				for(j=0;j<taillecourant;j++){
					if(motiftemp[j]==ev1[0] || motiftemp[j]==ev2[0] || motiftemp[j]==ev3[0]){ 
						correspond=1; // le motif en cours correspond
						indice=j;
						break;
					}
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice+1)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins un autre evenement a eu lieu a la suite du motif du patient)
					for(i=indice+1; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacun des evenements ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque evenement ayant eu lieu est une prediction
						printf("Prediction : %d\n",prediction);
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
		fclose(ecriture); // fermeture du fichier dans lequel on ecrit
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return listePredictions;
}


float calculerProba1(int contexte, int pred, char* evenement_1, char* evenement_2, char* evenement_3){
	float proba = 0.0;
	char* motif_1 = (char*)calloc(4, sizeof(char)); // motif_1 : premiere hospitalisation + prediction
	strcpy(motif_1,evenement_1);
	strcat(motif_1,conversionint(pred));
	char* motif_2 = (char*)calloc(4, sizeof(char)); // motif_2 : deuxieme hospitalisation + prediction
	strcpy(motif_2,evenement_2);
	strcat(motif_2,conversionint(pred));
	char* motif_3 = (char*)calloc(4, sizeof(char)); // motif_3 : troisieme hospitalisation + prediction
	strcpy(motif_3,evenement_3);
	strcat(motif_3,conversionint(pred));
	float score_1 = (float)score(contexte, motif_1); // score du motif_1
	printf("score 1 : %f du motif %s\n",score_1, motif_1);
	float score_2 = (float)score(contexte,motif_2); // score du motif_2
	printf("score 2 : %f du motif %s\n",score_2,motif_2);
	float score_3 = (float)score(contexte,motif_3); // score du motif_3
	printf("score 3 : %f du motif %s\n",score_3, motif_3);
	float score_4 = (float)score(contexte,evenement_1); // score de la premiere hospitalisation
	printf("score 4 : %f du motif %s\n",score_4, evenement_1);
	float score_5 = (float)score(contexte,evenement_2); // score de la deuxieme hospitalisation
	printf("score 5 : %f du motif %s\n",score_5, evenement_2);
	float score_6 = (float)score(contexte,evenement_3); // score de la troisieme hospitalisation
	printf("score 6 : %f du motif %s\n",score_6, evenement_3);
	proba = (score_1+score_2+score_3)/(score_4+score_5+score_6); // probabilite de la prediction 
	printf("Proba : %f\n", proba);
	return proba; // probabilite de la prediction
}


void predictionpartielle(int contexte, char* motif) {	
	/**************** Decoupage du motif en groupes de deux hospitalisations ****************/
	char* motif_1 = (char*)calloc(5, sizeof(char)); // premier groupe de 2 dans le motif : si motif = 1 2 3, on obtient 1 2
	strncpy(motif_1, motif, 3);
	motif_1[3] = '\0';
	printf("Motif 1 : %s\n", motif_1);
	char* motif_2 = decoupermotif(motif, 2, 5); // second groupe de 2 dans le motif : si motif = 1 2 3, on obtient 2 3
	printf("Motif 2 : %s\n", motif_2);
	char* motif_3 = (char*)calloc(5, sizeof(char)); // troisieme groupe de 2 dans le motif : si motif = 1 2 3, on obtient 1 3
	char* motif_3_1 = decoupermotif(motif, 0, 1); 
	char* motif_3_2 = decoupermotif(motif, 4, 5); 
	strcpy(motif_3, motif_3_1);
	strcat(motif_3, motif_3_2);
	printf("Motif 3 : %s\n", motif_3);
	int score_max = scoremax(contexte, motif_1, motif_2, motif_3); // score maximum
	int correspondance = 0; // verifie si une prediction est possible
	FILE* ecriture = NULL;
	ecriture = fopen("resultats/predictions.txt","w+"); // fichier dans lequel on va ecrire les preditions et leur probabilite associee 
	if(score(contexte,motif_1)==score_max) { // cas ou le score du motif 1 est egal au score maximum
		pile* listePredictions1 = remplirListePredictions2(contexte, motif_1); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions1);
		if(!estvide(&listePredictions1)) { // cas ou liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions1)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions1); // on realise une prediction
				char* amotif = decoupermotif(motif, 4, 5); // on recupere l'hospitalisation qui ne fait pas partie du motif 1
				float proba = calculerProba2(contexte,pred,motif_1,amotif); // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions1); // liberation de la memoire
		}
	}
	if(score(contexte,motif_2)==score_max) { // cas ou le score du motif 2 est egal au score maximum
		pile* listePredictions2 = remplirListePredictions2(contexte, motif_2); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions2);	
		if(!estvide(&listePredictions2)) { // cas ou la liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions2)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions2); // on realise une prediction
				char* amotif = decoupermotif(motif, 0, 1); // on recupere l'hospitalisation qui ne fait pas partie du motif 2
				float proba = calculerProba2(contexte,pred,motif_2,amotif);  // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions2); // liberation de la memoire
		}
	}
	if(score(contexte,motif_3)==score_max) { // cas ou le score du motif 3 est egal au score maximum
		pile* listePredictions3 = remplirListePredictions2(contexte, motif_3); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions3);
		if(!estvide(&listePredictions3)) { // cas ou la liste des predictions n'est pas vide
			correspondance=1; // une prediction est possible
			while(!estvide(&listePredictions3)) { // cas ou il reste encore des predictions possibles
				int pred = depiler(&listePredictions3); // on realise une prediction
				char* amotif = decoupermotif(motif, 2, 3); // on recupere l'hospitalisation qui ne fait pas partie du motif 3
				float proba = calculerProba2(contexte,pred,motif_3,amotif);  // probabilite de la prediction 
				fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
			}
			vider(&listePredictions3); // liberation de la memoire
		}
	}
	/**************** Decoupage du motif en hospitalisations prises de maniere isolee ****************/
	if(!correspondance){ // cas ou aucune correspondance n'a ete trouvee en scindant le motif en groupes de deux hospitalisations
		printf("Aucune correspondance n'a ete trouvee. Des predictions vont etre realisees sur chaque hospitalisation prise de maniere isolee.\n");
		char* evenement_1 =(char*)calloc(2, sizeof(char)); // premiere hospitalisation
		evenement_1 = decoupermotif(motif, 0, 1);
		evenement_1[2] = '\0';
		char* evenement_2 =(char*)calloc(2, sizeof(char)); // seconde hospitalisation
		evenement_2 = decoupermotif(motif, 2, 3);
		evenement_2[2] = '\0';
		char* evenement_3 =(char*)calloc(2, sizeof(char)); // troisieme hospitalisation
		evenement_3 = decoupermotif(motif, 4, 5);
		evenement_3[2] = '\0';
		printf("Les hospitalisations sont : %s, %s, %s\n", evenement_1,evenement_2,evenement_3);
		pile* listePredictions = remplirListePredictions1(contexte,evenement_1, evenement_2, evenement_3); // on recupere la liste des predictions
		printf("La liste des predictions est : \n");
		visualiser(listePredictions);
		while(!estvide(&listePredictions)){ // cas ou il reste encore des predictions possibles
			int pred = depiler(&listePredictions); // on realise une prediction
			fprintf(ecriture, "%i:%f\n", pred, calculerProba1(contexte,pred,evenement_1,evenement_2,evenement_3)); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
		}
	}
}