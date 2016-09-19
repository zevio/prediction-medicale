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
 * \brief Fonction permettant de recuperer le motif d'un patient passe en parametre sur le terminal (ses antecedents)
 * \param argc - le nombre de parametres passes au programme
 * \param argv - les parametres passes au programme
 * \return motif - le motif du patient
 */
char* creermotif(int argc, char* argv[]);

/**
 * \fn char* limitermotif(char* motif)
 * \brief Fonction permettant de limiter le motif d'un patient a ses trois dernieres hospitalisations, afin de proposer une prediction pertinente.
 * \param motif - le motif complet du patient
 * \return motifpred - le motif du patient utilise pour la prediction (les 3 dernieres hospitalisations)
 */
char* limitermotif(char* motif);

#endif
