
void CreerTableSymbole (SNoeud * Racine, SPile * Pile)
{
	SSymbole * Haut;
	
	SSymbole * Symbole;

	SNoeud * Noeud = Racine;
	int Type, Debut, Fin;
	
	for (int Adresse = 0; ; )
	{
		if (Noeud->Type == PROGRAMPAS)
		{
			Haut = Symbole = AjouterSymb (Noeud->Fils1.Nom, PROGRAM, NULL, NULL, Adresse);
			Noeud = Noeud->Fils2.Fils;
			Pile->TableSymbole = Symbole;
			++Adresse;
		}
		else if (Noeud->Type == BODY)
		{
			Noeud = Noeud->Fils1.Fils;
			//AfficherArbre (Racine->Fils2.Frere, Niveau);
		}
		else if (Noeud->Type == DECLVAR)
		{
			if (Noeud->Fils2.Fils->Type == TYPESIMPLE)
			{
				Debut = Fin = NULL;
				Type = Noeud->Fils2.Fils->Fils1.Nombre;
			}
			else if (Noeud->Fils2.Fils->Type == ARRAY)
			{
				Debut = Noeud->Fils2.Fils->Fils1.Nombre;
				Fin   = Noeud->Fils2.Fils->Fils2.Nombre;
				Type  = Noeud->Fils2.Fils->Fils3.Nombre;
			}
			
			for (SNoeud * NoeudID = Noeud->Fils1.Fils;
				 NoeudID != NULL;
				 NoeudID = NoeudID->Fils2.Frere)
			{
				TestVarExiste (NoeudID->Fils1.Nom, Pile);
				Symbole->Suivant = AjouterSymb (NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
				Symbole = Symbole->Suivant;
				++Adresse;
				
				//AfficherTableSymb (Haut);
				//printf("\n");
				//AfficherPile (Pile);
				//printf("\n");
			}
			
			break;
		}
		else if (Noeud->Type == DECLFONC)
		{
			printf("DECLFONC\n");
			
			if (Noeud->Fils3.Fils->Type == TYPESIMPLE)
			{
				Debut = Fin = NULL;
				Type = Noeud->Fils3.Fils->Fils1.Nombre;
			}
			else if (Noeud->Fils3.Fils->Type == ARRAY)
			{
				Debut = Noeud->Fils3.Fils->Fils1.Nombre;
				Fin   = Noeud->Fils3.Fils->Fils2.Nombre;
				Type  = Noeud->Fils3.Fils->Fils3.Nombre;
			}
			
			printf ("DECLFONC: %s\n", Noeud->Fils1.Nom);
			
			//Noeud = Noeud->Fils1.Fils;
			
			TestVarExiste (Noeud->Fils1.Nom, Pile);
			Symbole->Suivant = AjouterSymb (Noeud->Fils1.Nom, Type, Debut, Fin, Adresse);
			Symbole = Symbole->Suivant;
			++Adresse;
			
			AfficherPile (Pile);
			
			// Param
			for (SNoeud * NoeudParam = Noeud->Fils2.Fils;
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
					TestVarExiste (NoeudID->Fils1.Nom, Pile);
					Symbole->Suivant = AjouterSymb (NoeudID->Fils1.Nom, Type, Debut, Fin, Adresse);
					Symbole = Symbole->Suivant;
					++Adresse;
				}
				
			}
			
			break;
			
			// Bloc/DeclVar/... : body à gerer
			
			//AfficherArbre (Racine->Fils2.Fils, Niveau + 1); PARAM
			//AfficherArbre (Racine->Fils3.Fils, Niveau + 1); TYPE
			//AfficherArbre (Racine->Fils4.Fils, Niveau + 1); BLOC/DECVAR..
		}
		else if (Racine->Type == PARAM)
		{
			//printf ("PARAM\n");
			//AfficherArbre (Racine->Fils1.Fils, Niveau + 1); // LISTEID
			//AfficherArbre (Racine->Fils2.Fils, Niveau + 1); // TYPE
			//if (Racine->Fils3.Frere != NULL)
			//	AfficherArbre (Racine->Fils3.Frere, Niveau); // PARAM
		}
		
	}
	
} // CreerTableSymbole()
