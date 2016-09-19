/**
 *\file predictionpartielle.h
 *\brief Prediction partielle sur un motif de patient (cas ou il n'existe pas de correspondance exacte entre le motif du patient et la base sequentielle)
 *\author Stella Zevio
 *\version 1.0
 *\date 2015-06-20
 *
 * Prediction medicale
 *
 */

#ifndef PREDICTIONPARTIELLE_H_
#define PREDICTIONPARTIELLE_H_

#include "pile.h"

/**
 * \fn decoupermotif(char* motif, int debut, int fin)
 * \brief Fonction permettant de scinder un motif
 * \param motif - l'entier a convertir
 * \param debut - position de debut d'extraction de la sous chaine dans le motif
 * \param fin - position de fin d'extraction de la sous chaine dans le motif
 * \return nmotif - motif de la position debut a la position fin
 */
char* decoupermotif(char* motif, int debut, int fin);

/**
 * \fn scoremax(int contexte, char* motif_1, char* motif_2, char* motif_3)
 * \brief Fonction permettant de connaitre le score maximum de trois motifs. Utile lorsqu'on souhaite decouper un motif de trois hospitalisations en groupes de 2 
 * \param contexte - le contexte medical considere
 * \param motif_1 - un motif
 * \param motif_2 - un motif
 * \param motif_3 - un motif
 * \return score_max - le score maximum 
 */
int scoremax(int contexte, char* motif_1, char* motif_2, char* motif_3);

/**
 * \fn remplirListePredictions2(int contexte, char* motif)
 * \brief Fonction permettant de remplir la liste des predictions pour un motif scinde en groupes de deux 
 * \param contexte - le contexte medical considere
 * \param motif - le motif
 * \return listePredictions - la liste des predictions
 */
pile* remplirListePredictions2(int contexte, char* motif);

/**
 * \fn calculerProba2(int contexte, int pred, char* motif, char* amotif)
 * \brief Fonction permettant de calculer la probabilite d'une prediction pour un motif de deux hospitalisations
 * \param contexte - le contexte medical considere
 * \param pred - la prediction
 * \param motif - le motif de deux hospitalisations
 * \param amotif - l'hospitalisation restante du patient
 * \return proba - la probabilite de la prediction
 */
float calculerProba2(int contexte, int pred, char* motif, char* amotif);

/**
 * \fn remplirListePredictions1(int contexte,char* evenement_1, char* evenement_2, char* evenement_3)
 * \brief Fonction permettant de remplir la liste des predictions pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param evenement_1 - une hospitalisation
 * \param evenement_2 - une hospitalisation
 * \param evenement_3 - une hospitalisation
 * \return listePredictions - la liste des predictions
 */
pile* remplirListePredictions1(int contexte,char* evenement_1, char* evenement_2, char* evenement_3);

/**
 * \fn calculerProba1(int contexte, int pred, char* evenement_1, char* evenement_2, char* evenement_3)
 * \brief Fonction permettant de calculer la probabilite d'une prediction pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param pred - la prediction
 * \param evenement_1 - une hospitalisation
 * \param evenement_2 - une hospitalisation
 * \param evenement_3 - une hospitalisation
 * \return proba - la probabilite de la prediction
 */
float calculerProba1(int contexte, int pred, char* evenement_1, char* evenement_2, char* evenement_3);

/**
 * \fn predictionpartielle(int contexte, char* motif)
 * \brief Fonction permettant de realiser des predictions partielles sur un motif
 * \param contexte - le contexte medical considere
 * \param motif - le motif du patient
 */
void predictionpartielle(int contexte, char* motif);

#endif
