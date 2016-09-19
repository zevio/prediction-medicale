/**
 *\file pile.h
 *\brief Structure de pile. Permet de creer une liste de predictions
 *\author Stella Zevio
 *\version 1.0
 *\date 2015-06-20
 *
 * Prediction medicale
 *
 */

#ifndef PILE_H_
#define PILE_H_

/**
 * \struct Pile
 * \brief Pile contenant les predictions
 */
typedef struct pile {
	int valeur;
	struct pile *prec;
} pile;

/**
 * \fn empiler(pile **p, int val)
 * \brief Fonction permettant d'empiler une valeur
 * \param p - la pile
 * \param valeur - la valeur a empiler
 */
void empiler(pile **p, int valeur);

/**
 * \fn depiler(pile **p)
 * \brief Fonction permettant de depiler une valeur
 * \param p - la pile
 * \return valeur - la valeur qu'on vient de depiler
 */
int depiler(pile **p);

/**
 * \fn vider(pile **p)
 * \brief Fonction permettant de vider la pile
 * \param p - la pile
 */
void vider(pile **p);

/**
 * \fn visualiser(pile *p)
 * \brief Fonction permettant de visualiser la pile
 * \param p - la pile
 */
void visualiser(pile *p);

/**
 * \fn existe(pile *p, int valeur)
 * \brief Fonction permettant de verifier si une valeur existe dans la pile
 * \param p - la pile
 * \param valeur - la valeur dont on veut verifier la presence dans la pile
 * \return existe - 0 si la valeur n'existe pas dans la pile, 1 sinon
 */
int existe(pile *p, int valeur);

/**
 * \fn estvide(pile **p)
 * \brief Fonction permettant de verifier si une pile est vide
 * \param p - la pile
 * \return 1 si la pile est vide
 * \return 0 si la pile n'est pas vide
 */
int estvide(pile **p);

#endif
