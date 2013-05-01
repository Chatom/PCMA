/**
 *
 * Pascal Compiler to Mips Architecture
 *
 * @file: Declaration.h
 *
 * @author: Tom CHASSAGNE
 *
 * @date: 10/04/13
 *
 * @brief: Variables globales et enums du compilateur
 *
**/

#if ! defined __DECLARATION_H__
#define __DECLARATION_H__

// Variables globales
FILE * yyin;
FILE * FileOut;
char yytext [1000];
int verbose = 0;
int line = 1;
int lexeme;


// Type des lexèmes
enum {
	PROGRAM = 256,
	ID,
	NOMBRE,
	
	DIFFERENT,
	INFEGAL,
	SUPEGAL,
	AFFECT,
	INTERV,
	
	DIV,
	MOD,
	OR,
	AND,
	NOT,
	VAR,
	INTEGER,
	BOOLEAN,
	TRUE,
	FALSE,
	PROCEDURE,
	FUNCTION,
	
	BEGIN,
	END,
	WHILE,
	DO,
	IF,
	THEN,
	ELSE,
	READLN,
	WRITELN,
	ARRAY,
	OF
};

// Etats de l'automate
enum {
	FIRSTCHAR=0,
	DIGIT,
	ALPHA,
	DPOINT,
	POINT,
	INF,
	SUP
};

// Type de noeuds de l'arbre
enum {
	PROGRAMPAS,
	BODY,
	DECLFONC,
	DECLPROC,
	PARAM,
	DECLVAR,
	TYPESIMPLE,
	INSTRUCTION,
	WHILEDO,
	IFTHENELSE,
	AFFECTATION,
	APPELPROC,
	ARGUMENT,
	BLOC,
	EXPRESSION,
	CONJONCTION,
	COMPARAISON,
	SUITECOMPAR,
	EXPARITH,
	TERME,
	FACTEUR,
	LISTEEXP,
	VARIABLE
};



#endif /* __DECLARATION_H__ */
