/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: AnalyseurLexical.c
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Analyse les lexèmes
 *
**/

// Récupère les lexèmes
int yylex ()
{
	int c;
	int posText = 0;
	int etat = FIRSTCHAR;
	
	while (1)
	{
		c = fgetc (yyin);
		
		switch (etat)
		{
			case FIRSTCHAR:
				if ( isspace(c) )
				{
					if (c == '\n') ++line;
					break;
				}
				
				yytext [posText++] = c;
				
				if      ( isalpha(c) || c == '_' ) etat = ALPHA;
				else if ( isdigit(c) ) etat = DIGIT;
				else if ( c == '.' ) etat = POINT;
				else if ( c == ':' ) etat = DPOINT;
				else if ( c == '>' ) etat = SUP;
				else if ( c == '<' ) etat = INF;
				
				else
				{
					//printf("c : %c\n", c);
					yytext[posText++] = '\0';
					return c;
				}
				break;
				
				
			case DIGIT:
				if ( isdigit(c) ) yytext[posText++] = c;
				else
				{
					ungetc (c, yyin);
					yytext [posText++] = '\0';
					//return posText == 2 ? yytext[0] : NOMBRE;
					return NOMBRE;
				}
				break;
				
				
			case ALPHA:
				if ( isalnum(c) || c == '_') yytext [posText++] = c;
				else
				{
					ungetc (c, yyin);
					yytext [posText++] = '\0';
					
					//printf("yytext: %s\n", yytext);
					
					if      (!strcmp(tolower(yytext), "div")) return DIV;
					else if (!strcmp(tolower(yytext), "mod")) return MOD;
					else if (!strcmp(tolower(yytext), "or")) return OR;
					else if (!strcmp(tolower(yytext), "and")) return AND;
					else if (!strcmp(tolower(yytext), "not")) return NOT;
					else if (!strcmp(tolower(yytext), "var")) return VAR;
					else if (!strcmp(tolower(yytext), "integer")) return INTEGER;
					else if (!strcmp(tolower(yytext), "boolean")) return BOOLEAN;
					else if (!strcmp(tolower(yytext), "true")) return TRUE;
					else if (!strcmp(tolower(yytext), "false")) return FALSE;
					else if (!strcmp(tolower(yytext), "procedure")) return PROCEDURE;
					else if (!strcmp(tolower(yytext), "function")) return FUNCTION;
					else if (!strcmp(tolower(yytext), "program")) return PROGRAM;
					else if (!strcmp(tolower(yytext), "begin")) return BEGIN;
					else if (!strcmp(tolower(yytext), "end")) return END;
					else if (!strcmp(tolower(yytext), "while")) return WHILE;
					else if (!strcmp(tolower(yytext), "do")) return DO;
					else if (!strcmp(tolower(yytext), "if")) return IF;
					else if (!strcmp(tolower(yytext), "then")) return THEN;
					else if (!strcmp(tolower(yytext), "else")) return ELSE;
					else if (!strcmp(tolower(yytext), "readln")) return READLN;
					else if (!strcmp(tolower(yytext), "writeln")) return WRITELN;
					else if (!strcmp(tolower(yytext), "array")) return ARRAY;
					else if (!strcmp(tolower(yytext), "of")) return OF;
					
					//return posText == 2 ? yytext[0] : ID;
					return ID;

				}
				break;
				
				
			case POINT:
				if ( c == '.' )
				{
					yytext[posText++] = c;
					yytext[posText++] = '\0';
					return INTERV;
				}
				else if (posText == 1)
				{
					ungetc (c, yyin);
					yytext[posText++] = '\0';
					return '.';
				}
				break;
				
				
			case DPOINT:
				if ( c == '=' )
				{
					yytext[posText++] = c;
					yytext[posText++] = '\0';
					return AFFECT;
				}
				else if (posText == 1)
				{
					ungetc (c, yyin);
					yytext[posText++] = '\0';
					return ':';
				}
				break;
				
				
			case INF:
				if ( c == '>' )
				{
					yytext[posText++] = c;
					yytext[posText++] = '\0';
					return DIFFERENT;
				}
				else if ( c == '=' )
				{
					yytext[posText++] = c;
					yytext[posText++] = '\0';
					return INFEGAL;
				}
				else if (posText == 1)
				{
					ungetc (c, yyin);
					yytext[posText++] = '\0';
					return '<';
				}
				break;
				
				
			case SUP:
				if (c == '=' )
				{
					yytext[posText++] = c;
					yytext[posText++] = '\0';
					return SUPEGAL;
				}
				else if (posText == 1)
				{
					ungetc (c, yyin);
					yytext[posText++] = '\0';
					return '>';
				}
				break;
						
		}
	}

} // yylex ()

