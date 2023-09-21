%{
//王南钦 2022091605017
//2023.9.13
#include "token.h"
#include <string.h>
int line=1;//行数
char identifier[100][10];//变量表
int idcount = 0;//变量数
int l;//左括号数
int r;//右括号数
int area;//变量作用域标记
_YYLVAL yylval;
%}

digit     [0-9]
letter    [A-Za-z]
id        ({letter}|_)({letter}|{digit}|_)*
num_int   (0x|0X)[0-9A-Fa-f]* | (0)[1-7] |[1-9]{digit}*|0 |(0b|0B)[0-1]* 
num_float {num_int}("."{num_int})?
int       "int"
float     "float"
void      "void"
const     "const"
if        "if"
else      "else"
while     "while"
break     "break"
continue  "continue"
return    "return"
%x COMMENT
%%

\n            {++line;}
{num_int}     {yylval.int_value=atof(yytext); return num_INT;}
{num_float}   {yylval.float_value=atof(yytext); return num_FLOAT;}
{int}         {yylval.token=atof(yytext); return Y_INT;}
{float}       {yylval.token=atof(yytext); return Y_FLOAT;}
{void}        {yylval.token=atof(yytext); return Y_VOID;}
{id}          {
    int i;
    int flag;
    for(i=0; i<idcount; i++)
    {
        if(strcmp(identifier[i], yytext) == 0)
        {
            yylval.id_name=identifier[i]; 
            flag = 1;
            break;
        }
    }
    if (flag == 1 && area != 0)
    {
        yylval.id_name = yytext;
        return Y_ID;
    }
    else 
    {
        idcount++;
        strcpy(identifier[idcount], yytext);
        area = 1;
        yylval.id_name= yytext;
        return Y_ID;
    }
}
"{"           {
    l++;
    yylval.token=atof(yytext); 
    return Y_LBRACKET;
    }
"}"           {
    r++;
    yylval.token=atof(yytext); 
    return Y_RBRACKET;
    }
"["           {yylval.token=atof(yytext); return Y_LSQUARE;}
"]"           {yylval.token=atof(yytext); return Y_RSQUARE;}
"("           {yylval.token=atof(yytext); return Y_LPAR;}
")"           {yylval.token=atof(yytext); return Y_RPAR;}
"!"           {yylval.token=atof(yytext); return Y_NOT;}
"*"           {yylval.token=atof(yytext); return Y_MUL;}
"/"           {yylval.token=atof(yytext); return Y_DIV;}
"%"           {yylval.token=atof(yytext); return Y_MODULO;}
"+"           {yylval.token=atof(yytext); return Y_ADD;}
"-"           {yylval.token=atof(yytext); return Y_SUB;}
"<"           {yylval.token=atof(yytext); return Y_GREAT;}
">"           {yylval.token=atof(yytext); return Y_LSQUARE;}
"<="          {yylval.token=atof(yytext); return Y_LESSEQ;}
">="          {yylval.token=atof(yytext); return Y_GREATEQ;}
"="           {yylval.token=atof(yytext); return Y_EQ;}
"=="          {yylval.token=atof(yytext); return Y_ASSIGN;}
","           {yylval.token=atof(yytext); return Y_COMMA;}
";"           {yylval.token=atof(yytext); return Y_SEMICOLON;}
"&&"          {yylval.token=atof(yytext); return Y_AND;}
"||"          {yylval.token=atof(yytext); return Y_OR;}
"!="          {yylval.token=atof(yytext); return Y_NOTEQ;}
"//".*        {}
"/*" {BEGIN COMMENT;}
<COMMENT>"*/" {BEGIN INITIAL;}
<COMMENT>. {}
. {}
%%

int main(){
    yylex();
    printf("%d\n",yylval.token);
    printf("line: %d\n",line);
    return 0;
}

int yywrap(){ 
    return 1;
}