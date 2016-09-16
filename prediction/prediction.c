/**
 *\file prediction.c
 *\brief Prediction
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


char* conversionint(int i){
	char * s=(char*)calloc(4, sizeof(char)); // la chaine de caracteres qu'on veut obtenir a partir de l'entier 'i'
    if(s==NULL) {
    	return NULL;
    }
    sprintf(s, "%d", i); // conversion de l'entier 'i' en chaine de caracteres
    printf("%d => \"%s\"\n", i, s);
    return s;
}


int* conversionmotif(char* motif) {
	int i=0;
	int taillemotif = strlen(motif); // la taille du motif qu'on veut convertir
	int* T=malloc(((taillemotif+1)/2)*sizeof(int)); // le tableau d'entiers correspondant au motif
	int tailletableau=((taillemotif+1)/2); // la taille du tableau d'entiers
	for(i=0;i<tailletableau;i++) {
		T[i]=conversionchar(motif[i*2]); // conversion du motif en tableau d'entiers qu'il represente
	}
	return T;
}	


char* conversiontmotif(int* tmotif, int taillemotif){
	char* motif=(char*)calloc(taillemotif*4, sizeof(char)); // motif qu'on veut obtenir
	if(motif==NULL) {
		return NULL;
	}
	for(int i=0; i< taillemotif; i++) { // parcours du tableau d'entiers representant le motif
		strcat(motif, conversionint(tmotif[i])); // conversion du tableau d'entiers au motif qu'il represente
		strcat(motif, " "); // ajout d'un espace
		strcat(motif, "\0"); // ajout du caractere final
	}
	return motif;
}


int score(int contexte, char* motif){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour les motifs correspondant au contexte
	int i=0; 
	int j=0;  
	int score=0; // score du motif dans le contexte
	FILE* fichier = NULL;
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant la base sequentielle
	if (fichier != NULL) {
		printf("***** Calcul du score du motif %s *****\n", motif);
		char ligne[128]; // taille max d'une ligne
		int* tmotif=conversionmotif(motif); // conversion du motif en entree en tableau d'entiers
		int taillem = strlen(motif); // taille du motif
		int taillemotif=((taillem+1)/2); // taille du tableau tmotif
		while((s=fgets(ligne,sizeof ligne,fichier)) != NULL ) { // lecture d'une ligne
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du contexte
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un nouveau contexte
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de motifs du contexte
				int* motiftemp=conversionmotif(ligne); // conversion du motif en cours en tableau d'entiers 
				int tailletemp = strlen(ligne); // taille du motif en cours 
				int taillecourant=((tailletemp+1)/2); // taille du tableau tmotif 
				int correspond=0; // le motif courant ne contient pas le motif dont on calcule le score
				int correspondance=0; // nombre de correspondances entre le motif courant dans le contexte et le motif dont on calcule le score
				int indice=0;
				printf("Motif en cours : %s", ligne);
				for(i=0;i<taillemotif;i++) {
					for(j=indice;j<taillecourant;j++) {
						if(tmotif[i]==motiftemp[j]) {
							indice=j+1;
							correspondance++; // calcul des correspondances entre le motif courant et le motif dont on calcule le score
							break;
						}
					}
				}
				printf("Le nombre de correspondances est %i", correspondance);
				if(correspondance==taillemotif){ // si le motif courant contient bien le motif dont on veut calculer le score
					correspond=1; // le motif courant correspond
					printf("Le motif correspond");
					score++; // le score augmente
				}
				printf("\n");
			}
		}
	}
	else { // cas ou la base sequentielle n'a pas pu etre lue
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	}
	return score;
}


int prediction(int contexte, char* motif) {
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int i=0;
	int j=0;
	int motifscorrespondants=0; // nombre de motifs de la base qui contiennent l'integralite du motif du patient
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant les donnees 
	if (fichier != NULL) {
		char ligne [ 128 ]; // taille max d'une ligne
		int* tmotif=conversionmotif(motif); // conversion du motif passe en parametre en tableau d'entiers 
		int taillemotif = strlen(motif); // taille du motif 
		int taillepatient=((taillemotif+1)/2); // taille du tableau d'entiers correspondant au motif
		int scorep=score(contexte,motif); // calcul du score du motif du patient
		printf("Score du motif du patient : %d\n",scorep);
		pile *listePredictions = NULL; // contient la liste des evenements predits (tous les evenements dans le motif courant de la base sequentielle qui se produisent apres la premiere occurrence du motif du patient)
		FILE* ecriture = NULL;
		ecriture = fopen("resultats/predictions.txt","w+"); // fichier dans lequel on va ecrire les preditions et leur probabilite associee
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
							indice=j+1;
							break;
						}
					}
				}
				if(correspondance==taillepatient){ // si le motif courant contient le motif du patient
					correspond=1; // le motif courant correspond
					motifscorrespondants++; // le nombre de motifs correspondant au motif du patient dans la base sequentielle augmente
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins un autre evenement a eu lieu a la suite du motif du patient)
					for(i=indice; i<taillecourant;i++){
						prediction=motiftemp[i]; // on recupere chacun des evenements ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque evenement ayant eu lieu est une prediction
						printf("Prediction : %d\n",prediction);
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions, prediction); // on l'ajoute
						}
					}
				}
			}
		}
		while(!estvide(&listePredictions)){ // tant qu'il existe encore des predictions possibles
			int pred = depiler(&listePredictions); // on en traite une
			char* motifcourant = (char*)calloc(strlen(motif)+2, sizeof(char)); // motif courant : motif du patient + prediction
			strcpy(motifcourant, motif);
			strcat(motifcourant, conversionint(pred));
			int scorec = score(contexte, motifcourant); // le score du motif courant
			float proba = ((float)scorec)/((float)scorep); // probabilite de la prediction = score_du_motif_courant/score_du_motif_patient
			fprintf(ecriture, "%i:%f\n", pred, proba); // on enregistre la prediction et sa probabilite dans le fichier predictions.txt 
		}
		vider(&listePredictions); // liberation de la memoire
		fclose(ecriture); // fermeture du fichier dans lequel on ecrit
	}
	else {
		fprintf(stderr, "La base sequentielle est introuvable. Veuillez verifier que base.txt existe dans le dossier base.\n");
		exit(1);
	} 
	fclose(fichier); // fermeture de la base sequentielle
	return motifscorrespondants; // le nombre d'occurrences du motif du patient dans la base sequentielle
}