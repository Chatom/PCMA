/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: AnalyseurSyntaxique.c
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Analyse syntaxiquement le programme selon la grammaire fixée
 *
**/


#define Accept(attendu) AcceptInfo(attendu,__FILE__,__LINE__)
#define Erreur(attendu) ErreurInfo(attendu,__FILE__,__LINE__)

void AcceptInfo (int attendu, char * File, int Line)
{
	if (lexeme != attendu)
	{
		//printf ("[Error] %s:%d\n", File, Line);
		
		/*
		if (attendu < 256)
		 printf ("  %c attendu au lieu de %s (%d)\n", attendu, yytext, lexeme);
		else
		 printf ("  %d attendu au lieu de %d (%s)\n", attendu, lexeme, yytext);
		*/
		
		printf ("[Error:%d] line %d :  %s non attendu\n", Line, line, yytext);
		
		exit (EXIT_FAILURE);
	}
	
	if (verbose)
		printf ("%d:\t%d\t%s\n", line, lexeme, yytext);
	
	lexeme = yylex();

} // AcceptInfo ()

void ErreurInfo (int attendu, char * File, int Line)
{
	printf ("[Error:%d] line %d :  %s non attendu\n", Line, line, yytext);
	exit (1);
	
} // ErreurInfo ()


// Pré-déclarations des fonctions d'analyse syntaxique
SNoeud * ProgrammePascal ();
SNoeud * Body ();
SNoeud * DeclarProcOuFunc ();
SNoeud * DeclarFonction ();
SNoeud * DeclarProcedure ();
SNoeud * ListeParam ();
SNoeud * DeclarVariable ();
SNoeud * CorpsDeclVariable ();
SNoeud * SuiteDeclVariable ();
SNoeud * ListeID ();
SNoeud * Type ();
SNoeud * WhileDo ();
SNoeud * IfThenElse ();
SNoeud * Affectation ();
SNoeud * AppelProcedure ();
SNoeud * ListeArguments ();
SNoeud * Bloc ();
SNoeud * Expression ();
SNoeud * Conjonction ();
SNoeud * Comparaison ();
SNoeud * Suite_Comparaison ();
SNoeud * ExpressionArithmetique ();
SNoeud * Terme ();
SNoeud * Facteur ();
SNoeud * ListeExpressions ();
SNoeud * Variable ();


SNoeud * ProgrammePascal ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = PROGRAMPAS;

	Accept (PROGRAM);
	
	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (ID);
	
	Accept (';');
	Noeud->Fils2.Fils = Body ();
	Accept ('.');
	
	return Noeud;
	
} // ProgrammePascal ()

SNoeud * Body ()
{
	// Récursivité terminale

	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = BODY;

	if (lexeme == VAR)
	{
		Noeud->Fils1.Fils = DeclarVariable();
		Noeud->Fils2.Frere = Body();
	}
	else if (lexeme == FUNCTION || lexeme == PROCEDURE)
	{
		Noeud->Fils1.Fils = DeclarProcOuFunc();
		Accept (';');
		Noeud->Fils2.Frere = Body();
	}
	else if (lexeme == BEGIN)
	{
		return Bloc();
		//return;
	}
	else Erreur (lexeme);
	
	return Noeud;
	
	/*
	Erreur_Else :
	
	else
	{
		printf ("[Error] line %d\n", __LINE__);
		//exit (1);
	}
	*/

} // Body ()

SNoeud * DeclarProcOuFunc ()
{
	if (lexeme == FUNCTION)
	{
		return DeclarFonction ();
	}
	else if (lexeme == PROCEDURE)
	{
		return DeclarProcedure ();
	}
	else Erreur (lexeme);
	
} // DeclarProcOuFunc ()

SNoeud * DeclarFonction ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = DECLFONC;

	Accept (FUNCTION);

	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (ID);
	
	Accept ('(');
	Noeud->Fils2.Fils = ListeParam ();
	
	Accept (')');
	Accept (':');
	Noeud->Fils3.Fils = Type ();
	Accept (';');
	Noeud->Fils4.Fils = Body ();
	
	return Noeud;
	
} // DeclarFonction ()

SNoeud * DeclarProcedure ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = DECLPROC;
	
	Accept (PROCEDURE);
	
	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (ID);
	
	Accept ('(');
	Noeud->Fils2.Fils = ListeParam ();
	Accept (')');
	Accept (';');
	Noeud->Fils3.Fils = Body ();
	
	return Noeud;
	
} // DeclarProcedure ()

SNoeud * ListeParam ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	
	if (lexeme == ID)
	{
		Noeud->Type = PARAM;
		
		Noeud->Fils1.Fils = ListeID ();
		Accept (':');
		Noeud->Fils2.Fils = Type ();

		if (lexeme == ';')
		{
			Accept (';');
			Noeud->Fils3.Frere = ListeParam ();
		}
	}
	else if (lexeme == ')'); //return;
	else Erreur (lexeme);
	
	return Noeud;
	
} // ListeParam ()

SNoeud * DeclarVariable ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();

	if (lexeme == VAR)
	{
		Accept (VAR);
		return CorpsDeclVariable ();
		//Noeud->Type = VAR;
		//Noeud->Fils1.Fils = CorpsDeclVariable ();
	}
	else if (lexeme == FUNCTION  || 
			 lexeme == PROCEDURE ||
			 lexeme == BEGIN);
		//return;
	else Erreur (lexeme);
	
	return Noeud;
	
} // DeclarVariable ()

SNoeud * CorpsDeclVariable ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();

	if (lexeme == ID)
	{
		Noeud->Type = DECLVAR;
		Noeud->Fils1.Fils = ListeID ();
		Accept (':');
		Noeud->Fils2.Fils = Type ();
		Accept (';');
		Noeud->Fils3.Fils = SuiteDeclVariable ();
	}
	else Erreur (lexeme);
	
	return Noeud;
	
} // CorpsDeclVariable ()

SNoeud * SuiteDeclVariable ()
{
	if (    lexeme == VAR
	     || lexeme == FUNCTION
	     || lexeme == PROCEDURE
	     || lexeme == BEGIN)
		return DeclarVariable ();
		
	else if (lexeme == ID)
		return CorpsDeclVariable ();
	
	else Erreur (lexeme);
	
} // SUiteDeclVariable ()

SNoeud * ListeID ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = ID;
	
	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (ID);

	if (lexeme == ',')
	{
		Accept (',');
		Noeud->Fils2.Frere = ListeID ();
	}
	
	return Noeud;

} // ListeID ()

SNoeud * Type ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	
	if (lexeme == ARRAY)
	{
		Noeud->Type = ARRAY;

		Accept (ARRAY);
		Accept ('[');
		Noeud->Fils1.Nombre = atoi (yytext);
		Accept (NOMBRE);
		Accept (INTERV);
		Noeud->Fils2.Nombre = atoi (yytext);
		Accept (NOMBRE);
		Accept (']');
		Accept (OF);
		
		Noeud->Fils3.Nombre = lexeme;
		TypeSimple ();
	}
	else if (lexeme == INTEGER ||
			 lexeme == BOOLEAN)
	{
		Noeud->Type = TYPESIMPLE;
		Noeud->Fils1.Nombre = lexeme;
		TypeSimple ();
	}
	else Erreur (lexeme);
	
	return Noeud;

} // Type ()

void TypeSimple ()
{
	if (lexeme == INTEGER) Accept (INTEGER);
	else if (lexeme == BOOLEAN)  Accept (BOOLEAN);
	
	else Erreur (lexeme);
	
} // TypeSimple ()

SNoeud * Instruction ()
{
	if (lexeme == WHILE) return WhileDo ();
	else if (lexeme == BEGIN) return Bloc ();
	else if (lexeme == IF) return IfThenElse ();
	else if (lexeme == ID)
	{
		SNoeud * Noeud;
		
		char * IdTmp = CreerChaine (yytext);
		Accept (ID);
		
		if (lexeme == '(') Noeud = AppelProcedure ();
		else Noeud = Affectation ();
		
		Noeud->Fils1.Nom = IdTmp;
	}

	else Erreur (lexeme);
	
} // Instruction ()

SNoeud * ListeInstructions ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	
	if (lexeme == END) return Noeud;
	
	Noeud->Type = INSTRUCTION;
	
	Noeud->Fils1.Fils = Instruction ();
	if (lexeme == ';')
	{
		Accept (';');
		Noeud->Fils2.Frere = ListeInstructions ();
	}
	
	return Noeud;

} // ListeInstructions ()

SNoeud * WhileDo ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = WHILEDO;

	if (lexeme == WHILE)
	{
		Accept (WHILE);
		Noeud->Fils1.Fils = Expression ();
		Accept (DO);
		Noeud->Fils2.Fils = Instruction ();
	}
	
	return Noeud;
	
} // WhileDo ()

SNoeud * IfThenElse ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	
	Noeud->Type = IFTHENELSE;
	
	Accept (IF);
	Noeud->Fils1.Fils = Expression ();
	Accept (THEN);
	Noeud->Fils2.Fils = Instruction ();
	if (lexeme == ELSE)
	{
		Accept (ELSE);
		Noeud->Fils3.Fils = Instruction ();
	}
	
	return Noeud;

} // IfThenElse ()

SNoeud * Affectation ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = AFFECTATION;
	// Fils1 sera initialisé après le return
	
	Noeud->Fils2.Fils = Variable ();
	Accept (AFFECT);
	Noeud->Fils3.Fils = Expression ();
	
	return Noeud;

} // Affectation ()

SNoeud * AppelProcedure ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = APPELPROC;
	
	// Fils1 sera initialisé après le return
	
	Accept ('(');
	Noeud->Fils2.Fils = ListeArguments ();
	Accept (')');
	
	return Noeud;
	
} // AppelProcedure ()

SNoeud * ListeArguments ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = ARGUMENT;

	if (lexeme == ')') return Noeud;
	else if (lexeme == '-' || lexeme == '+'  ||
			 lexeme == NOT || lexeme == '('  ||
			 lexeme == ID  || lexeme == TRUE ||
			 lexeme == FALSE || lexeme == NOMBRE)
	{
		Noeud->Fils1.Fils = Expression ();
		
		if (lexeme == ',')
		{
			Accept (',');
			Noeud->Fils2.Frere = ListeArguments ();
		}
	}
	else Erreur (lexeme);
	
	return Noeud;
	
} // ListeArguments ()

SNoeud * Bloc ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = BLOC;

	Accept (BEGIN);
	Noeud->Fils1.Fils = ListeInstructions ();
	Accept (END);
	
	return Noeud;
	
} // Bloc ()

int Signe ()
{
	if (lexeme == '+')
	{
		Accept ('+');
		return '+';
	}
	else if (lexeme == '-')
	{
		Accept ('-');
		return '-';
	}
	else if (lexeme == '('  || lexeme == ID ||
			 lexeme == TRUE || lexeme == FALSE ||
			 lexeme == NOMBRE)
		return;

	// ??  else Erreur (lexeme);
	
} // Signe ()

int Addition ()
{
	if (lexeme == '+')
	{
		Accept ('+');
		return '+';
	}
	else if (lexeme == '-')
	{
		Accept ('-');
		return '-';
	}
	
	else Erreur (lexeme);
	
} // Addition ()

int Multiplication ()
{
	if (lexeme == '*')
	{
		Accept ('*');
		return '*';
	}
	else if (lexeme == DIV)
	{
		Accept (DIV);
		return DIV;
	}
	else if (lexeme == MOD)
	{
		Accept (MOD);
		return MOD;
	}
	else Erreur (lexeme);
	
} // Multiplication ()

int OperateurRelationnel ()
{
	int LexTmp = lexeme;
	
	if 		(lexeme == '<') Accept ('<');
	else if (lexeme == '=') Accept ('=');
	else if (lexeme == '>') Accept ('>');
	else if (lexeme == INFEGAL)   Accept (INFEGAL);
	else if (lexeme == DIFFERENT) Accept (DIFFERENT);
	else if (lexeme == SUPEGAL)   Accept (SUPEGAL);
	
	// ??  else Erreur (lexeme);
	
	return LexTmp;

} // OperateurRelationnel ()

int Boolean ()
{
	int LexTmp = lexeme;
	
	if (lexeme == TRUE)  Accept (TRUE);
	else if (lexeme == FALSE) Accept (FALSE);
	
	else Erreur (lexeme);
	
	return LexTmp;

} // Boolean ()

SNoeud * Expression ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = EXPRESSION;
	
	Noeud->Fils1.Fils = Conjonction ();

	if (lexeme == OR)
	{
		Accept (OR);
		Noeud->Fils2.Frere = Expression ();
	}
	
	return Noeud;

} // Expression ()

SNoeud * Conjonction ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = CONJONCTION;

	Noeud->Fils1.Fils =	Comparaison ();
	
	if (lexeme == AND)
	{
		Accept (AND);
		Noeud->Fils2.Frere = Conjonction ();
	}
	
	return Noeud;
	
} // Conjonction ()

SNoeud * Comparaison ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = COMPARAISON;
	
	if (lexeme == NOT)
	{
		Accept (NOT);
	}
	Noeud->Fils1.Fils = ExpressionArithmetique ();
	Noeud->Fils2.Fils = Suite_Comparaison ();
	
	return Noeud;

} // Comparaison ()

SNoeud * Suite_Comparaison ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	
	if (lexeme == '<' || lexeme == '='|| 
		lexeme == '>' || lexeme == INFEGAL ||
		lexeme == DIFFERENT || lexeme == SUPEGAL)
	{
		Noeud->Type = SUITECOMPAR;
		Noeud->Fils1.Nombre = OperateurRelationnel ();
		Noeud->Fils2.Fils = ExpressionArithmetique ();
	}
	else if (lexeme != AND  || lexeme != OR  ||
			lexeme != ')'  || lexeme != ',' ||
			lexeme != ']'  || lexeme != ';' ||
			lexeme != THEN || lexeme != DO  ||
			lexeme != END);
	
	return Noeud;
	
} // Suite_Comparaison ()

SNoeud * ExpressionArithmetique ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = EXPARITH;
	
	Noeud->Fils1.Nombre = Signe ();
	Noeud->Fils2.Fils = Terme ();
	if (lexeme == '+' || lexeme == '-')
	{
		Noeud->Fils3.Nombre = Addition ();
		Noeud->Fils4.Frere = ExpressionArithmetique ();
	}

	return Noeud;
	
} // ExpressionArithmetique ()

SNoeud * Terme ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = TERME;

	Noeud->Fils1.Fils = Facteur ();
	if (lexeme == '*' || lexeme == DIV ||
		lexeme == MOD)
	{
		Noeud->Fils2.Nombre = Multiplication ();
		Noeud->Fils3.Frere = Terme ();
	}
	
	return Noeud;

} // Terme ()

SNoeud * Facteur ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = FACTEUR;

	if (lexeme == '(')
	{
		Accept ('(');
		return Expression ();
		Accept (')');
	}
	else if (lexeme == ID)
	{
		Noeud->Fils2.Nom = CreerChaine (yytext);
		//Noeud->Type = ID;
		Accept (ID);
		
		if (lexeme == '(')
		{
			Accept ('(');
			Noeud->Fils3.Fils = ListeExpressions ();
			Accept (')');
		}
		else if (lexeme == '[')
			Noeud->Fils3.Fils = Variable ();
	}
	else if (lexeme == TRUE || lexeme == FALSE)
	{
		//Noeud->Type = Boolean ();
		Noeud->Fils1.Nombre = Boolean ();
	}
	else if (lexeme == NOMBRE)
	{
		//Noeud->Type = NOMBRE;
		Noeud->Fils1.Nombre = atoi (yytext);
		Accept (NOMBRE);
	}
	
	// ?? else Erreur (lexeme);

	return Noeud;

} // Facteur ()

SNoeud * ListeExpressions ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = LISTEEXP;

	if (lexeme == ')');
	else
	{
		Noeud->Fils1.Fils = Expression ();
		if (lexeme == ',')
		{
			Accept (',');
			Noeud->Fils2.Frere = ListeExpressions ();
		}
	}
	
	return Noeud;
	
} // ListeExpressions ()

SNoeud * Variable ()
{
	SNoeud * Noeud;
	Noeud = CreerNoeud ();
	Noeud->Type = VARIABLE;
	
	if (lexeme == '[')
	{
		Accept ('[');
		Noeud->Fils1.Fils = Expression ();
		Accept (']');
	}
	
	return Noeud;
	
} // Variable ()

