%option noyywrap
%{
#include<stdio.h>
%}

digit  [0-9]
letter [a-zA-z]
id     {letter}({letter}|{digit})*
delim  [ \t]
operator [+ = - * < > ; <= >= ==]

%%

{digit}+    {printf("num: %s\n" , yytext);}
{id}        {printf("ident: %s\n" , yytext);}
{delim}     {printf("delim: %s\n" , yytext);}
{operator}  {printf("op: %s\n" , yytext);}
.        {printf("other: %s\n", yytext);}

%%

void main()
{
        yylex();
}

