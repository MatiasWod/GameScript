%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

%x COMMENTLINE

%x STRING



/* Patrones reutilizables. */
hex             0x[0-9a-fA-F]{6}
digit			[0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
justspace       [ \t\v]
letter          [a-zA-Z]
var             [a-zA-Z][a-zA-Z0-9_]*
/* condition     "("({var}(!{justspace}+)?("|!="{justspace}+"{var}"{justspace}*")"({justspace}+"{var})?")" */

%%

"//"                                        { BEGIN(COMMENTLINE); BeginCommentPatternAction(); }
<COMMENTLINE>.								{ IgnoredPatternAction(yytext, yyleng); }
<COMMENTLINE>{endline}                      { EndCommentPatternAction(); BEGIN(INITIAL); }

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }


"+"											{ return AdditionOperatorPatternAction(yytext); }
"-"											{ return SubtractionOperatorPatternAction(yytext); }
"*"											{ return MultiplicationOperatorPatternAction(yytext); }
"/"											{ return DivisionOperatorPatternAction(yytext); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

"char"                                   
"string" 
"int"
"bool"
"block"
"gobject"
"scene"
"button"
"text"

"when"                                      { return WhenPatternAction(yytext); }
"for"
"while"
"if"                                        { return IfPatternAction(yytext); }
"elif"
"else"
"in"

"<"                                         { return LessThanPatternAction(yytext); }
">"                                        { return GreaterThanPatternAction(yytext); }
"="                                        { return EqualPatternAction(yytext); }
"!="                                       { return NotEqualPatternAction(yytext); }
"<="                                      { return LessThanOrEqualPatternAction(yytext); }
">="                                     { return GreaterThanOrEqualPatternAction(yytext); }
"=="                                    { return EqualEqualPatternAction(yytext); }

"++"
"--"
"+="
"-="
"*="
"/="

"!"{var}
"NOT"
"not"
"AND"
"and"
"OR"
"or"

"\""                                        { BEGIN(STRING); /* BeginStringPatternAction(); */ }
<STRING>.									{ IgnoredPatternAction(yytext, yyleng); /*todo*/ }
<STRING>"\""								{ /* EndStringPatternAction(); */ BEGIN(INITIAL); }

"RGB"
"px"

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

{letter}+

%%


