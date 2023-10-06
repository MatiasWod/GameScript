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


"+"											{ return AdditionOperatorPatternAction(); }
"-"											{ return SubtractionOperatorPatternAction(); }
"*"											{ return MultiplicationOperatorPatternAction(); }
"/"											{ return DivisionOperatorPatternAction(); }

"("											{ return OpenParenthesisPatternAction(); }
")"											{ return CloseParenthesisPatternAction(); }

"char"                                      { return CharPatternAction();             }
"string"                                    { return StringPatternAction(); }
"int"                                       { return IntPatternAction(); }
"bool"                                      { return BoolPatternAction(); }
"block"                                     { return BlockPatternAction();}
"gobject"                                   { return GobjectPatternAction();}
"scene"                                     { return ScenePatternAction();}   
"button"                                    {return ButtonPatternAction();}
"text"                                      {return TextPatternAction();}

"when"                                      { return WhenPatternAction(); }
"for"                                       { return ForPatternAction();} 
"while"                                     {return WhilePatternAction();}
"if"                                        { return IfPatternAction(); }
"elif"                                      {return ElifPatternAction();}
"else"                                      {return ElsePatternAction();}
"in"                                        {return InPatternAction();}

"<"                                         { return LessThanPatternAction(); }
">"                                        { return GreaterThanPatternAction(); }
"="                                        { return EqualPatternAction(); }
"!="                                       { return NotEqualPatternAction(); }
"<="                                      { return LessThanOrEqualPatternAction(); }
">="                                     { return GreaterThanOrEqualPatternAction(); }
"=="                                    { return EqualEqualPatternAction(); }

"++"                                        { return IncrementPatternAction(); }
"--"                                        { return DecrementPatternAction(); }
"+="                                        { return PlusEqualPatternAction(); }
"-="                                        { return MinusEqualPatternAction(); }
"*="                                        { return MulEqualPatternAction(); }
"/="                                        {return SlashEqualPatternAction();}

"!"{var}                                    { return NegationPatternAction(yytext,yyleng); }
"NOT"                                       { return NotPatternAction();}
"not"                                       { return NotPatternAction();}
"AND"                                       { return AndPatternAction();}        
"and"                                       { return AndPatternAction();}
"OR"                                        { return OrPatternAction();}        
"or"                                        { return OrPatternAction();}
{var}                                       { return VarPatternAction(const char *lexeme, const int length); }
"\""                                        { BEGIN(STRING);  BeginStringPatternAction();  }
<STRING>.									{ IgnoredPatternAction(yytext, yyleng); /*todo*/ }
<STRING>"\""								{  EndStringPatternAction();  BEGIN(INITIAL); }

"RGB"                                       {return RgbPatternAction();}
"px"                                        {return PxPatternAction();}

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

{letter}+                                   { return LetterPatternAction(yytext, yyleng); }


"#DEFINE"{whitespace}+{letter}+             { return ConstPatternAction(yytext, yyleng); }

"STDIN"                                     { return StdinPatternAction(); }
"STDOUT"                                    { return StdoutPatternAction(); }

";"                                        { return SemicolonPatternAction(); }
","                                       { return CommaPatternAction(); }
"."                                     { return DotPatternAction(); }

"{"                                        { return OpenBracePatternAction(); }
"}"                                       { return CloseBracePatternAction(); }

"["                                        { return OpenBracketPatternAction(); }
"]"                                       { return CloseBracketPatternAction(); }

"true"                                     { return TruePatternAction(); }
"TRUE"                                    { return TruePatternAction();}
"false"                                    { return FalsePatternAction(); }
"FALSE"                                   { return FalsePatternAction(); }

"this"                                   { return ThisPatternAction(); }
"THIS"                                  { return ThisPatternAction(); }

"NULL"                                 { return NullPatternAction(); }
"null"                                 { return NullPatternAction(); }

"mainScene"                                { return MainScenePatternAction(); }
"return"                                    { return ReturnPatternAction(); }
%%


