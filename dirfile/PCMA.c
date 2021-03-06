/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: PCMA.c
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: main du compilateur Pascal vers Mips
 *
**/

#include <stdio.h>
#include <stdlib.h>

#include "Declaration.h"
#include "ArbreSyntaxique.h"
#include "TableSymbole.h"

#include "AnalyseurLexical.c"
#include "ArbreSyntaxique.c"
#include "AnalyseurSyntaxique.c"
#include "TableSymbole.c"


int main (int argc, char ** argv)
{
	if (argc == 2 || argc == 3 || argc == 4)
	{
		if (argc >= 3) verbose = 1;
		
		printf("Analyse syntaxique du fichier %s :\n", argv[1]);
		
		yyin = fopen (argv[1], "r"); 
		lexeme = yylex ();
		
		SNoeud * Racine = ProgrammePascal ();
		
		printf ("Analyse syntaxique réussie\n");
		
		if (argc >= 4)
		{
			printf ("\n=> Affichage de l'arbre syntaxique :\n\n");
			AfficherArbre (Racine, 0);
		}
		
		FileOut = fopen (".data.asm", "w");
		DataOut = fopen (".text.asm", "w");
		//FuncOut = fopen (".func.asm", "w");
		
		SPile * Pile;
		Pile = CreerPile (Racine);
		
		printf("\nFichier MIPS généré dans ./out.asm\n");
		
		if (argc >= 4)
			AfficherPile (Pile);
		
		fclose (yyin);
		fclose (FileOut);
		fclose (DataOut);
		
		system ("cat .text.asm .data.asm > out.asm");
		system ("rm .text.asm .data.asm");
		return 0;
	}
	else
	{
		printf ("usage: ./PCMA.run FichierPascal [verbose] [tree && symb]\n");
		return 1;
	}

} // main ();
