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

/*
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


// Ajouts
*/

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
	
	return Symbole;
	
} // AjouterSymb ()

void AjouterSymbSurPile (SPile * Pile, char * Nom, int Type, int Debut, int Fin, int Adresse)
{
	// Ajout du symbole sur le dessus de la pile
	
	if (Pile->TableSymbole == NULL)
		Pile->TableSymbole = AjouterSymb (Nom, Type, Debut, Fin, Adresse);
	else
	{
		SSymbole * Symbole = Pile->TableSymbole;
		while (Symbole->Suivant != NULL) Symbole = Symbole->Suivant;
		
		Symbole->Suivant = AjouterSymb (Nom, Type, Debut, Fin, Adresse);
	}
	/*
	
	SSymbole * Symbole = Pile->TableSymbole;
	
	if (Symbole != NULL)
	{
		while (Symbole->Suivant != NULL) Symbole = Symbole->Suivant;

		Symbole = AjouterSymb (Nom, Type, Debut, Fin, Adresse);

		//Symbole->Suivant = (SSymbole *) malloc (sizeof (SSymbole));
		//Symbole = Symbole->Suivant;
	}
	else
	{
		//Symbole = (SSymbole *) malloc (sizeof (SSymbole));
		Symbole = AjouterSymb (Nom, Type, Debut, Fin, Adresse);
	}
	
	/*
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
	*/

} // AjouterSymbSurPile ()

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

SPile * CreerPile (SNoeud * Racine)
{
	SPile * Pile = (SPile *) malloc (sizeof (SPile));
	
	if (Pile == NULL)
	{
		sprintf (stderr, "Error: malloc ()\n");
		exit (1);
	}
	
//	SSymbole * TableSymbole = (SSymbole *) malloc (sizeof (SSymbole));
	
//	Pile->TableSymbole = TableSymbole;
//	free (TableSymbole);
	
	CreerTableSymbole (Racine, Pile);
	
	return Pile;
	
} // CreerPile ()


// Affichage

void AfficherTableSymb (SSymbole * TableSymb)
{
	SSymbole * Symb = TableSymb;
	
	for ( ; Symb != NULL; Symb = Symb->Suivant)
	{
		printf("  Symbole:   Nom=%s  |  Type=%d  | Adresse=%d\n", Symb->Nom, Symb->Type, Symb->Adresse);
		
		/*
		for (SSymbole * Param = Symb->Param ; Param != NULL; Param = Param->Param)
		{
			printf("Param:     Nom=%s  |  Type=%d\n", Param->Nom, Param->Type);
		}
		*/
	}
	
} // AfficherTableSymb ()

void AfficherPile (SPile * Pile)
{
	printf("\nAffichage des piles :\n");
	
	SPile * PileCourante = Pile;
	
	for ( ; PileCourante != NULL; PileCourante = PileCourante->Suivant)
	{
		printf("Pile :\n");
		AfficherTableSymb (PileCourante->TableSymbole);
	}
	
} // AfficherPile ()


void TestVarExiste (char * Nom, SPile * Pile)
{

	SSymbole * Symb = Pile->TableSymbole;
	
	for ( ; Symb->Suivant != NULL; Symb = Symb->Suivant)
	{
		if ( strcmp (Symb->Nom, Nom) == 0 )
			printf("[Error]: var %s is already declared\n", Symb->Nom);
		
		/*
		for (SSymbole * Param = Symb->Param; Param != NULL; Param = Param->Param)
		{
			if ( strcmp (Param->Nom, Nom) == 0 )
				printf("[Error]: param %s in %s() is already declared\n", Param->Nom, Symb->Nom);
		}
		*/
	}
	
} // TestVarExiste ()

int Adresse = 0;

void CreerTableSymbole (SNoeud * Racine, SPile * Pile)
{
	if (Racine->Type == PROGRAMPAS)
	{
		/*
		if (Symbole == NULL)
			Pile->TableSymbole = AjouterSymb (Racine->Fils1.Nom, PROGRAM, NULL, NULL, Adresse);
		else
		{
			Symbole->Suivant = AjouterSymb (Racine->Fils1.Nom, PROGRAM, NULL, NULL, Adresse);
			Symbole = Symbole->Suivant;
		}*/
		
		if (Pile->TableSymbole == NULL) printf("TABLE PGRM NULL\n");
		//Symbole = AjouterSymb (Racine->Fils1.Nom, PROGRAM, NULL, NULL, Adresse);
		AjouterSymbSurPile (Pile, Racine->Fils1.Nom, PROGRAM, NULL, NULL, Adresse);
		AfficherPile (Pile);
		
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
	}
	
	SSymbole * Symbole = Pile->TableSymbole;
	//if (Symbole != NULL)
	while (Symbole->Suivant != NULL) Symbole = Symbole->Suivant;
	
	//printf("->    ");
	//AfficherTableSymb(Symbole);
	//printf("\n");
	
	int Type, Debut, Fin;
	
	if (Racine->Type == -1) return;
	
	if (Racine->Type == BODY)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == DECLVAR)
	{
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
		
		for (SNoeud * NoeudID = Racine->Fils1.Fils;
			 NoeudID != NULL;
			 NoeudID = NoeudID->Fils2.Frere)
		{
			printf("var: %s\n", NoeudID->Fils1.Nom);
			
			TestVarExiste (NoeudID->Fils1.Nom, Pile);
			//Symbole->Suivant = AjouterSymb (NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
			AjouterSymbSurPile (Pile, NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
			Symbole = Symbole->Suivant;
			++Adresse;
			
			AfficherPile (Pile);
			printf("\n");
		}
		
		if (Racine->Fils3.Fils != NULL)
			CreerTableSymbole (Racine->Fils3.Fils, Pile);
	}
	else if (Racine->Type == DECLFONC)
	{
		printf ("DECLFONC: %s\n",Racine->Fils1.Nom);
		
		if (Racine->Fils3.Fils->Type == TYPESIMPLE)
		{
			Debut = Fin = NULL;
			Type = Racine->Fils3.Fils->Fils1.Nombre;
		}
		else if (Racine->Fils3.Fils->Type == ARRAY)
		{
			Debut = Racine->Fils3.Fils->Fils1.Nombre;
			Fin   = Racine->Fils3.Fils->Fils2.Nombre;
			Type  = Racine->Fils3.Fils->Fils3.Nombre;
		}
		
		//Noeud = Noeud->Fils1.Fils;
		
		TestVarExiste (Racine->Fils1.Nom, Pile);
		//Symbole->Suivant = AjouterSymb (Racine->Fils1.Nom, Type, Debut, Fin, Adresse);
		AjouterSymbSurPile (Pile, Racine->Fils1.Nom, Type, Debut, Fin, Adresse);
		Symbole = Symbole->Suivant;
		++Adresse;
		
		AfficherPile (Pile);
		
		int NbParam = 0;		
		
		SPile * PileFonc = Pile;
		for ( ; PileFonc->Suivant != NULL; PileFonc = PileFonc->Suivant);
		
		PileFonc->Suivant = AjouterPile ( AjouterSymb(NULL, NULL, NULL, NULL, NULL) );
		PileFonc = PileFonc->Suivant;
		
		SSymbole * SymbParam = PileFonc->TableSymbole;
		
		// Param
		for (SNoeud * NoeudParam = Racine->Fils2.Fils;
			 NoeudParam != NULL;
			 NoeudParam = NoeudParam->Fils3.Frere)
		{
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
			
			for (SNoeud * NoeudID = NoeudParam->Fils1.Fils;
				 NoeudID != NULL;
				 NoeudID = NoeudID->Fils2.Frere)
			{
				printf("Param: %s\n", NoeudID->Fils1.Nom);
				TestVarExiste (NoeudID->Fils1.Nom, PileFonc);
				//SymbParam->Suivant = AjouterSymb (NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				AjouterSymbSurPile (PileFonc, NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				SymbParam = SymbParam->Suivant;
				++Adresse;
			}
			
		}
		
		PileFonc->TableSymbole = PileFonc->TableSymbole->Suivant;
		
		Symbole->NbParam = NbParam;
		
		CreerTableSymbole (Racine->Fils4.Fils, PileFonc);
		
	}
	else if (Racine->Type == DECLPROC)
	{
		printf ("DECLPROC: %s\n",Racine->Fils1.Nom);
		
		//Noeud = Noeud->Fils1.Fils;
		
		TestVarExiste (Racine->Fils1.Nom, Pile);
		Symbole->Suivant = AjouterSymb (Racine->Fils1.Nom, NULL, NULL, NULL, Adresse);
		Symbole = Symbole->Suivant;
		++Adresse;
		
		AfficherPile (Pile);
		
		int NbParam = 0;		
		
		SPile * PileFonc = Pile;
		for ( ; PileFonc->Suivant != NULL; PileFonc = PileFonc->Suivant);
		
		PileFonc->Suivant = AjouterPile ( AjouterSymb(NULL, NULL, NULL, NULL, NULL) );
		PileFonc = PileFonc->Suivant;
		
		SSymbole * SymbParam = PileFonc->TableSymbole;
		
		// Param
		for (SNoeud * NoeudParam = Racine->Fils2.Fils;
			 NoeudParam != NULL;
			 NoeudParam = NoeudParam->Fils3.Frere)
		{
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
			
			for (SNoeud * NoeudID = NoeudParam->Fils1.Fils;
				 NoeudID != NULL;
				 NoeudID = NoeudID->Fils2.Frere)
			{
				printf("Param: %s\n", NoeudID->Fils1.Nom);
				TestVarExiste (NoeudID->Fils1.Nom, PileFonc);
				//SymbParam->Suivant = AjouterSymb (NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				AjouterSymbSurPile (PileFonc, NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				SymbParam = SymbParam->Suivant;
				++Adresse;
			}
			
		}
		
		PileFonc->TableSymbole = PileFonc->TableSymbole->Suivant;
		
		Symbole->NbParam = NbParam;
		
		CreerTableSymbole (Racine->Fils3.Fils, PileFonc);
	}
	
	
} // CreerTableSymbole ()

/* */
