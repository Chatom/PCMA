/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: TableSymbole.h
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 12/04/13
 *
 * @brief: Structure de la table des symboles
 *
**/

#if ! defined __TABLESYMBOLE_H__
#define __TABLESYMBOLE_H__

typedef struct SSymbole
{
	char * Nom;
	int Type;
	int Debut, Fin;
	int Adresse;
	
	int NbParam;
	
	struct SSymbole * Suivant;
	//struct SSymbole * Param;

} SSymbole;

typedef struct SPile
{
	struct SPile * Suivant;
	SSymbole * TableSymbole;

} SPile;

#endif /* __TABLESYMBOLE_H__ */

