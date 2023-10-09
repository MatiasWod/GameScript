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
var             [a-zA-Z0-9_]
allcharacters  [a-zA-Z0-9_ \t\v]
optionalThis    "."[{letter}{var}+]
char           '{letter}'
string          "{letter}{var}+"
 // params allow to have string, char or {letter}{var}+ for example (0, "hola", 'a', var1, var2, var3)
params         (string|char|{letter}{var}+)(,{justspace}*(string|char|{letter}{var}+))*
/* condition     "("({var}(!{justspace}+)?("|!="{justspace}+"{var}"{justspace}*")"({justspace}+"{var})?")" */

%%

"//"                                        { BEGIN(COMMENTLINE); BeginCommentPatternAction(); }
<COMMENTLINE>{endline}                      { EndCommentPatternAction(); BEGIN(INITIAL); }
<COMMENTLINE>.								{ IgnoredPatternAction(yytext, yyleng); }

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }


"++"                                        { return IncrementPatternAction(); }
"--"                                        { return DecrementPatternAction(); }
"+="                                        { return PlusEqualPatternAction(); }
"-="                                        { return MinusEqualPatternAction(); }
"*="                                        { return MulEqualPatternAction(); }
"/="                                        { return SlashEqualPatternAction();}

"+"											{ return AdditionOperatorPatternAction(); }
"-"											{ return SubtractionOperatorPatternAction(); }
"*"											{ return MultiplicationOperatorPatternAction(); }
"/"											{ return DivisionOperatorPatternAction(); }

"("											{ return OpenParenthesisPatternAction(); }
")"											{ return CloseParenthesisPatternAction(); }

"{"                                        { return OpenBracePatternAction(); }
"}"                                       { return CloseBracePatternAction(); }

"["                                        { return OpenBracketPatternAction(); }
"]"                                       { return CloseBracketPatternAction(); }


"char"                                      { return CharPatternAction();             }
"string"                                    { return StringPatternAction(); }
"int"                                       { return IntPatternAction(); }
"bool"                                      { return BoolPatternAction(); }
"block"                                     { return BlockPatternAction();}
"gobject"                                   { return GobjectPatternAction();}
"scene"                                     { return ScenePatternAction();}   
"button"                                    { return ButtonPatternAction();}
"text"                                      { return TextPatternAction();}

"when"                                      { return WhenPatternAction(); }
"for"                                       { return ForPatternAction();} 
"while"                                     { return WhilePatternAction(); }
"if"                                        { return IfPatternAction(); }
"elif"                                      { return ElifPatternAction();}
"else"                                      { return ElsePatternAction();}
"in"                                        { return InPatternAction();}
"hits"                                      { return HitsPatternAction();}

"<"                                         { return LessThanPatternAction(); }
">"                                         { return GreaterThanPatternAction(); }
"="                                         { return EqualPatternAction(); }
"!="                                        { return NotEqualPatternAction(); }
"<="                                        { return LessThanOrEqualPatternAction(); }
">="                                        { return GreaterThanOrEqualPatternAction(); }
"=="                                        { return EqualEqualPatternAction(); }



"!"{var}                                    { return NegationPatternAction(yytext,yyleng); }
"NOT"                                       { return NotPatternAction();}
"not"                                       { return NotPatternAction();}
"AND"                                       { return AndPatternAction();}        
"and"                                       { return AndPatternAction();}
"OR"                                        { return OrPatternAction();}        
"or"                                        { return OrPatternAction();}

"NOKEY"                                    { return NokeyPatternAction(); }
"UP_BORDER"                             { return UpBorderPatternAction(); }
"DOWN_BORDER"                           { return DownBorderPatternAction(); }
"RIGHT_BORDER"                          { return RightBorderPatternAction(); }
"LEFT_BORDER"                           { return LeftBorderPatternAction(); }
"UP"                                      { return UpPatternAction(); }
"DOWN"                                   { return DownPatternAction(); }
"RIGHT"                                  { return RightPatternAction(); }
"LEFT"                                   { return LeftPatternAction(); }
"SPACE"                                  { return SpacePatternAction(); }
"ENTER"                                  { return EnterPatternAction(); }


 /* {letter}{var}*                              { return VarPatternAction(yytext,yyleng);} */
"\""                                        { BEGIN(STRING);  return QuotePatternAction();  }
<STRING>{allcharacters}+								    { return StringTextPatternAction(yytext,yyleng); /*todo*/ }
<STRING>"\""								{  BEGIN(INITIAL); return QuotePatternAction();   }
<STRING>.								{ IgnoredPatternAction(yytext, yyleng); /*todo*/ }
"return"                                    { return ReturnPatternAction(); }
"\'"{letter}"\'"                            { return CharTextPatternAction(yytext,yyleng); }

"RGB"                                       {return RgbPatternAction();}
"px"                                        {return PxPatternAction();}

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

"#DEFINE"                                      { return ConstPatternAction(); }
"onClick"                                   { return OnclickPatternAction(); }
"STDIN"                                     { return StdinPatternAction(); }
"STDOUT"                                    { return StdoutPatternAction(); }

"this""."{letter}{var}*                                 { return ThisPatternAction(yytext, yyleng); }
"this"                                { return ThisPatternAction(yytext, yyleng); }
"THIS"{optionalThis}?                             { return ThisPatternAction(yytext, yyleng); }
"mainScene"                                { return MainScenePatternAction(); }
{letter}{var}*"."{letter}{var}*                   { return VarPatternAction(yytext, yyleng); }
{letter}{var}*                                    { return VarPatternAction(yytext, yyleng); }
 /* {letter}{var}+"."{letter}{var}+"("{params}")"      { return FunctionPatternAction(yytext, yyleng); } */

";"                                        { return SemicolonPatternAction(); }
","                                       { return CommaPatternAction(); }
"."                                     { return DotPatternAction(); }


"true"                                     { return TruePatternAction(); }
"TRUE"                                    { return TruePatternAction();}
"false"                                    { return FalsePatternAction(); }
"FALSE"                                   { return FalsePatternAction(); }


"NULL"                                 { return NullPatternAction(); }
"null"                                 { return NullPatternAction(); }



{letter}{var}*									{ return VarnamePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

 /* {letter}+                                   { return LetterPatternAction(yytext, yyleng); } */



%%


