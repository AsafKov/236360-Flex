%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

void invalid_char_error();

%}

%option yylineno
%option noyywrap

digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n\r ])
letterdigit     ([a-zA-Z0-9])
string           ([ !#-\[\]-~	])
escape           ([\\ntr\"0])
hex              ([0-7][0-F])
%x STR
%%
void                                                        return VOID;
int                                                         return INT;
byte                                                        return BYTE;
b                                                           return B;
bool                                                        return BOOL;
and                                                         return AND;
or                                                          return OR;
not                                                         return NOT;
true                                                        return TRUE;  
false                                                       return FALSE;  
return                                                      return RETURN;  
if                                                          return IF;  
else                                                        return ELSE;  
while                                                       return WHILE;  
break                                                       return BREAK;  
continue                                                    return CONTINUE;  
;                                                           return SC;  
,                                                           return COMMA;  
\(                                                          return LPAREN;  
\)                                                          return RPAREN;  
\{                                                          return LBRACE;  
\}                                                          return RBRACE;
\=                                                          return ASSIGN;
[<>=!]=|<|>                                                 return RELOP;
[-+*/]                                                      return BINOP;
\/\/[^\n\r]*                                         return COMMENT;
override                                                    return OVERRIDE;
{letter}+{letterdigit}*                                     return ID;
([1-9]+{digit}*)|0                                          return NUM;
{whitespace}                                                ;
\"                                                          BEGIN(STR);
<STR>[\t ]                                                  return STRING;
<STR>{string}                                               return STRING;
<STR>\\[^x]                                                 return STRING;
<STR>\\x{string}?[^\n\r\"]?                                   return STRING;
<STR>\"                                                     {BEGIN(INITIAL); return STRING; }
<STR>[\n\r]                                                       return STRING;
<STR>.                                                      invalid_char_error();
.                                                           invalid_char_error();
%%

void invalid_char_error(){
    printf("Error %s\n", yytext);
    exit(0);
}
