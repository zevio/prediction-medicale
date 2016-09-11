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


void empiler(pile **p, int val) {
	pile *element = malloc(sizeof(pile));
	if(!element) {
		fprintf(stderr, "Erreur lors de l'allocation memoire de la pile.\n");
		exit(2);
	}
	element->valeur = val;
	element->prec = *p;
    *p = element;       /* Le pointeur pointe sur le dernier élément. */
}


int depiler(pile **p) {
    int val;
    pile *tmp;
    if(!*p) {
   		return -1;     /* Retourne -1 si la pile est vide. */
    }
    tmp = (*p)->prec;
    val = (*p)->valeur;
    free(*p);
    *p = tmp;       /* Le pointeur pointe sur le dernier élément. */
    return val;     /* Retourne la valeur soutirée de la pile. */
}


void vider(pile **p) {
    pile *tmp;
    while(*p) {
        tmp = (*p)->prec;
        free(*p);
         *p = tmp;
    }
}


void visualiser(pile *p) {
    while(p) {
        printf("%d\n",p->valeur);
        p = p->prec;
    }
}


int existe(pile *p, int val) {
	int existe = 0;
    while(p) {
        printf("%d\n",p->valeur);
        if(p->valeur == val) {
            existe = 1;
        }
        p = p->prec;
    }
    return existe;
}


int estvide(pile **p){
	return (!*p);
}