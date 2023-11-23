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
	Program program;
	Mainscene mainscene;
	Expression expression;
	Factor factor;
	Constant constant;
	Conditionals conditionals;
	GSBoolean gsboolean;
	Function function;
	Type type;
	ParametersDef parameters_def;
	Parameters parameters;
	Body body;
	Value value;
	Functionvalue functionvalue;
	Mathexp mathexp;
	char * str;
	char character;
	Constante constante;
	Array array;
	Assignment assignment;
	ReturnValue returnvalue;
	VarSingleAction varsingleaction;
	Negation negation;
	GConstant gconstant;
	FunctionAssignment function_assignment;
	IfOptions if_options;
	ForOptions for_options;
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
%token <str> TEXT
%token <token> BLOCK
%token <token> GOBJECT
%token <token> SCENE
%token <token> BUTTON
%token <token> FOR
%token <token> WHILE
%token <token> ELIF
%token <token> ELSE
%token <token> IN
%token <token> HITS
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
// %token <token> LETTER
%token <token> CONST
%token <token> STDIN
%token <token> STDOUT
%token <token> BOOL

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
%token <token> TRUE_TOKEN
%token <token> FALSE_TOKEN
%token <token> THIS
%token <token> NULL_TOKEN
%token <token> MAIN_SCENE
%token <token> WHEN
%token <token> IF
%token <token> RETURN
%token <str> VARNAME
// %token <token> FUNCTION

// %token <token> NAME
%token <character> CHAR
%token <str> STRING_TEXT
%token <integer> INTEGER
%token <token> ARRAY
%token <token> CHAR_TEXT

%token <token> NOKEY
%token <token> UP_BORDER
%token <token> DOWN_BORDER
%token <token> LEFT_BORDER
%token <token> RIGHT_BORDER
%token <token> UP
%token <token> DOWN
%token <token> LEFT
%token <token> RIGHT
%token <token> SPACE
%token <token> ENTER
%token <token> QUOTE
%token <token> ONCLICK
// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant
%type <constante> constante
%type <conditionals> conditionals
%type <gsboolean> gsboolean
%type <function> function
%type <type> type
%type <parameters_def> parameters_def
%type <body> body
%type <value> value
%type <functionvalue> functionvalue
%type <mathexp> mathexp
%type <array> array
%type <returnvalue> returnValue
%type <mainscene> main_scene
%type <parameters> parameters
%type <function_assignment> function_assignment
%type <gconstant> gconstant
%type <assignment> assignment
%type <if_options> if_options
%type <varsingleaction> varsingleaction
%type <for_options> for_options
%type <negation> negation

// Reglas de asociatividad y precedencia (de menor a mayor).
%left  INCREMENT DECREMENT
%left  ADD SUB
%left  MUL DIV
%left  LESS_THAN GREATER_THAN LESS_THAN_OR_EQUAL GREATER_THAN_OR_EQUAL
%left  EQUAL_EQUAL NOT_EQUAL
%left  AND
%left  OR
%right EQUAL PLUS_EQUAL MINUS_EQUAL MUL_EQUAL SLASH_EQUAL
%left  COMMA


// El símbolo inicial de la gramatica.
%start program

%%

program: main_scene													{ $$ = ProgramGrammarAction($1); }
	;

main_scene: expression MAIN_SCENE OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE 	{ $$ = MainSceneGrammarAction($6, $1); } 
	;
expression: %empty 												{ $$ = ExpressionEmptyGrammarAction(); } 
	| function expression 						{ $$ = ExpressionFunctionGrammarAction($1,$2); } // function
	| constante expression											{ $$ = ExpressionConstanteGrammarAction($1, $2); }
	;

function: type VARNAME OPEN_PARENTHESIS parameters_def CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE	{ $$ = FunctionGrammarAction($1, $2, $4, $7); }
	;
constante: CONST value 												{ $$ = ConstanteGrammarAction($2); }
	;

type: INT															{ $$ = IntTypeGrammarAction(); }
	| TEXT															{ $$ = TextTypeGrammarAction(); }
	| BOOL															{ $$ = BoolTypeGrammarAction(); }
	| CHAR															{ $$ = CharTypeGrammarAction(); }
	| RGB															{ $$ = RgbTypeGrammarAction(); }
	| BLOCK															{ $$ = BlockTypeGrammarAction(); }
	| GOBJECT														{ $$ = GObjectTypeGrammarAction(); }
	| SCENE															{ $$ = SceneTypeGrammarAction(); }
	| BUTTON														{ $$ = ButtonTypeGrammarAction(); }
	| STRING 														{ $$ = StringTypeGrammarAction(); }
	;

parameters_def: %empty													{ $$ = EmptyParametersDefGrammarAction(); }
	| type VARNAME														{ $$ = ParametersDefGrammarAction($1, $2); }
	| type VARNAME COMMA parameters_def									{ $$ = ParametersDefCommaGrammarAction($1, $2, $4); }
	;

parameters: %empty 													{ $$ = EmptyParametersGrammarAction(); }
	| returnValue														{ $$ = ParametersGrammarAction($1); }
	| returnValue COMMA parameters										{ $$ = ParametersCommaGrammarAction($1, $3); }
	;

body: %empty														{ $$ = BodyEmptyGrammarAction(); } 
	| conditionals body													{ $$ = BodyConditionalsGrammarAction($1, $2); }
	| THIS array assignment returnValue SEMICOLON	body      { $$ = BodyThisAssignRetGrammarAction($2,$3,$4,$6); }
	| THIS array assignment functionvalue SEMICOLON body								{ $$ = BodyThisArrayAssignFuncGrammarAction($2,$3,$4,$6); }
	| functionvalue SEMICOLON body											{ $$ = BodyFuncGrammarAction($1, $3); }
	| THIS array functionvalue SEMICOLON body 									{ $$ =  BodyThisArrayFuncGrammarAction($2,$3,$5); }
	| type VARNAME array assignment constant SEMICOLON body							{ $$ = BodyTypeAssignConstGrammarAction($1, $2,$3, $4, $5, $7); }	
	| type VARNAME array SEMICOLON body										{ $$ = BodyTypeArrayGrammarAction($1, $2, $3,$5); }
	| type VARNAME array assignment STRING_TEXT SEMICOLON body							{ $$ = BodyTypeArrayAssignStringGrammarAction($1, $2,$3, $4, $5,$7); }
	| VARNAME array assignment constant SEMICOLON body								{ $$ = BodyVarArrayAssignConstGrammarAction($1, $2,$3, $4, $6); }
	| VARNAME array assignment functionvalue SEMICOLON body								{ $$ = BodyVarArrayAssignFuncGrammarAction($1, $2, $3, $4, $6); }
	| type VARNAME array assignment functionvalue SEMICOLON body     { $$ = BodyTypeVarArrayAssignFuncGrammarAction($1, $2, $3, $4, $5,$7); }
	| VARNAME array varsingleaction SEMICOLON body										{ $$ = BodyVarArraySingleGrammarAction($1, $2, $3, $5); } 
	| RETURN returnValue SEMICOLON											{ $$ = BodyRetGrammarAction($2); }
	| ONCLICK OPEN_PARENTHESIS functionvalue CLOSE_PARENTHESIS SEMICOLON body 						{ $$ = BodyOnClickGrammarAction($3, $6); } 
	;

varsingleaction: INCREMENT														{ $$ =   IncrementVarSingleActionGrammarAction(); }
	| DECREMENT														{ $$ = DecrementVarSingleActionGrammarAction(); }
	;

assignment: EQUAL 												{ $$ = EqualAssignmentGrammarAction(); }
	| PLUS_EQUAL 														{ $$ =   PlusEqualAssignmentGrammarAction(); }
	| MINUS_EQUAL 														{ $$ = MinusEqualAssignmentGrammarAction(); }
	| SLASH_EQUAL 														{ $$ =   SlashEqualAssignmentGrammarAction(); }
	| MUL_EQUAL 														{ $$ =  MulEqualAssignmentGrammarAction(); }
	;
function_assignment: OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS { $$ =   FunctionAssignmentGrammarAction($2); }
	;

array: %empty 														{ $$ =   EmptyArrayGrammarAction(); }
	| OPEN_BRACKET VARNAME CLOSE_BRACKET array										{ $$ =   VarnameArrayGrammarAction($2,$4); }
	| OPEN_BRACKET INTEGER CLOSE_BRACKET array								{ $$ = IntegerArrayGrammarAction($2,$4); }
	| OPEN_BRACKET CLOSE_BRACKET array										{ $$ =  ArrayArrayGrammarAction($3); }
	;

returnValue: constant													{ $$ = ReturnValueGrammarAction($1); }
	| THIS                                                              { $$ =ReturnValueThisGrammarAction(); }
	| CHAR_TEXT														 { $$ = ReturnValueCharTextGrammarAction(); }
	| gconstant															{ $$ =  ReturnValueGconstantGrammarAction($1); }
	| THIS VARNAME array													{ $$ = ReturnValueThisVarnameArrayGrammarAction($2,$3); }
	| THIS functionvalue											{ $$ = ReturnValueThisFunctionvalueGrammarAction($2);}
	| constant SUB constant 															{ $$ = ReturnValueConstantSubGrammarAction($1,$3); }
	| constant ADD constant 															{ $$ = ReturnValueConstantAddGrammarAction($1,$3); }
	| constant MUL constant 															{ $$ = ReturnValueConstantMulGrammarAction($1,$3); }
	| constant DIV constant 															{ $$ = ReturnValueConstantDivGrammarAction($1,$3); }
	;

value: type 														{ $$ =   TypeValueGrammarAction($1); }
	| functionvalue   												{ $$ =   FunctionValueValueGrammarAction($1); }
;

functionvalue: VARNAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS	{ $$ = FunctionValueGrammarAction($1, $3); }
	;

conditionals:  WHEN OPEN_PARENTHESIS negation gsboolean CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE		{ $$ = WhenBodyGrammarAction($3, $4, $7); }
	| WHEN OPEN_PARENTHESIS negation gsboolean CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE ELSE OPEN_BRACE body CLOSE_BRACE	{ $$ = WhenElseBodyGrammarAction($3, $4, $7, $11); }
	| IF OPEN_PARENTHESIS negation gsboolean CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE	if_options	{ $$ = IfBodyGrammarAction($3, $4, $7, $9); }
	| FOR OPEN_PARENTHESIS INT VARNAME EQUAL INTEGER SEMICOLON negation gsboolean SEMICOLON for_options CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE	{ $$ = ForBodyGrammarAction($4, $6, $8, $9, $11, $14); }
	| WHILE OPEN_PARENTHESIS negation gsboolean CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE			{ $$ = WhileBodyGrammarAction($3, $4, $7); }
	;

negation: %empty													{ $$ = EmptyNegationGrammarAction(); }
	| NEGATION														{ $$ = NegationGrammarAction(); }

for_options: %empty													{ $$ = EmptyForOptionsGrammarAction(); }
	| VARNAME INCREMENT														{ $$ = IncForOptionsGrammarAction($1); }
	| VARNAME DECREMENT														{ $$ = DecForOptionsGrammarAction($1); }
	| VARNAME PLUS_EQUAL INTEGER													{ $$ = PlusEqualForOptionsGrammarAction($1, $3); }
	| VARNAME MINUS_EQUAL INTEGER													{ $$ = MinusEqualForOptionsGrammarAction($1, $3); }
	| VARNAME SLASH_EQUAL INTEGER													{ $$ = SlashEqualForOptionsGrammarAction($1, $3); }
	| VARNAME MUL_EQUAL INTEGER													{ $$ = MulEqualForOptionsGrammarAction($1, $3); }
	| VARNAME EQUAL mathexp														{ $$ = EqualForOptionsGrammarAction($1, $3); }
	;

if_options: %empty													{ $$ = EmptyIfOptionsGrammarAction(); }
	| ELIF OPEN_PARENTHESIS gsboolean CLOSE_PARENTHESIS OPEN_BRACE body CLOSE_BRACE if_options { $$ = ElifBodyGrammarAction($3, $6, $8); }
	| ELSE OPEN_BRACE body CLOSE_BRACE										{ $$ = ElseBodyGrammarAction($3); }
	;

gsboolean: mathexp[left] LESS_THAN mathexp[right]						{ $$ = MinorBooleanGrammarAction($left, $right); }
	| mathexp[left] GREATER_THAN mathexp[right]							{ $$ = MajorBooleanGrammarAction($left, $right); }
	| mathexp[left] LESS_THAN_OR_EQUAL mathexp[right]						{ $$ = MinorEqualBooleanGrammarAction($left, $right); }
	| mathexp[left] GREATER_THAN_OR_EQUAL mathexp[right]						{ $$ = MajorEqualBooleanGrammarAction($left, $right); }
	| mathexp[left] EQUAL_EQUAL mathexp[right]							{ $$ =  EqualBooleanGrammarAction($left, $right); }
	| mathexp[left] NOT_EQUAL mathexp[right]						{ $$ = NotEqualBooleanGrammarAction($left, $right); }
	| gsboolean OR gsboolean												{ $$ = OrBooleanGrammarAction($1, $3); }
	| gsboolean AND gsboolean												{ $$ =  AndBooleanGrammarAction($1, $3); }
	| VARNAME IN VARNAME													{ $$ = InVarBooleanGrammarAction($1,$3); }
	| VARNAME IN gconstant 												{ $$ = InGCBooleanGrammarAction($1,$3); }
	| VARNAME                   				   { $$ = VarnameBooleanGrammarAction($1); }
	| VARNAME function_assignment 				   { $$ =  VarnameFuncBooleanGrammarAction($1,$2); }
	| VARNAME HITS VARNAME													{ $$ = HitsVarBooleanGrammarAction($1,$3); }
	| VARNAME HITS gconstant 												{ $$ =  HitsGCBooleanGrammarAction($1,$3); }
	| conditionals													{ $$ = ConditionalsBooleanGrammarAction($1); }
	;



mathexp: mathexp[left] SUB mathexp[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
		| mathexp[left] MUL mathexp[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
		| mathexp[left] DIV mathexp[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
		| factor														{ $$ = FactorExpressionGrammarAction($1); }
		| mathexp[left] ADD mathexp[right]						{ $$ = AdditionExpressionGrammarAction($left, $right); }
	;

factor: OPEN_PARENTHESIS mathexp CLOSE_PARENTHESIS				{ $$ = MathExpFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	| VARNAME														{ $$ = ConstantVarnameGrammarAction($1); }
	| SUB INTEGER 													{ $$ = ConstantSubstractIntegerGrammarAction($2); }
	| INTEGER PX 													{ $$ = IntegerPxConstantGrammarAction($1); }
	| QUOTE STRING_TEXT QUOTE 										{ $$ = StringConstantGrammarAction($2); }
	;

gconstant: UP_BORDER 												{ $$ = UpBorderConstantGrammarAction(); }
	| DOWN_BORDER													{ $$ = DownBorderConstantGrammarAction(); }
	| LEFT_BORDER													{ $$ = LeftBorderConstantGrammarAction(); }
	| RIGHT_BORDER													{ $$ = RightBorderConstantGrammarAction(); }
	| UP															{ $$ = UpConstantGrammarAction(); }
	| DOWN															{ $$ = DownConstantGrammarAction(); }
	| LEFT															{ $$ = LeftConstantGrammarAction(); }
	| RIGHT															{ $$ = RightConstantGrammarAction(); }
	| SPACE															{ $$ = SpaceConstantGrammarAction(); }
	| ENTER															{ $$ = EnterConstantGrammarAction(); }
	| NOKEY															{ $$ = NokeyConstantGrammarAction(); }
	;
%%
