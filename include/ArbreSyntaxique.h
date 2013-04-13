/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: ArbreSyntaxique.h
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Structure de l'arbre syntaxique
 *
**/

union MyUnion {
	struct SNoeud * Fils;
	int Nombre;
	char * Nom;
	struct SNoeud * Frere;
};

typedef struct SNoeud {
	int Type;
	union MyUnion Fils1;
	union MyUnion Fils2;
	union MyUnion Fils3;
	union MyUnion Fils4;
} SNoeud;
