%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int expression;
	int factor;
	int constant;
	int conditionals;
	int condition;
	int boolean;

	// Terminales.
	token token;
	int integer;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> LESS_THAN
%token <token> GREATER_THAN
%token <token> LESS_THAN_OR_EQUAL
%token <token> GREATER_THAN_OR_EQUAL
%token <token> EQUAL_EQUAL
%token <token> NOT_EQUAL
%token <token> NEGATION
%token <token> STRING
%token <token> EQUAL
%token <token> INT
%token <token> TEXT
%token <token> BLOCK
%token <token> GOBJECT
%token <token> SCENE
%token <token> BUTTON
%token <token> FOR
%token <token> WHILE
%token <token> ELIF
%token <token> ELSE
%token <token> IN
%token <token> INCREMENT
%token <token> DECREMENT
%token <token> PLUS_EQUAL
%token <token> MINUS_EQUAL
%token <token> MUL_EQUAL
%token <token> SLASH_EQUAL
%token <token> NOT
%token <token> AND
%token <token> OR
%token <token> PX
%token <token> LETTER
%token <token> CONST
%token <token> STDIN
%token <token> STDOUT
%token <token> BOOL
%token <token> CHAR
%token <token> RGB
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> SEMICOLON
%token <token> COMMA
%token <token> DOT
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> TRUE
%token <token> FALSE
%token <token> THIS
%token <token> NULL
%token <token> MAIN_SCENE
%token <token> RETURN
%token <token> NAME

%token <token> WHEN
%token <token> IF

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant
%type <conditionals> conditionals
%type <boolean> boolean

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: expression													{ $$ = ProgramGrammarAction($1); }
	;

expression: %empty 												{ $$ = 0 /* EmptyExpressionGrammarAction() */ ; }
	| function expression 						{ $$ = 0 /* FunctionExpressionGrammarAction($1); */ ; }
	| constant expression											{ $$ = 0 /* ConstExpressionGrammarAction($1, $2); */ ; }
	;

function: type NAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE	{ $$ = 0 /* FunctionGrammarAction($1, $2, $4, $7) */ ; }
	;
constant: CONST value 												{ $$ = 0 /* ConstantGrammarAction($1, $2); */ ; }
	;

type: INT															{ $$ = 0 /* IntTypeGrammarAction(); */ ; }
	| TEXT															{ $$ = 0 /* TextTypeGrammarAction(); */ ; }
	| BOOL															{ $$ = 0 /*  BoolTypeGrammarAction(); */ ;}
	| CHAR															{ $$ = 0 /* CharTypeGrammarAction() */ ; }
	| RGB															{ $$ = 0 /* RGBTypeGrammarAction() */ ; }
	| BLOCK															{ $$ = 0 /*  BlockTypeGrammarAction() */ ; }
	| GOBJECT														{ $$ = 0 /* GObjectGrammarAction() */ ; }
	| SCENE															{ $$ = 0 /* SceneGrammarAction() */ ; }
	| BUTTON														{ $$ = 0 /* ButtonGrammarAction() */ ; }
	| STRING 														{ $$ = 0 /* StringGrammarAction() */ ; }
	;

parameters: %empty													{ $$ = 0 /* EmptyParametersGrammarAction() */ ; }
	| type NAME														{ $$ = 0 /* ParametersGrammarAction($1, $2) */ ; }
	| type NAME COMMA parameters									{ $$ = 0 /* ParametersGrammarAction($1, $2, $4) */ ; }
	;

body: %empty														{ $$ = 0 /* EmptyBodyGrammarAction() */ ; }
	| type NAME EQUAL value SEMICOLON body							{ $$ = 0 /* BodyGrammarAction($1, $2, $4, $6); */ ; }	
	| type NAME EQUAL value SEMICOLON								{ $$ = 0 /* BodyGrammarAction($1, $2, $4); */ ;}
	| type NAME SEMICOLON body										{ $$ = 0 /* BodyGrammarAction($1, $2, $4); */ ; }
	| type NAME SEMICOLON											{ $$ = 0 /* BodyGrammarAction($1, $2); */ ; }
	;

value: type 														{ $$ = 0/* TypeValueGrammarAction($1);  */ ; }
	functionvalue   												{ $$ = 0/* FunctionValueGrammarAction($1); */ ; }
;

functionvalue: NAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS	{ $$ = 0 /* FunctionValueGrammarAction($1, $3) */ ; }
	;

// var word OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE
// CONST value
conditionals: IF OPEN_PARENTHESIS boolean CLOSE_PARENTHESIS			{ $$ = ConditionalsGrammarAction(); }
	;

//	(1 < 2) integer[left] MINOR integer[right]
//	| (1 > 2) integer[left] MAJOR integer[right]
//	| (1 <= 2) integer[left] MINOR_EQUAL integer[right]
//	| (1 >= 2) integer[left] MAJOR_EQUAL integer[right]	
//	| (1 == 2) integer[left] EQUAL integer[right]
//	| (1 != 2) integer[left] NOT_EQUAL integer[right]
//	| (true) boolean

boolean: mathexp[left] LESS_THAN mathexp[right]						{ $$ = 0 ;/* MinorBooleanGrammarAction($left, $right); */ }
	| mathexp[left] GREATER_THAN mathexp[right]							{ $$ = 0 ;/* MajorBooleanGrammarAction($left, $right); */ }
	| mathexp[left] LESS_THAN_OR_EQUAL mathexp[right]						{ $$ = 0; /*  MinorEqualBooleanGrammarAction($left, $right); */ }
	| mathexp[left] GREATER_THAN_OR_EQUAL mathexp[right]						{ $$ = 0; /* MajorEqualBooleanGrammarAction($left, $right); */ }
	| mathexp[left] EQUAL_EQUAL mathexp[right]							{ $$ =  EqualBooleanGrammarAction($left, $right);  }
	| mathexp[left] NOT_EQUAL mathexp[right]						{ $$ = 0; /* NotEqualBooleanGrammarAction($left, $right); */ }
	| conditionals													{ $$ = 0; /* ConditionalsBooleanGrammarAction($1); */ }
	;


mathexp: mathexp[left] SUB mathexp[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
		| mathexp[left] MUL mathexp[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
		| mathexp[left] DIV mathexp[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
		| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

%%
