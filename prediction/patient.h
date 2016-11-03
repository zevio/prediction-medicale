/**
 *\file patient.h
 *\brief Antecedents du patient (motif)
 *\author Stella Zevio
 *\version 1.0
 *\date 2015-06-20
 *
 * Prediction medicale
 *
 */

#ifndef PATIENT_H_
#define PATIENT_H_

/**
 * \fn char *creermotif(int argc, char *argv[])
 * \brief Fonction permettant de recuperer le motif d'un patient passe en parametre sur le terminal (ses antecedents medicaux)
 * \param argc - le nombre de parametres passes au programme
 * \param argv - les parametres passes au programme
 * \return motif - le motif du patient
 */
char* creermotif(int argc, char* argv[]);

/**
 * \fn int* liremotif(char *motif, size_t* taille)
 * \brief Fonction permettant de construire le tableau d'hospitalisations correspondant au motif du patient
 * \param motif - le motif du patient sous forme de chaine de caracteres (chaque hospitalisation dans le motif est separee de la suivante par un espace)
 * \param taille - la taille du motif du patient 
 * \return tab_motif - le motif du patient sous forme de tableau d'entiers (chaque entier correspondant a une hospitalisation du motif)
 */
int* liremotif(char *motif, size_t* taille);

/**
 * \fn void limitermotif(int* tab_motif, size_t* taille)
 * \brief Procedure permettant de limiter le motif d'un patient a ses trois dernieres hospitalisations, afin de proposer une prediction pertinente
 * \param tab_motif - le motif complet du patient sous forme de tableau d'entiers (chaque entier correspond a une hospitalisation)
 * \param taille - la taille du motif du patient (complet)
 */
void limitermotif(int* tab_motif, size_t* taille);

#endif
