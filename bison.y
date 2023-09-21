%{
#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

int yylex();
%}

%debug

%locations

%start CompUnit
%token nun_INT num_FLOAT Y_ID Y_INT Y_FLOAT Y_VOID Y_CONST Y_IF Y_ELSE Y_WHILE Y_BREAK Y_CONTINUE Y_RETURN Y_ADD Y_SUB Y_MUL Y_DIV Y_MODULO Y_LESS Y_LESSEQ Y_GREAT Y_GREATEQ Y_NOTEQ Y_EQ Y_NOT Y_AND Y_OR Y_ASSIGN Y_LPAP Y_RPAP Y_LBRACKET Y_RBRACKET Y_LSQUARE Y_RSQUARE Y_COMMA Y_SEMICOLON Y_FLOAT


%%
CompUnit: [CompUnit](Decl | FuncDef)

Deel: 

int yywrap()
{
    return 1;
}

int main()
{
	extern FILE *yyin;
	if(argc > 1 && (yyin = fopen(args[1], "r")) == NULL) {
		fprintf(stderr, "can not open %s\n", args[1]);
		exit(1);
	}
	if(yyparse()) {
		exit(-1);
	}
    return 0;
}

void yyerror(char *s, ...)
{
    extern int yylineno;

    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}