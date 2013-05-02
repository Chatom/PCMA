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

int CptMips;

int Adresse = 0;
SPile * PileGlobale;
char * VarPrec;

int TypeAffectation = NULL;
int TypeComparaison = NULL;
int TypeComparTemp  = NULL;

int DebutTab, FinTab;
int DebutTmp, FinTmp;

int NbParamRestant = NULL;
SSymbole * ParamActuel;
char * NomFonction;

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
			printf("  Symbole:   Nom=%s  |  Type=%d  | Adresse=%d  |  [%d..%d]\n", Symb->Nom, Symb->Type, Symb->Adresse, Symb->Debut, Symb->Fin);
		else if (Symb->NbParam != -1)
			if (Symb->Type == NULL)
				printf("  PROCEDURE:   Nom=%s  |  Type=%d  |            |  NbParam=%d\n", Symb->Nom, Symb->Type, Symb->NbParam);
			else
				printf("  FONCTION:   Nom=%s  |  Type=%d |            |  NbParam=%d\n", Symb->Nom, Symb->Type, Symb->NbParam);
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
	
	for ( ; Symb != NULL; Symb = Symb->Suivant)
	{
		if ( strcmp (Symb->Nom, Nom) == 0 )
			printf("[Error]: var %s is already declared\n", Symb->Nom);
	}
	
} // TestVarExiste ()

// Vérifie si une variable de même type est déclarée et renvoie le type
int TestTypeVar (char * Nom, SPile * Pile, int Type)
{
	/*
	printf("[!] Test si %s est déclarée", Nom);
	
	if (Type == PROCEDURE) printf ("   [PROCEDURE]\n");
	else if (Type == FUNCTION) printf ("   [FUNCTION]\n");
	else printf("\n");
	*/
	
	VarPrec = Nom;
	
	SSymbole * Symb = PileGlobale->TableSymbole;
	
	// Si c'est read/write/writeln, alors c'est ok
	if ( Type == PROCEDURE || FUNCTION )
	{
		if (strcmp (Nom, "write") == 0
			 || strcmp (Nom, "writeln") == 0 )
			return;
		if ( strcmp (Nom, "read") == 0 )
			return INTEGER;
	}
	
	// Recherche dans les variables globales
	for ( ; Symb != NULL; Symb = Symb->Suivant)
	{
		if ( strcmp (Symb->Nom, Nom) == 0 )
		{
			/*
			printf("->    %s [type=%d] est déclarée dans la pile globale\n", Nom, Symb->Type);
			if (Type == PROCEDURE && Symb->NbParam != -1 && Symb->Type == 0)
				printf ("  ->   Procedure !\n");
			else if (Type == FUNCTION && Symb->NbParam != -1 && Symb->Type > 0)
				printf ("  ->   Function !\n");
			
			if (TypeAffectation != NULL && TypeAffectation != Symb->Type)
				printf("     !!! TYPE AFFECT DIFFERENT\n");
			if (TypeComparaison != NULL && TypeComparaison != Symb->Type)
				printf("     !!! TYPE COMPAR DIFFERENT\n");
			*/
			
			AfficherErreurType (Type, Symb);

			return Symb->Type;
		}
	}
	
	// On vérifie si la pile courante est une sous-pile
	//if (Pile->TableSymbole->Type == 256) return;

	// Recherche dans la pile courante si différente
	Symb = Pile->TableSymbole;
	
	for ( ; Symb != NULL; Symb = Symb->Suivant)
	{
		if ( strcmp (Symb->Nom, Nom) == 0 )
		{
			/*
			printf("->    %s [type=%d] est déclarée dans la pile courante\n", Nom, Symb->Type);
			
			if (TypeAffectation != NULL && TypeAffectation != Symb->Type)
				printf("     !!! TYPE AFFECT DIFFERENT\n");
			if (TypeComparaison != NULL && TypeComparaison != Symb->Type)
				printf("     !!! TYPE COMPAR DIFFERENT\n");
			*/
			
			AfficherErreurType (Type, Symb);

			return Symb->Type;
		}
	}

	printf(" ! Variable %s is not declared\n", Nom);
	
} // TestTypeVar ()

// Compare et affiche si erreur le type est différent de celui attendu
void AfficherErreurType (int Type, SSymbole * Symb)
{
//	printf("->    %s [type=%d] est déclarée\n", Symb->Nom, Symb->Type);
	
	/*
	// Affiche si c'est une fonction/procédure
	if (Type == PROCEDURE && Symb->NbParam != -1 && Symb->Type == 0)
		printf ("  ->   Procedure !\n");
	else if (Type == FUNCTION && Symb->NbParam != -1 && Symb->Type > 0)
		printf ("  ->   Function !\n");
	else if (Type == PROCEDURE || Type == FUNCTION)
		printf ("  ->   ERROR !\n");
	*/
	
	// Gère les erreurs de types
	if (TypeAffectation != NULL && TypeAffectation != Symb->Type)
		printf(" ! Wrong affectation types (%d) with variable %s\n",
						Symb->Type, Symb->Nom);
	if (TypeComparaison != NULL && TypeComparaison != Symb->Type)
		printf(" ! Different types (%d) in comparison with variable %s\n",
						Symb->Type, Symb->Nom);
	if (TypeComparaison != NULL && TypeComparaison != INTEGER)
		printf (" ! Comparison type is not an integer, next to variable %s\n",
						Symb->Nom);
	
	// Gère les tableaux
	if (Symb->Fin != NULL)
	{
		// Si c'est un tableau alors j'enregistre l'intervalle
		DebutTmp = Symb->Debut;
		FinTmp = Symb->Fin;
		// Il faut vérifier au niveau de l'accès à la case si c'est dans l'intervalle...
	}
	
	// Gère les fonctions
	if (NomFonction == NULL &&
		Type == PROCEDURE || Type == FUNCTION)
	{
		//printf("\n--- Starting comparing param types in %s\n", Symb->Nom);
		
		NomFonction = Symb->Nom;
		NbParamRestant = Symb->NbParam;
		
		SPile * PileTmp = PileGlobale;
		
		// Cherche la pile de la fonction
		for ( ; PileTmp != NULL; PileTmp = PileTmp->Suivant)
		{
			if ( strcmp (Symb->Nom, PileTmp->TableSymbole->Nom) == 0 )
			{
				if (Symb->NbParam != 0) ParamActuel = PileTmp->TableSymbole->Suivant;
				return;
			}
		}
		
	}
	
	// Vérifie les paramètres
	if (NomFonction != NULL)
	{
		
		//printf("---! Compar Param %d : %s\n", NbParamRestant, ParamActuel->Nom);
		
		if (NbParamRestant <= 0)
		{
			printf(" ! Too much parameters for function call %s\n", NomFonction);
			NomFonction = NbParamRestant = NULL;
			return;
		}
		
		if (ParamActuel->Type != Symb->Type)
		{
			printf(" ! Wrong parameter type for function call %s\n", NomFonction);
		}
		
		--NbParamRestant;
		ParamActuel = ParamActuel->Suivant;
	}
	if (NbParamRestant > 0 && NomFonction == NULL)
	{
		printf(" ! Missing parameters for fonction call %s\n", NomFonction);
		NbParamRestant = NomFonction = NULL;
	}
	
} // AfficherErreurType ()

// Compare et affiche les erreurs pour un type (nombre, bool...)
void AfficherErreurAutre (int Type)
{
	//printf("__ autre [type=%d] utilisé\n", Type);
	
	// Gère les erreurs de types
	if (TypeAffectation != NULL && TypeAffectation != Type)
		printf(" ! Wrong affectation types (%d) next to %s\n", Type, VarPrec);
	if (TypeComparaison != NULL && TypeComparaison != Type)
		printf(" ! Different types (%d) in comparison next to %s\n", Type, VarPrec);
	if (TypeComparaison != NULL && TypeComparaison != INTEGER)
		printf (" ! Comparison type is not an integer next to %s\n", VarPrec);
	
	// Vérifie les paramètres
	if (NomFonction != NULL)
	{
		
		//printf("---! Compar Param %d\n", NbParamRestant);
		
		if (NbParamRestant <= 0)
		{
			printf(" ! Too much parameters for function call %s\n", NomFonction);
			NbParamRestant = NomFonction = NULL;
			return;
		}
		
		if (ParamActuel->Type != Type)
		{
			printf(" ! Wrong parameter type for function call %s\n", NomFonction);
		}
		
		--NbParamRestant;
		ParamActuel = ParamActuel->Suivant;
	}
	if (NbParamRestant > 0 && NomFonction == NULL)
	{
		printf(" ! Missing parameters for fonction call %s\n", NomFonction);
		NbParamRestant = NomFonction = NULL;
	}
	
} // AfficherErreurAutre ()


// Fonction récursive qui crée les tables des symboles dans Pile
// en parcourant l'arbre Racine
void CreerTableSymbole (SNoeud * Racine, SPile * Pile)
{
	int Type, Debut, Fin;
	
	if (Racine->Type == -1) return;
	
	if (Racine->Type == PROGRAMPAS)
	{
		AjouterSymbSurPile (Pile, Racine->Fils1.Nom, PROGRAM, NULL, NULL, -1);
		
		PileGlobale = Pile;
		
		fprintf(FileOut, "main:\n");
		fprintf(DataOut, "\t.data\n");
		
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
		
		GenererMips("exit");
		fprintf(DataOut, "\t.text\n\n");
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
		else if (Racine->Fils2.Fils->Type == ARRAY)
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
			fprintf(DataOut, "%s: \t.word\n", NoeudID->Fils1.Nom);
			
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
	else if (Racine->Type == BLOC)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
	}
	else if (Racine->Type == INSTRUCTION)
	{
		if (Racine->Fils1.Fils != NULL)
			CreerTableSymbole (Racine->Fils1.Fils, Pile);
		
		CptMips = NULL;
		
		if (Racine->Fils2.Frere != NULL)
			CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == WHILEDO)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
	}
	else if (Racine->Type == IFTHENELSE)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
		if (Racine->Fils3.Fils != NULL)
			CreerTableSymbole (Racine->Fils3.Fils, Pile);
	}
	else if (Racine->Type == AFFECTATION)
	{
	//	printf ("AFFECTATION: %s\n", Racine->Fils1.Nom);
		int TypeAffectLocal = TestTypeVar (Racine->Fils1.Nom, Pile, 0);
		
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
		
		TypeAffectation = TypeAffectLocal;
		
		CreerTableSymbole (Racine->Fils3.Fils, Pile);
		
		fprintf(FileOut, "\tsw $t0, %s\n\n", Racine->Fils1.Nom);

		TypeAffectation = NULL;
	}
	else if (Racine->Type == APPELPROC)
	{
		TestTypeVar (Racine->Fils1.Nom, Pile, 0);
		
		// On enregistre l'ancienne fonction
		int NbParamTmp = NbParamRestant;
		SSymbole * ParamTmp = ParamActuel;
		char * NomFoncTmp = NomFonction;
		NomFonction = ParamActuel = NbParamRestant = NULL;
		
	//	printf ("APPELPROC: %s\n", Racine->Fils1.Nom);
		TestTypeVar (Racine->Fils1.Nom, Pile, PROCEDURE);
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
		
		if (NbParamRestant > 0)
			printf(" ! Missing parameters for fonction call %s\n", NomFonction);
		else if (NbParamRestant < 0)
			printf(" ! Too much parameters for function call %s\n", NomFonction);
		
		// Gère l'affichage
		if (Racine->Fils1.Nom != NULL &&
			(strcmp (Racine->Fils1.Nom, "write") == 0 || strcmp (Racine->Fils1.Nom, "writeln") == 0) )
			GenererMips ("write");
		
		// Reset les variables de paramètres
		NbParamRestant = NbParamTmp;
		ParamActuel = ParamTmp;
		NomFonction = NomFoncTmp;
	}
	else if (Racine->Type == ARGUMENT)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		if (Racine->Fils2.Frere != NULL)
			CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == EXPRESSION)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		if (Racine->Fils2.Frere != NULL)
			CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == CONJONCTION)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		if (Racine->Fils2.Frere != NULL)
			CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == COMPARAISON)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
	}
	else if (Racine->Type == SUITECOMPAR)
	{
		if (Racine->Fils1.Nombre != NULL &&
			Racine->Fils2.Fils != NULL)
		{
	//		printf ("SUITECOMPAR: %d\n", Racine->Fils1.Nombre);
			
			// Le type de comparaison doit être int
			
			TypeComparaison = TypeComparTemp;
			CreerTableSymbole (Racine->Fils2.Fils, Pile);
			TypeComparaison = NULL;
		}
	}
	else if (Racine->Type == EXPARITH)
	{
	//	printf ("EXPARITH: %d\n", Racine->Fils1.Nombre);
		CreerTableSymbole (Racine->Fils2.Fils, Pile);
		
		if (Racine->Fils3.Nombre != NULL)
		{
			//printf ("Addition: %d\n", Racine->Fils3.Nombre);
			CreerTableSymbole (Racine->Fils4.Frere, Pile);
			
			// Génère l'opération
			if (Racine->Fils3.Nombre == 43)
				fprintf(FileOut, "\tadd $t%d, $t%d, $t%d\n", --CptMips-1, CptMips-1, CptMips-2);
			else
				fprintf(FileOut, "\tadd $t%d, $t%d, -$t%d\n", --CptMips-1, CptMips-1, CptMips-2);
		}
	}
	else if (Racine->Type == TERME)
	{
		CreerTableSymbole (Racine->Fils1.Fils, Pile);

		if (Racine->Fils2.Nombre != NULL)
		{
		//	printf ("MULTIPL: %d\n", Racine->Fils2.Nombre);
			CreerTableSymbole (Racine->Fils3.Frere, Pile);
			
			// Génère l'opération
			if (Racine->Fils2.Nombre == 42)
				fprintf(FileOut, "\tmul $t%d, $t%d, $t%d\n", --CptMips-1, CptMips-1, CptMips-2);
			else if (Racine->Fils2.Nombre == DIV)
				fprintf(FileOut, "\tdiv $t%d, $t%d\n  mflo $t%d\n", --CptMips-1, CptMips-1, CptMips-2);
			else
				fprintf(FileOut, "\tdiv $t%d, $t%d\n  mfhi $t%d\n", --CptMips-1, CptMips-1, CptMips-2);
		}
	}
	else if (Racine->Type == FACTEUR)
	{
		
		int NbParamTmp;
		SSymbole * ParamTmp;
		char * NomFoncTmp;
	
		if (Racine->Fils1.Nombre != NULL)
		{
	//		printf ("FACTEUR: %d\n", Racine->Fils1.Nombre);
			
			// Si c'est la case d'indice d'un tableau
			if (FinTab != NULL)
			{
				if ( Racine->Fils1.Nombre < DebutTab || Racine->Fils1.Nombre > FinTab )
				{
					// Alors l'indice n'est pas dans l'intervalle
					printf (" ! Array access [%d] not between %d and %d\n",
									Racine->Fils1.Nombre, DebutTab, FinTab);
				}
				DebutTab = FinTab = NULL;
			}
			else
			{
				if (Racine->Fils1.Nombre == TRUE || Racine->Fils1.Nombre == FALSE)
					AfficherErreurAutre(BOOLEAN);
				else
					AfficherErreurAutre(INTEGER);
				
				// Génère le nombre
				if (Racine->Fils1.Nombre == TRUE)
					fprintf(FileOut, "\taddi $t%d, $zero, %d\n", CptMips++, 1);
				else if (Racine->Fils1.Nombre == FALSE)
					fprintf(FileOut, "\taddi $t%d, $zero, %d\n", CptMips++, 0);
				else
					fprintf(FileOut, "\taddi $t%d, $zero, %d\n", CptMips++, Racine->Fils1.Nombre);
			}
		}
		if (Racine->Fils2.Nom != NULL)
		{
	//		printf ("FACTEUR: %s\n", Racine->Fils2.Nom);
			
			int TypeComparLocal;

			// Vérifie si c'est une fonction
			if ( Racine->Fils3.Fils != NULL
				 && Racine->Fils3.Fils->Type == LISTEEXP)
			{
				TestTypeVar (Racine->Fils2.Nom, Pile, 0);
				
				// On enregistre l'ancienne fonction
				NbParamTmp = NbParamRestant;
				ParamTmp = ParamActuel;
				NomFoncTmp = NomFonction;
				NomFonction = ParamActuel = NbParamRestant = NULL;
				
				TestTypeVar (Racine->Fils2.Nom, Pile, FUNCTION);
				
				// Gère les input
				if (Racine->Fils2.Nom != NULL && strcmp (Racine->Fils2.Nom, "read") == 0 )
					GenererMips ("read");
				
			}
			// Autrement si c'est un tableau
			else if (Racine->Fils3.Fils != NULL
					 && Racine->Fils3.Fils->Type == EXPRESSION)
			{
				TestTypeVar (Racine->Fils2.Nom, Pile, ARRAY);
			}
			// Autrement c'est une variable normale
			else
			{
				TypeComparTemp = TestTypeVar (Racine->Fils2.Nom, Pile, 0);
				fprintf(FileOut, "\tlw $t%d, %s\n", CptMips++, Racine->Fils2.Nom);
			}
			
			// Si c'est le terme à gauche de la comparaison
			//if (TypeComparTemp == NULL) TypeComparTemp = TypeComparLocal;
		}
		if (Racine->Fils3.Fils != NULL)
		{
			CreerTableSymbole (Racine->Fils3.Fils, Pile);
			
			// Si on sort d'une fonction
			if (Racine->Fils3.Fils->Type == LISTEEXP)
			{
				if (NbParamRestant > 0)
					printf(" ! Missing parameters for fonction call %s\n", NomFonction);
				else if (NbParamRestant < 0)
					printf(" ! Too much parameters for function call %s\n", NomFonction);
				
				// Reset les variables de paramètres
				NbParamRestant = NbParamTmp;
				ParamActuel = ParamTmp;
				NomFonction = NomFoncTmp;
			}
		}	
	}
	else if (Racine->Type == LISTEEXP)
	{
		if (Racine->Fils1.Fils != NULL)
			CreerTableSymbole (Racine->Fils1.Fils, Pile);
		if (Racine->Fils2.Frere != NULL)
			CreerTableSymbole (Racine->Fils2.Frere, Pile);
	}
	else if (Racine->Type == VARIABLE)
	{
		if (Racine->Fils1.Fils != NULL)
		{
			DebutTab = DebutTmp;
			FinTab = FinTmp;
			CreerTableSymbole (Racine->Fils1.Fils, Pile);
			DebutTab = DebutTmp = FinTab = FinTmp = NULL;
		}
	}
	
	
} // CreerTableSymbole ()

void GenererMips (char * Nom)
{
	if 		(strcmp (Nom, "exit") == 0)
		fprintf(FileOut, "\n\t#exit\n\taddi $v0, $zero, 10\n\tsyscall\n");
	else if (strcmp (Nom, "write") == 0)
		fprintf(FileOut, "\n\t#write\n\tadd $a0, $t0, $zero\n\taddi $v0, $zero, 1\n\tsyscall\n");
	else if (strcmp (Nom, "read") == 0)
		fprintf(FileOut, "\n\t#read\n\taddi $v0, $zero, 5\n\tsyscall\n\tadd $t0, $v0, $zero\n");
	
} // GenererMips ()

