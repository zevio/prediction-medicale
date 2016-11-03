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
#include "patient.h"


char* conversionint(int i){
	char* s=(char*)calloc(4, sizeof(char)); // la chaine de caracteres qu'on veut obtenir a partir de l'entier 'i'
    if(s==NULL) {
    	fprintf(stderr, "Erreur lors de l'allocation memoire de la chaine de caracteres representant l'entier %d\n",i);
    	exit(EXIT_FAILURE);
    }
    sprintf(s, "%d", i); // conversion de l'entier 'i' en chaine de caracteres
    return s;
}

char* conversiontmotif(int* tmotif, int taillemotif){
	char* motif=(char*)calloc(taillemotif*4, sizeof(char)); // motif qu'on veut obtenir sous forme de chaine de caracteres
	if(motif==NULL) {
    	fprintf(stderr, "Erreur lors de l'allocation memoire de la chaine de caracteres correspondant au tableau d'hospitalisations\n");
    	exit(EXIT_FAILURE);
	}
	for(int i=0; i<taillemotif; i++) { // parcours du tableau d'entiers representant le motif
		strcat(motif, conversionint(tmotif[i])); // conversion du tableau d'entiers au motif qu'il represente
		strcat(motif, " "); // ajout d'un espace
		strcat(motif, "\0"); // ajout du caractere final
	}
	return motif;
}

char* nomfichierprediction(int contexte, int* motif, size_t* taille){
	char* debut_nom_fichier = "resultats/predictions_"; // debut du nom du fichier (arborescence)
	char* ctxt = conversionint(contexte); // contexte
	char* u = "_";
	char* extension = ".txt"; // extension txt
	char* nom_fichier = (char*)calloc(strlen(debut_nom_fichier)+1+(*taille*2)+strlen(extension)+strlen(ctxt), sizeof(char)); // allocation memoire du nom du fichier
	strcpy(nom_fichier,debut_nom_fichier); // ajout du debut du nom du fichier
	strcat(nom_fichier,ctxt); // copie du contexte dans le nom du fichier
	strcat(nom_fichier,u); // ajout du caractere '_'
	char * motif_chaine = conversiontmotif(motif, (int)*taille); // motif sous forme de chaine de caracteres
	strcat(nom_fichier,motif_chaine); // copie du motif dans le nom du fichier
	strcat(nom_fichier,extension); // ajout de l'extension dans le nom du fichier
	return nom_fichier;
}


int score(int contexte, int* motif, size_t* taille){
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour les motifs correspondant au contexte
	int i=0; 
	int j=0;  
	int score=0; // score du motif dans le contexte
	FILE* fichier = NULL;
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant la base sequentielle
	if (fichier != NULL) {
		char ligne[128]; // taille max d'une ligne
		while((s=fgets(ligne,sizeof ligne,fichier)) != NULL ) { // lecture d'une ligne
			if(s[0]=='c' && cptmotif!=0){ // cas ou on a fini de lire les motifs du contexte
				break;
			}
			else if(s[0]=='c'){ // cas ou on entre dans un nouveau contexte
				cptcontexte+=1; // on augmente le compteur de contextes
			}
			else if(cptcontexte==contexte){ // cas ou on arrive au bon contexte dans la base sequentielle
				cptmotif+=1; // augmentation du compteur de motifs du contexte
				size_t taillecourant = 0;
				int* motiftemp = liremotif(ligne, &taillecourant); // conversion du motif en cours en tableau d'entiers 
				int correspond=0; // le motif courant ne contient pas le motif dont on calcule le score
				int correspondance=0; // nombre de correspondances entre le motif courant dans le contexte et le motif dont on calcule le score
				int indice=0;
				printf("Motif en cours : %s ", ligne);
				for(i=0;i<*taille;i++) {
					for(j=indice;j<taillecourant;j++) {
						if(motif[i]==motiftemp[j]) {
							indice=j+1;
							correspondance++; // calcul des correspondances entre le motif courant et le motif dont on calcule le score
							break;
						}
					}
				}
				printf("\t nombre de correspondances : %i ", correspondance);
				if(correspondance==*taille){ // si le motif courant contient bien le motif dont on veut calculer le score
					correspond=1; // le motif courant correspond
					printf(" (le motif correspond)");
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


int prediction(int contexte, int* motif, size_t* taille) {
	char* s; // la ligne lue 
	int cptcontexte=-1; // un compteur pour le contexte
	int cptmotif=0; // un compteur pour le nombre de motifs correspondant au contexte
	int prediction_possible=0;
	int i=0;
	int j=0;
	int motif_correspondants = 0;
	FILE* fichier = NULL; 
	fichier = fopen("base/base.txt", "r+"); // ouverture du fichier contenant les donnees 
	if (fichier != NULL) {
		char ligne[128]; // taille max d'une ligne
		int scorep=score(contexte,motif,taille); // calcul du score du motif du patient
		pile *listePredictions = NULL; // contient la liste des evenements predits (tous les evenements dans le motif courant de la base sequentielle qui se produisent apres la premiere occurrence du motif du patient)
		FILE* ecriture = NULL; // fichier dans lequel on va ecrire les predictions et leur probabilite associee
		char* nom_fichier = nomfichierprediction(contexte, motif, taille);
		ecriture = fopen(nom_fichier,"w+"); // ouverture en ecriture du fichier dans lequel on va ecrire les predictions et leur probabilite associee
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
				int* motifcourant=liremotif(ligne, &taillecourant); // conversion du motif en cours en tableau d'entiers 
				int correspondance=0; // nombre de correspondances entre le motif courant dans le contexte et le motif du patient
				int correspond=0; // le motif courant ne contient pas le motif du patient
				int indice=0;
				int prediction=-1; // evenement predit
				for(i=0;i<*taille;i++){
					for(j=indice;j<taillecourant;j++){
						if(motif[i]==motifcourant[j]){ 
							correspondance++; // calcul des correspondances entre le motif courant et le motif du patient
							indice=j+1;
							break;
						}
					}
				}
				if(correspondance==*taille){ // si le motif courant contient le motif du patient
					correspond=1; // le motif courant correspond
					motif_correspondants++;
				}
				if((correspond==1) && (indice!=-1) && (taillecourant>indice)){ // si le motif courant contient le motif du patient et qu'une prediction est possible (au moins un autre evenement a eu lieu a la suite du motif du patient)
					printf("\nPrediction sur le motif %s ", ligne);
					for(i=indice; i<taillecourant;i++){
						prediction=motifcourant[i]; // on recupere chacun des evenements ayant eu lieu a la suite de la correspondance avec le motif du patient dans le motif courant : chaque evenement ayant eu lieu est une prediction
						printf("\t %d\n",prediction);
						if(!existe(listePredictions,prediction)) { // si la prediction n'est pas deja dans la liste des predictions possibles
							empiler(&listePredictions,prediction); // on l'ajoute
						}
					}
				}
			}
		}
		if(!estvide(&listePredictions)){ // si on a trouve au moins une prediction
			prediction_possible=1; // une prediction au moins est possible : il existait une correspondance du motif du patient dans la base, et cette correspondance etait suivie d'au moins une hospitalisation qu'on pourra predire
		}
		while(!estvide(&listePredictions)){ // tant qu'il existe encore des predictions possibles
			printf("********* Predictions : \n");
			int pred = depiler(&listePredictions); // on en traite une
			size_t taillecourant = *taille+1;
			int* motifcourant = (int*)calloc(taillecourant, sizeof(int)); // motif courant : motif du patient + prediction
			int i = 0;
			for(i=0;i<*taille;i++){
				motifcourant[i]=motif[i];
			}
			motifcourant[i]=pred;
			
			int scorec = score(contexte, motifcourant, &taillecourant); // le score du motif courant
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
	return prediction_possible; // 1 si au moins une prediction est possible, 0 sinon
}
