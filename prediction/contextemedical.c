/**
 *\file contextemedical.c
 *\brief Contexte medical dans lequel on souhaite realiser la prediction
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

#include "contextemedical.h"


int lirecontexte(char a){
	char temp[2];
	int aint; // entier correspondant au caractere 'a'
	sprintf(temp,"%c",a); // formate le caractere 'a' en chaine de caracteres
	aint = atoi(temp); // conversion chaine de caractere -> entier
	return aint;
}
