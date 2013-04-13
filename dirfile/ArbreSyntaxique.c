/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: ArbreSyntaxique.c
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Parcours et affichage de l'arbre syntaxique
 *
**/

//#include "ArbreSyntaxique.h"

/*
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

*/

SNoeud * CreerNoeud ()
{
	SNoeud * p = (SNoeud *) malloc (sizeof (SNoeud));
	if (p == NULL)
	{
		fprintf (stderr, "[ERROR] Malloc\n");
		exit (EXIT_FAILURE);
	}

	p->Type = -1; // rajouté
	
	return p;
	
} // CreerNoeud ()

void AfficherArbre (SNoeud * Racine, int Niveau)
{
	if (Racine->Type == -1) return;
	
	for (int i = 0; i < Niveau; ++i)
		printf ("|  ");

	if (Racine->Type == PROGRAMPAS)
	{
		printf ("PROGRAM: %s\n", Racine->Fils1.Nom);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
	}
	else if (Racine->Type == BODY)
	{
		printf ("BODY\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau); // +1
		AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == DECLFONC)
	{
		printf ("DECLFONC: %s\n",Racine->Fils1.Nom);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils4.Fils, Niveau + 1);
	}
	else if (Racine->Type == DECLPROC)
	{
		printf ("DECLPROC: %s\n",Racine->Fils1.Nom);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
	}
	else if (Racine->Type == PARAM)
	{
		printf ("PARAM\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		if (Racine->Fils3.Frere != NULL)
			AfficherArbre (Racine->Fils3.Frere, Niveau);
	}
	else if (Racine->Type == DECLVAR)
	{
		printf ("DECLAR VAR\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		if (Racine->Fils3.Fils != NULL)
			AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
	}
	else if (Racine->Type == ID)
	{
		printf ("IDENTIFIANT: %s\n", Racine->Fils1.Nom);
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == ARRAY)
	{
		printf ("ARRAY: [%d-%d] of %d", Racine->Fils1.Nombre,
										Racine->Fils2.Nombre,
										Racine->Fils3.Nombre);
	}
	else if (Racine->Type == TYPESIMPLE)
	{
		printf ("TYPESIMPLE: %d\n", Racine->Fils1.Nombre);
	}
	else if (Racine->Type == INSTRUCTION)
	{
		printf ("INSTRUCTION\n");
		
		if (Racine->Fils1.Fils != NULL)
			AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == WHILEDO)
	{
		printf ("WHILEDO\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
	}
	else if (Racine->Type == IFTHENELSE)
	{
		printf ("IFTHENELSE\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		if (Racine->Fils3.Fils != NULL)
			AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
	}
	else if (Racine->Type == AFFECTATION)
	{
		printf ("AFFECTATION: %s\n", Racine->Fils1.Nom);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
	}
	else if (Racine->Type == APPELPROC)
	{
		printf ("APPELPROC: %s\n", Racine->Fils1.Nom);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
	}
	else if (Racine->Type == ARGUMENT)
	{
		printf ("ARGUMENT\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == BLOC)
	{
		printf ("BLOC\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
	}
	else if (Racine->Type == EXPRESSION)
	{
		printf ("EXPRESSION\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == CONJONCTION)
	{
		printf ("CONJONCTION\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == COMPARAISON)
	{
		printf ("COMPARAISON\n");
		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
	}
	else if (Racine->Type == SUITECOMPAR)
	{
		printf ("SUITECOMPAR");

		if (Racine->Fils1.Nombre != NULL &&
			Racine->Fils2.Fils != NULL)
		{
			printf (": %d\n", Racine->Fils1.Nombre);
			AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		}
		else
			printf ("\n");
	}
	else if (Racine->Type == EXPARITH)
	{
		printf ("EXPARITH: %d\n", Racine->Fils1.Nombre);
		AfficherArbre (Racine->Fils2.Fils, Niveau + 1);
		if (Racine->Fils3.Nombre != NULL)
		{
			for (int i = 0; i < Niveau + 1; ++i)
				printf ("|  ");
			
			printf ("Addition: %d\n", Racine->Fils3.Nombre);
			AfficherArbre (Racine->Fils4.Frere, Niveau + 1);
		}
	}
	else if (Racine->Type == TERME)
	{
		printf ("TERME\n");

		AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		
		if (Racine->Fils2.Nombre != NULL)
		{
			for (int i = 0; i < Niveau + 1; ++i)
				printf ("|  ");
			
			printf ("MULTIPL: %d\n", Racine->Fils2.Nombre);
		}

		if (Racine->Fils3.Frere != NULL)
			AfficherArbre (Racine->Fils3.Frere, Niveau + 1);
	}
	else if (Racine->Type == FACTEUR)
	{
		if (Racine->Fils1.Nombre != NULL)
			printf ("FACTEUR: %d\n", Racine->Fils1.Nombre);
		if (Racine->Fils2.Nom != NULL)
			printf ("FACTEUR: %s\n", Racine->Fils2.Nom);

		if (Racine->Fils3.Fils != NULL)
		{
			AfficherArbre (Racine->Fils3.Fils, Niveau + 1);
		}
	}
	else if (Racine->Type == LISTEEXP)
	{
		printf ("LISTEEXP\n");
		if (Racine->Fils1.Fils != NULL)
			AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
		if (Racine->Fils2.Frere != NULL)
			AfficherArbre (Racine->Fils2.Frere, Niveau);
	}
	else if (Racine->Type == VARIABLE)
	{
		printf ("VARIABLE\n");
		if (Racine->Fils1.Fils != NULL)
			AfficherArbre (Racine->Fils1.Fils, Niveau + 1);
	}
	else
	{
		printf ("\n");
	}
	
} // AfficheArbre ()

char * CreerChaine (char * chaine)
{
	char * c;
	
	c = (char *) malloc (strlen (chaine) + 1);

	if (c == NULL)
	{
		fprintf (stderr, "[ERROR] Malloc CreerChaine\n");
		exit (EXIT_FAILURE);
	}
	c = strcpy (c, chaine);
	
	return c;
	
} // CreerChaine ()


