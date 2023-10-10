#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

token VarnamePatternAction(const char * lexeme, const int length);

// Patrones terminales del lenguaje diseñado.
token AdditionOperatorPatternAction();
token CloseParenthesisPatternAction();
token DivisionOperatorPatternAction();
token IntegerPatternAction(const char *lexeme, const int length);
token MultiplicationOperatorPatternAction();
token OpenParenthesisPatternAction();
token SubtractionOperatorPatternAction();

token LessThanPatternAction();
token GreaterThanPatternAction();
token EqualPatternAction();
token WhenPatternAction();
token IfPatternAction();
token EqualEqualPatternAction();
token GreaterThanOrEqualPatternAction();
token LessThanOrEqualPatternAction();
token NotEqualPatternAction();

// void BeginStringPatternAction();
// token StringPatternAction(yytext, yyleng);
// void EndStringPatternAction();

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char *lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char *lexeme, const int length);

token CharPatternAction();
token StringPatternAction();
token IntPatternAction();
token BoolPatternAction();
token BlockPatternAction();
token GobjectPatternAction();
token ScenePatternAction();
token ButtonPatternAction();
token TextPatternAction();
token ForPatternAction();
token WhilePatternAction();
token ElifPatternAction();
token ElsePatternAction();
token InPatternAction();
token HitsPatternAction();
token IncrementPatternAction();
token DecrementPatternAction();
token PlusEqualPatternAction();
token MinusEqualPatternAction();
token MulEqualPatternAction();
token SlashEqualPatternAction();
token NotPatternAction();
token AndPatternAction();
token OrPatternAction();
token RgbPatternAction();
token PxPatternAction();
token ConstPatternAction();
token StdinPatternAction();
token StdoutPatternAction();
token IntegerPatternAction(const char *lexeme, const int length);
token LetterPatternAction(const char *lexeme, const int length);

token SemicolonPatternAction();
token CommaPatternAction();
token DotPatternAction();
token OpenBracePatternAction();
token CloseBracePatternAction();
token OpenBracketPatternAction();
token CloseBracketPatternAction();
token TruePatternAction();
token FalsePatternAction();
token ThisPatternAction(const char *lexeme, const int length);
token NullPatternAction();
token MainScenePatternAction();
token ReturnPatternAction();
token VarPatternAction(const char *lexeme, const int length);
token FunctionPatternAction(const char * lexeme, const int length);

void BeginStringPatternAction();
void EndStringPatternAction();
token StringTextPatternAction(const char * lexeme, const int length);
token ArrayPatternAction(const char * lexeme, const int length);
token CharTextPatternAction(const char * lexeme, const int length);

token NokeyPatternAction();
token UpBorderPatternAction();
token DownBorderPatternAction();
token RightBorderPatternAction();
token LeftBorderPatternAction();
token UpPatternAction();
token DownPatternAction();
token RightPatternAction();
token LeftPatternAction();
token SpacePatternAction();
token EnterPatternAction();
token OnclickPatternAction();

#endif
