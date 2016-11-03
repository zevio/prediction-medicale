/**
 *\file prediction.h
 *\brief Prediction des suites d'un patient a partir de l'analyse des motifs de patients contenus dans une base sequentielle
 *\author Stella Zevio
 *\version 1.0
 *\date 2015-06-20
 *
 * Prediction medicale
 *
 */

#ifndef PREDICTION_H_
#define PREDICTION_H_

/**
 * \fn char* conversionint(int i)
 * \brief Fonction de conversion d'un entier en chaine de caractere. Utile pour traiter les motifs de la base sequentielle
 * \param i - l'entier a convertir
 * \return s - la chaine de caractere equivalente a l'entier
 */
char* conversionint(int i);

/**
 * \fn char* conversiontmotif(int* tmotif, int taillemotif)
 * \brief Fonction de conversion d'un tableau d'entiers (tableau d'hospitalisations) en chaine de caracteres (motif). Utile pour traiter les motifs de la base sequentielle
 * \param tmotif - le tableau d'entiers representant un motif
 * \param taillemotif - la taille du tableau d'entiers
 * \return motif - le motif sous forme de chaine de caracteres
 */
char* conversiontmotif(int* tmotif, int taillemotif);

/**
 * \fn char* nomfichierprediction(int contexte, int* motif, size_t* taille)
 * \brief Fonction permettant d'ecrire le nom du fichier qui contiendra les predictions associees a un motif dans un contexte medical precis
 * \param contexte - le contexte medical dans lequel on se place
 * \param motif - le motif du patient
 * \param taille - la taille du motif du patient
 * \return nom_fichier - le nom du fichier qui contiendra les predictions associees au motif dans le contexte medical
 */
char* nomfichierprediction(int contexte, int* motif, size_t* taille);

/**
 * \fn int score(int contexte, int* motif, size_t* taille)
 * \brief Fonction de calcul du score d'un motif dans un contexte. Le score correspond au nombre de motifs dans la base sequentielle qui correspondent avec le motif dont on veut calculer le score
 * \param contexte - le contexte medical considere
 * \param motif - le motif dont on calcule le score
 * \param taille - la taille du motif dont on calcule le score
 * \return score - le score du motif dans le contexte
 */
int score(int contexte, int* motif, size_t* taille);

/**
 * \fn int prediction(int contexte, int* motif, size_t* taille)
 * \brief Fonction de prediction pour un motif dans un contexte. Les predictions sont stockees dans un fichier prediction.txt
 * \param contexte - le contexte medical considere
 * \param motif - le motif sur lequel on veut realiser la prediction
 * \param taille - la taille du motif sur lequel on veut realiser la prediction
 * \return prediction_possible - 1 si au moins une prediction est possible sur le motif, 0 sinon
 */
int prediction(int contexte, int* motif, size_t* taille);

#endif
