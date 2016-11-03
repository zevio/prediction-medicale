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
#include "prediction.h"

/**
 * \fn scoremax(int contexte, char* motif_1, char* motif_2, char* motif_3)
 * \brief Fonction permettant de connaitre le score maximum de trois motifs. Utile lorsqu'on souhaite decouper un motif de trois hospitalisations en groupes de 2 
 * \param contexte - le contexte medical considere
 * \param motif_1 - un motif
 * \param motif_2 - un motif
 * \param motif_3 - un motif
 * \return score_max - le score maximum des trois motifs
 */
int scoremax(int contexte, int* motif_1, int* motif_2, int* motif_3);

/****************************** Le motif du patient est compose de trois hospitalisations ******************************/

/**
 * \fn remplirListePredictions2(int contexte, char* motif)
 * \brief Fonction permettant de remplir la liste des predictions dans un contexte medical donne pour un motif de deux hospitalisations (extrait d'un motif initial de trois hospitalisations)
 * \param contexte - le contexte medical considere
 * \param motif - le motif de deux hospitalisations (extrait d'un motif initial de trois hospitalisations)
 * \return listePredictions - la liste des predictions
 */
pile* remplirListePredictions2(int contexte, int* motif);

/**
 * \fn calculerProba2(int contexte, int pred, char* motif, char* amotif)
 * \brief Fonction permettant de calculer la probabilite d'une prediction pour un motif de deux hospitalisations (extrait d'un motif initial de trois hospitalisations)
 * \param contexte - le contexte medical considere
 * \param pred - la prediction
 * \param motif - le motif de deux hospitalisations (extrait d'un motif initial de trois hospitalisations)
 * \param amotif - l'hospitalisation restante du patient (dans le motif initial de trois hospitalisations)
 * \return proba - la probabilite de la prediction
 */
float calculerProba2(int contexte, int pred, int* motif, int amotif);

/**
 * \fn remplirListePredictions1(int contexte, char* hospitalisation_1, char* hospitalisation_2, char* hospitalisation_3)
 * \brief Fonction permettant de remplir la liste des predictions pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param hospitalisation_1 - une hospitalisation
 * \param hospitalisation_2 - une hospitalisation
 * \param hospitalisation_3 - une hospitalisation
 * \return listePredictions - la liste des predictions
 */
pile* remplirListePredictions1(int contexte, int* hospitalisation_1, int* hospitalisation_2, int* hospitalisation_3);

/**
 * \fn calculerProba1(int contexte, int pred, char* hospitalisation_1, char* hospitalisation_2, char* hospitalisation_3)
 * \brief Fonction permettant de calculer la probabilite d'une prediction pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param pred - la prediction
 * \param hospitalisation_1 - une hospitalisation
 * \param hospitalisation_2 - une hospitalisation
 * \param hospitalisation_3 - une hospitalisation
 * \return proba - la probabilite de la prediction
 */
float calculerProba1(int contexte, int pred, int* hospitalisation_1, int* hospitalisation_2, int* hospitalisation_3);

/**
 * \fn predictionpartielle_motif_3_hospitalisations(int contexte, int* motif)
 * \brief Procedure permettant de realiser une prediction partielle dans un contexte medical donne pour un motif compose de trois hospitalisations
 * \param contexte - le contexte medical considere
 * \param motif - le motif du patient compose de trois hospitalisations
 */
void predictionpartielle_motif_3_hospitalisations(int contexte, int* motif);

/****************************** Le motif du patient est compose de deux hospitalisations ******************************/

/**
 * \fn remplirListePredictions(int contexte, char* hospitalisation_1, char* hospitalisation_2)
 * \brief Fonction permettant de remplir la liste des predictions pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param hospitalisation_1 - une hospitalisation
 * \param hospitalisation_2 - une hospitalisation
 * \return listePredictions - la liste des predictions
 */
pile* remplirListePredictions(int contexte, int* hospitalisation_1, int* hospitalisation_2);

/**
 * \fn calculerProba(int contexte, int pred, int* hospitalisation_1, int* hospitalisation_2)
 * \brief Fonction permettant de calculer la probabilite d'une prediction pour chaque hospitalisation prise de maniere isolee
 * \param contexte - le contexte medical considere
 * \param pred - la prediction
 * \param hospitalisation_1 - une hospitalisation
 * \param hospitalisation_2 - une hospitalisation
 * \return proba - la probabilite de la prediction
 */
float calculerProba(int contexte, int pred, int* hospitalisation_1, int* hospitalisation_2);

/**
 * \fn predictionpartielle_motif_2_hospitalisations(int contexte, int* motif)
 * \brief Procedure permettant de realiser une prediction partielle dans un contexte medical donne pour un motif compose de deux hospitalisations
 * \param contexte - le contexte medical considere
 * \param motif - le motif du patient compose de deux hospitalisations
 */
void predictionpartielle_motif_2_hospitalisations(int contexte, int* motif);

/****************************** Prediction partielle ******************************/

/**
 * \fn predictionpartielle(int contexte, char* motif)
 * \brief Procedure permettant de realiser des predictions partielles sur un motif
 * \param contexte - le contexte medical considere
 * \param motif - le motif du patient
 */
void predictionpartielle(int contexte, int* motif, size_t* taille);

#endif
