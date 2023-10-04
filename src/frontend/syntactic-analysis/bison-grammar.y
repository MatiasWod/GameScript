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
%token <token> EQUAL

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

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

program: conditionals													{ $$ = ProgramGrammarAction($1); }
	;

conditionals: IF OPEN_PARENTHESIS boolean CLOSE_PARENTHESIS			{ $$ = ConditionalsGrammarAction(); }
	;

//	(1 < 2) integer[left] MINOR integer[right]
//	| (1 > 2) integer[left] MAJOR integer[right]
//	| (1 <= 2) integer[left] MINOR_EQUAL integer[right]
//	| (1 >= 2) integer[left] MAJOR_EQUAL integer[right]	
//	| (1 == 2) integer[left] EQUAL integer[right]
//	| (1 != 2) integer[left] NOT_EQUAL integer[right]
//	| (true) boolean

boolean: expression[left] LESS_THAN expression[right]						{ $$ = 0 ;/* MinorBooleanGrammarAction($left, $right); */ }
	| expression[left] GREATER_THAN expression[right]							{ $$ = 0 ;/* MajorBooleanGrammarAction($left, $right); */ }
	| expression[left] LESS_THAN_OR_EQUAL expression[right]						{ $$ = 0; /*  MinorEqualBooleanGrammarAction($left, $right); */ }
	| expression[left] GREATER_THAN_OR_EQUAL expression[right]						{ $$ = 0; /* MajorEqualBooleanGrammarAction($left, $right); */ }
	| expression[left] EQUAL_EQUAL expression[right]							{ $$ =  EqualBooleanGrammarAction($left, $right);  }
	| expression[left] NOT_EQUAL expression[right]						{ $$ = 0; /* NotEqualBooleanGrammarAction($left, $right); */ }
	| conditionals													{ $$ = 0; /* ConditionalsBooleanGrammarAction($1); */ }
	;


expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

%%
