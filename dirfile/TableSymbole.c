/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: TableSymbole.c
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Création et vérifications de la table des symboles
 *
**/


// Crée et renvoie un nouveau symbole correspondant aux paramètres
SSymbole * AjouterSymb (char * Nom, int Type, int Debut, int Fin, int Adresse)
{
	SSymbole * Symbole;
	
	Symbole = (SSymbole *) malloc (sizeof (SSymbole));
	
	if (Symbole == NULL)
	{
		sprintf (stderr, "Error: malloc ()\n");
		exit (1);
	}
	
	Symbole->Nom = Nom;
	Symbole->Type = Type;
	Symbole->Debut = Debut;
	Symbole->Fin = Fin;
	Symbole->Adresse = Adresse;
	
	Symbole->NbParam = -1;

	return Symbole;
	
} // AjouterSymb ()

// Ajoute le symbole sur la pile
// Adresse vaut -1 si ce n'est pas une variable
// NULL autrement
SSymbole * AjouterSymbSurPile (SPile * Pile, char * Nom, int Type, int Debut, int Fin, int Adresse)
{
	// Ajout du symbole sur le dessus de la pile
	
	if (Pile->TableSymbole == NULL)
		return Pile->TableSymbole = AjouterSymb (Nom, Type, Debut, Fin, Adresse);
	else
	{
		int NbSymb;
		SSymbole * Symbole = Pile->TableSymbole;
		while (Symbole->Suivant != NULL) 
		{
			Symbole = Symbole->Suivant;
			if (Symbole->NbParam == -1) ++NbSymb;
		}
		
		//if (Adresse != -1) Adresse = NbSymb;

		return Symbole->Suivant = AjouterSymb (Nom, Type, Debut, Fin, Adresse);
	}

} // AjouterSymbSurPile ()

// Crée et retourne une nouvelle pile avec une première Table des Symb.
SPile * AjouterPile (SSymbole * TableSymbole)
{
	SPile * Pile = (SPile *) malloc (sizeof (SPile));
	
	if (Pile == NULL)
	{
		sprintf (stderr, "Error: malloc ()\n");
		exit (1);
	}
	
	Pile->TableSymbole = TableSymbole;
	
	return Pile;
	
} // AjouterPile ()

// Renvoie la table des symboles correspondant à l'arbre donné
SPile * CreerPile (SNoeud * Racine)
{
	SPile * Pile = (SPile *) malloc (sizeof (SPile));
	
	if (Pile == NULL)
	{
		sprintf (stderr, "Error: malloc ()\n");
		exit (1);
	}
	
	CreerTableSymbole (Racine, Pile);
	
	return Pile;
	
} // CreerPile ()


// Affiche la table des symboles donnée en param. selon son type
void AfficherTableSymb (SSymbole * TableSymb)
{
	SSymbole * Symb = TableSymb;
	
	for ( ; Symb != NULL; Symb = Symb->Suivant)
	{
		if (Symb->Fin != NULL)
			printf("  Symbole:   Nom=%s  |  Type=%d  | Adresse=%d  |  [%d - %d]\n", Symb->Nom, Symb->Type, Symb->Adresse, Symb->Debut, Symb->Fin);
		else if (Symb->NbParam != -1)
			if (Symb->Type == NULL)
				printf("  PROCedure:   Nom=%s  |  Type=%d  |            |  NbParam=%d\n", Symb->Nom, Symb->Type, Symb->NbParam);
			else
				printf("  FONCtion:   Nom=%s  |  Type=%d |            |  NbParam=%d\n", Symb->Nom, Symb->Type, Symb->NbParam);
		else
			printf("  Symbole:   Nom=%s  |  Type=%d  | Adresse=%d\n", Symb->Nom, Symb->Type, Symb->Adresse);
		
	}
	
} // AfficherTableSymb ()

// Affiche les tables des symboles de la pile grâce à AfficherTableSymb()
void AfficherPile (SPile * Pile)
{
	printf("\nAffichage des piles :\n");
	
	SPile * PileCourante = Pile;
	
	for ( ; PileCourante != NULL; PileCourante = PileCourante->Suivant)
	{
		printf("Pile :\n");
		AfficherTableSymb (PileCourante->TableSymbole);
	}
	
	printf("____________________\n");
	
} // AfficherPile ()

// Vérifie si une variable de même nom existe déjà dans la pile donnée
void TestVarExiste (char * Nom, SPile * Pile)
{

	SSymbole * Symb = Pile->TableSymbole;
	
	for ( ; Symb->Suivant != NULL; Symb = Symb->Suivant)
	{
		if ( strcmp (Symb->Nom, Nom) == 0 )
			printf("[Error]: var %s is already declared\n", Symb->Nom);
	}
	
} // TestVarExiste ()

int Adresse = 0;

// Fonction récursive qui crée les tables des symboles dans Pile
// en parcourant l'arbre Racine
void CreerTableSymbole (SNoeud * Racine, SPile * Pile)
{
	int Type, Debut, Fin;
	
	if (Racine->Type == -1) return;
	
	if (Racine->Type == PROGRAMPAS)
	{
		AjouterSymbSurPile (Pile, Racine->Fils1.Nom, PROGRAM, NULL, NULL, -1);
		
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
	}
	else if (Racine->Type == BODY)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == DECLVAR)
	{
		// Récupération du type
		if (Racine->Fils2.Fils->Type == TYPESIMPLE)
		{
			Debut = Fin = NULL;
			Type = Racine->Fils2.Fils->Fils1.Nombre;
		}
		else if (Racine->Fils3.Fils->Type == ARRAY)
		{
			Debut = Racine->Fils2.Fils->Fils1.Nombre;
			Fin   = Racine->Fils2.Fils->Fils2.Nombre;
			Type  = Racine->Fils2.Fils->Fils3.Nombre;
		}
		
		// ListeID
		for (SNoeud * NoeudID = Racine->Fils1.Fils;
			 NoeudID != NULL;
			 NoeudID = NoeudID->Fils2.Frere)
		{
			printf("var: %s\n", NoeudID->Fils1.Nom);
			
			TestVarExiste (NoeudID->Fils1.Nom, Pile);
			AjouterSymbSurPile (Pile, NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
			++Adresse;
		}
		
		if (Racine->Fils3.Fils != NULL)
			CreerTableSymbole (Racine->Fils3.Fils, Pile);

	} // DECLVAR
	else if (Racine->Type == DECLFONC || Racine->Type == DECLPROC)
	{
		if (Racine->Type == DECLFONC)
			printf ("DECLFONC: %s\n",Racine->Fils1.Nom);
		if (Racine->Type == DECLPROC)
			printf ("DECLPROC: %s\n",Racine->Fils1.Nom);
		
		Type = Debut = Fin = NULL;
		
		// Récupération du type de la fonc/proc
		if (Racine->Type == DECLFONC)
			if (Racine->Fils3.Fils->Type == TYPESIMPLE)
			{
				Type = Racine->Fils3.Fils->Fils1.Nombre;
			}
			else if (Racine->Fils3.Fils->Type == ARRAY)
			{
				Debut = Racine->Fils3.Fils->Fils1.Nombre;
				Fin   = Racine->Fils3.Fils->Fils2.Nombre;
				Type  = Racine->Fils3.Fils->Fils3.Nombre;
			}
		
		// Test et ajout de la fonc/proc
		TestVarExiste (Racine->Fils1.Nom, Pile);
		SSymbole * Symbole = AjouterSymbSurPile (Pile, Racine->Fils1.Nom, Type, Debut, Fin, -1);
		
		//AfficherPile (Pile);
		
		int NbParam = 0;
		
		// Création  et parcours de la nouvelle pile
		SPile * PileFonc = Pile;
		for ( ; PileFonc->Suivant != NULL; PileFonc = PileFonc->Suivant);
		
		PileFonc->Suivant = AjouterPile (AjouterSymb (Racine->Fils1.Nom, Type, Debut, Fin, -1));
		PileFonc = PileFonc->Suivant;
		
		// Préparation des adresses de la nouvelle pile
		int OldAdr = Adresse;
		Adresse = 0;

		// ListeParametres
		for (SNoeud * NoeudParam = Racine->Fils2.Fils;
			 NoeudParam != NULL;
			 NoeudParam = NoeudParam->Fils3.Frere)
		{
			if (NoeudParam->Fils1.Fils == NULL) break;
			
			// Récupération du type
			if (NoeudParam->Fils2.Fils->Type == TYPESIMPLE)
			{
				Debut = Fin = NULL;
				Type = NoeudParam->Fils2.Fils->Fils1.Nombre;
			}
			else if (NoeudParam->Fils2.Fils->Type == ARRAY)
			{
				Debut = NoeudParam->Fils2.Fils->Fils1.Nombre;
				Fin   = NoeudParam->Fils2.Fils->Fils2.Nombre;
				Type  = NoeudParam->Fils2.Fils->Fils3.Nombre;
			}
			
			// ListeID
			for (SNoeud * NoeudID = NoeudParam->Fils1.Fils;
				 NoeudID != NULL;
				 NoeudID = NoeudID->Fils2.Frere)
			{
				printf("Param: %s\n", NoeudID->Fils1.Nom);
				TestVarExiste (NoeudID->Fils1.Nom, PileFonc);
				AjouterSymbSurPile (PileFonc, NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				++Adresse;
				++NbParam;
			}
			
		}
		
		Symbole->NbParam = NbParam;
		PileFonc->TableSymbole->NbParam = NbParam;
		
		// Appel du body
		if (Racine->Type == DECLPROC)
			CreerTableSymbole (Racine->Fils3.Fils, PileFonc);
		else
			CreerTableSymbole (Racine->Fils4.Fils, PileFonc);
		
		Adresse = OldAdr;

	} // DECLPROC || DECLFUNC
	
} // CreerTableSymbole ()

/* */
