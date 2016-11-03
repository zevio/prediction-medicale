/**
 *\file pile.c
 *\brief Structure de pile. Permet de creer une liste de predictions
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


void empiler(pile **p, int valeur) {
	pile *element = malloc(sizeof(pile)); // creation d'un element de pile
	if(!element) {
		fprintf(stderr, "Erreur lors de l'allocation memoire de l'element de la pile.\n");
		exit(EXIT_FAILURE);
	}
	element->valeur = valeur; // valeur
	element->prec = *p; // on empile
    *p = element; // le pointeur pointe sur le dernier élément
}


int depiler(pile **p) {
    int valeur;
    pile *tmp;
    if(!*p) {
   		return -1; // retourne -1 si la pile est vide
    }
    tmp = (*p)->prec;
    valeur = (*p)->valeur; // on recupere la valeur soutiree de la pile
    free(*p); // liberation memoire
    *p = tmp; // le pointeur pointe sur le dernier élément
    return valeur; // retourne la valeur soutiree de la pile
}


void vider(pile **p) {
    pile *tmp;
    while(*p) {
        tmp = (*p)->prec;
        free(*p); // liberation memoire 
         *p = tmp;
    }
}


void visualiser(pile *p) {
    while(p) {
        printf("%d\n",p->valeur); // affichage d'un element de la pile
        p = p->prec;
    }
}


int existe(pile *p, int valeur) {
	int existe = 0;
    while(p) {
        if(p->valeur == valeur) {
            existe = 1; // cas ou la valeur existe dans la pile
        }
        p = p->prec;
    }
    return existe;
}


int estvide(pile **p){
	return (!*p); // 1 si la pile est vide
}
