#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include "../../backend/semantic-analysis/symbol-table.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char *copyLexeme(const char *lexeme, const int length)
{
	char *lexemeCopy = (char *)calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction()
{
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void BeginStringPatternAction()
{
	LogDebug("[Flex] [STRING] BeginStringPatternAction.............................");
}

void EndStringPatternAction()
{
	LogDebug("[Flex] [STRING] EndStringPatternAction................................");
}

void EndCommentPatternAction()
{
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token AdditionOperatorPatternAction()
{
	LogDebug("[Flex] AdditionOperatorPatternAction: '+'.");
	yylval.token = ADD;
	return ADD;
}

token CloseParenthesisPatternAction()
{
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token DivisionOperatorPatternAction()
{
	LogDebug("[Flex] DivisionOperatorPatternAction: '/'.");
	yylval.token = DIV;
	return DIV;
}

token IntegerPatternAction(const char *lexeme, const int length)
{
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	char *lexemeCopy = copyLexeme(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	free(lexemeCopy);
	return INTEGER;
}

token LessThanPatternAction()
{
	LogDebug("[Flex] LessThanPatternAction: '<'.");
	yylval.token = LESS_THAN;
	return LESS_THAN;
}

token GreaterThanPatternAction()
{
	LogDebug("[Flex] GreaterThanPatternAction: '>'.");
	yylval.token = GREATER_THAN;
	return GREATER_THAN;
}

token EqualPatternAction()
{
	LogDebug("[Flex] EqualPatternAction: '='.");
	yylval.token = EQUAL;
	return EQUAL;
}

/*void BeginStringPatternAction()
{
	LogDebug("[Flex] BeginStringPatternAction: '\"'.");
}

void EndStringPatternAction()
{
	LogDebug("[Flex] EndStringPatternAction: '\"'.");
}

token StringPatternAction(const char *lexeme, const int length)
{
	LogDebug("[Flex] StringPatternAction: '%s' (length = %d).", lexeme, length);
	char *lexemeCopy = copyLexeme(lexeme, length);
	yylval.token = lexemeCopy;
	return STRING;
}
*/
token WhenPatternAction()
{
	LogDebug("[Flex] WhenPatternAction: 'when'.");
	yylval.token = WHEN;
	return WHEN;
}

token IfPatternAction()
{
	LogDebug("[Flex] IfPatternAction: 'if'.");
	yylval.token = IF;
	return IF;
}

token EqualEqualPatternAction()
{
	LogDebug("[Flex] EqualEqualPatternAction: '=='.");
	yylval.token = EQUAL_EQUAL;
	return EQUAL_EQUAL;
}

token GreaterThanOrEqualPatternAction()
{
	LogDebug("[Flex] GreaterThanOrEqualPatternAction: '>='.");
	yylval.token = GREATER_THAN_OR_EQUAL;
	return GREATER_THAN_OR_EQUAL;
}

token LessThanOrEqualPatternAction()
{
	LogDebug("[Flex] LessThanOrEqualPatternAction: '<='.");
	yylval.token = LESS_THAN_OR_EQUAL;
	return LESS_THAN_OR_EQUAL;
}

token NotEqualPatternAction()
{
	LogDebug("[Flex] NotEqualPatternAction: '!='.");
	yylval.token = NOT_EQUAL;
	return NOT_EQUAL;
}

token NegationPatternAction()
{
	LogDebug("[Flex] NegationPatternAction: '!'.");
	yylval.token = NEGATION;
	return NEGATION;
}

/*token BeginWhenPatternAction() {
	LogDebug("[Flex] BeginWhenPatternAction: 'when'.");
	yylval.token = WHEN;
	return WHEN;
}

token EndWhenPatternAction() {
	LogDebug("[Flex] EndWhenPatternAction: 'end when'.");
	yylval.token = END_WHEN;
	return END_WHEN;

}*/

/*token WordPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] WordPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.string = lexemeCopy;
	return WORD;
}*/

token MultiplicationOperatorPatternAction()
{
	LogDebug("[Flex] MultiplicationOperatorPatternAction: '*'.");
	yylval.token = MUL;
	return MUL;
}

token OpenParenthesisPatternAction()
{
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token SubtractionOperatorPatternAction()
{
	LogDebug("[Flex] SubtractionOperatorPatternAction: '-'.");
	yylval.token = SUB;
	return SUB;
}

token UnknownPatternAction(const char *lexeme, const int length)
{
	char *lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char *lexeme, const int length)
{
	char *lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}

token CharPatternAction()
{
	LogDebug("[Flex] CharPatternAction: 'char'.");
	yylval.token = CHAR;
	return CHAR;
}

token StringPatternAction()
{
	LogDebug("[Flex] StringPatternAction: 'string'.");
	yylval.token = STRING;
	return STRING;
}

token IntPatternAction()
{
	LogDebug("[Flex] IntPatternAction: 'int'.");
	yylval.token = INT;
	return INT;
}

token BoolPatternAction()
{
	LogDebug("[Flex] BoolPatternAction: 'bool'.");
	yylval.token = BOOL;
	return BOOL;
}

token BlockPatternAction()
{
	LogDebug("[Flex] BlockPatternAction: 'block'.");
	yylval.token = BLOCK;
	return BLOCK;
}

token GobjectPatternAction()
{
	LogDebug("[Flex] GobjectPatternAction: 'gobject'.");
	yylval.token = GOBJECT;
	return GOBJECT;
}

token ScenePatternAction()
{
	LogDebug("[Flex] ScenePatternAction: 'scene'.");
	yylval.token = SCENE;
	return SCENE;
}

token ButtonPatternAction()
{
	LogDebug("[Flex] ButtonPatternAction: 'button'.");
	yylval.token = BUTTON;
	return BUTTON;
}

token TextPatternAction()
{
	LogDebug("[Flex] TextPatternAction: 'text'.");
	yylval.token = TEXT;
	return TEXT;
}

token ForPatternAction()
{
	LogDebug("[Flex] ForPatternAction: 'for'.");
	yylval.token = FOR;
	return FOR;
}

token WhilePatternAction()
{
	LogDebug("[Flex] WhilePatternAction: 'while'.");
	yylval.token = WHILE;
	return WHILE;
}

token ElifPatternAction()
{
	LogDebug("[Flex] ElifPatternAction: 'elif'.");
	yylval.token = ELIF;
	return ELIF;
}

token ElsePatternAction()
{
	LogDebug("[Flex] ElsePatternAction: 'else'.");
	yylval.token = ELSE;
	return ELSE;
}

token InPatternAction()
{
	LogDebug("[Flex] InPatternAction: 'in'.");
	yylval.token = IN;
	return IN;
}

token HitsPatternAction()
{
	LogDebug("[Flex] HitsPatternAction: 'hits'.");
	yylval.token = HITS;
	return HITS;
}

token IncrementPatternAction()
{
	LogDebug("[Flex] IncrementPatternAction: '++'.");
	yylval.token = INCREMENT;
	return INCREMENT;
}

token DecrementPatternAction()
{
	LogDebug("[Flex] DecrementPatternAction: '--'.");
	yylval.token = DECREMENT;
	return DECREMENT;
}

token PlusEqualPatternAction()
{
	LogDebug("[Flex] PlusEqualPatternAction: '+='.");
	yylval.token = PLUS_EQUAL;
	return PLUS_EQUAL;
}

token MinusEqualPatternAction()
{
	LogDebug("[Flex] MinusEqualPatternAction: '-='.");
	yylval.token = MINUS_EQUAL;
	return MINUS_EQUAL;
}

token MulEqualPatternAction()
{
	LogDebug("[Flex] MulEqualPatternAction: '*='.");
	yylval.token = MUL_EQUAL;
	return MUL_EQUAL;
}

token SlashEqualPatternAction()
{
	LogDebug("[Flex] SlashEqualPatternAction: '/='.");
	yylval.token = SLASH_EQUAL;
	return SLASH_EQUAL;
}

token NotPatternAction()
{
	LogDebug("[Flex] NotPatternAction: 'NOT'.");
	yylval.token = NOT;
	return NOT;
}

token AndPatternAction()
{
	LogDebug("[Flex] AndPatternAction: 'AND'.");
	yylval.token = AND;
	return AND;
}

token OrPatternAction()
{
	LogDebug("[Flex] OrPatternAction: 'OR'.");
	yylval.token = OR;
	return OR;
}

token RgbPatternAction()
{
	LogDebug("[Flex] RgbPatternAction: 'RGB'.");
	yylval.token = RGB;
	return RGB;
}

token PxPatternAction()
{
	LogDebug("[Flex] PxPatternAction: 'PX'.");
	yylval.token = PX;
	return PX;
}

token LetterPatternAction(const char *lexeme, const int length)
{
	LogDebug("[Flex] LetterPatternAction: 'LETTER'.");
//	yylval.token = LETTER;
//	return LETTER;
	return VARNAME;
}

token ConstPatternAction()
{
	LogDebug("[Flex] ConstPatternAction: 'CONST'.");
	yylval.token = CONST;
	return CONST;
}

token StdinPatternAction()
{
	LogDebug("[Flex] StdinPatternAction: 'STDIN'.");
	yylval.token = STDIN;
	return STDIN;
}

token StdoutPatternAction()
{
	LogDebug("[Flex] StdoutPatternAction: 'STDOUT'.");
	yylval.token = STDOUT;
	return STDOUT;
}

token SemicolonPatternAction()
{
	LogDebug("[Flex] SemicolonPatternAction: ';'.");
	yylval.token = SEMICOLON;
	return SEMICOLON;
}

token CommaPatternAction()
{
	LogDebug("[Flex] CommaPatternAction: ','.");
	yylval.token = COMMA;
	return COMMA;
}

token DotPatternAction()
{
	LogDebug("[Flex] DotPatternAction: '.'.");
	yylval.token = DOT;
	return DOT;
}

token OpenBracePatternAction()
{
	LogDebug("[Flex] OpenBracePatternAction: '{'.");
	StepIntoScope();
	yylval.token = OPEN_BRACE;
	return OPEN_BRACE;
}

token CloseBracePatternAction()
{
	LogDebug("[Flex] CloseBracePatternAction: '}'.");
	LeaveScope();
	yylval.token = CLOSE_BRACE;
	return CLOSE_BRACE;
}

token OpenBracketPatternAction()
{
	LogDebug("[Flex] OpenBracketPatternAction: '['.");
	yylval.token = OPEN_BRACKET;
	return OPEN_BRACKET;
}

token CloseBracketPatternAction()
{
	LogDebug("[Flex] CloseBracketPatternAction: ']'.");
	yylval.token = CLOSE_BRACKET;
	return CLOSE_BRACKET;
}

token TruePatternAction()
{
	LogDebug("[Flex] TruePatternAction: 'true'.");
	yylval.token = TRUE_TOKEN;
	return TRUE_TOKEN;
}

token FalsePatternAction()
{
	LogDebug("[Flex] FalsePatternAction: 'false'.");
	yylval.token = FALSE_TOKEN;
	return FALSE_TOKEN;
}

token ThisPatternAction(const char *lexeme, const int length)
{
	LogDebug("[Flex] ThisPatternAction: 'this'.");
	char *lexemeCopy = copyLexeme(lexeme, length);
	yylval.token = THIS;
	return THIS;
}

token NullPatternAction()
{
	LogDebug("[Flex] NullPatternAction: 'null'.");
	yylval.token = NULL_TOKEN;
	return NULL_TOKEN;
}

token MainScenePatternAction()
{
	LogDebug("[Flex] MainScenePatternAction: 'main_scene'.");
	yylval.token = MAIN_SCENE;
	return MAIN_SCENE;
}

token ReturnPatternAction()
{
	LogDebug("[Flex] ReturnPatternAction: 'return'.");
	yylval.token = RETURN;
	return RETURN;
}

token VarPatternAction(const char *lexeme, const int length)
{
	LogDebug("[Flex] VarPatternAction: '%s' (length = %d).", lexeme, length);
	char *lexemeCopy = copyLexeme(lexeme, length);
	yylval.string = lexemeCopy;
	//return NAME;
	return VARNAME;
}

token VarnamePatternAction(const char * lexeme, const int length) {
	LogDebug("VarnamePatternAction: '%s' (length = %d).", lexeme, length);
    char * varname = (char *) calloc(length + 1, sizeof(char));
    strncpy(varname, lexeme, length);
    yylval.string = varname;
	return VARNAME;
}

token StringTextPatternAction(const char * lexeme, const int length) {
	LogDebug("StringTextPatternAction: '%s' (length = %d).", lexeme, length);
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, lexeme, length);
	yylval.string = string;
	return STRING_TEXT;
}

token ArrayPatternAction(const char * lexeme, const int length)
{
	LogDebug("[Flex] ArrayPatternAction: 'array'.");
	yylval.token = ARRAY;
	return ARRAY;
}

/*token FunctionPatternAction(const char * lexeme, const int length)
{
	LogDebug("[Flex] FunctionPatternAction: 'function'.");
	yylval.token = FUNCTION;
	return FUNCTION;
}*/

token CharTextPatternAction(const char * lexeme, const int length)
{
	LogDebug("[Flex] CharTextPatternAction: '%s' (length = %d).", lexeme, length);
	char * charText = (char *) calloc(length + 1, sizeof(char));
	strncpy(charText, lexeme, length);
	yylval.string = charText;
	return CHAR_TEXT;
}

token NokeyPatternAction(){
	LogDebug("[Flex] NokeyPatternAction: 'nokey'.");
	yylval.token = NOKEY;
	return NOKEY;
}

token UpBorderPatternAction(){
	LogDebug("[Flex] UpBorderPatternAction: 'upborder'.");
	yylval.token = UP_BORDER;
	return UP_BORDER;
}

token DownBorderPatternAction(){
	LogDebug("[Flex] DownBorderPatternAction: 'downborder'.");	
	yylval.token = DOWN_BORDER;
	return DOWN_BORDER;
}

token RightBorderPatternAction(){
	LogDebug("[Flex] RightBorderPatternAction: 'rightborder'.");
	yylval.token = RIGHT_BORDER;
	return RIGHT_BORDER;
}

token LeftBorderPatternAction(){
	LogDebug("[Flex] LeftBorderPatternAction: 'leftborder'.");
	yylval.token = LEFT_BORDER;
	return LEFT_BORDER;
}

token UpPatternAction(){
	LogDebug("[Flex] UpPatternAction: 'up'.");
	yylval.token = UP;
	return UP;
}

token DownPatternAction(){
	LogDebug("[Flex] DownPatternAction: 'down'.");
	yylval.token = DOWN;
	return DOWN;
}

token RightPatternAction(){
	LogDebug("[Flex] RightPatternAction: 'right'.");
	yylval.token = RIGHT;
	return RIGHT;
}

token LeftPatternAction(){
	LogDebug("[Flex] LeftPatternAction: 'left'.");
	yylval.token = LEFT;
	return LEFT;
}

token SpacePatternAction(){
	LogDebug("[Flex] SpacePatternAction: 'space'.");
	yylval.token = SPACE;
	return SPACE;
}

token EnterPatternAction(){
	LogDebug("[Flex] EnterPatternAction: 'enter'.");
	yylval.token = ENTER;
	return ENTER;
}

token QuotePatternAction() {
	LogDebug("[Flex] QuotePatternAction: '\"'.");
	yylval.token = QUOTE;
	return QUOTE;
}

token OnclickPatternAction() {
	LogDebug("[Flex] OnClickPatternAction: 'onclick'.");
	yylval.token = ONCLICK;
	return ONCLICK;
}