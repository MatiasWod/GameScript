#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include "../../backend/support/shared.h"
#include "../../backend/support/utils.h"

/**
 * Implementación de "bison-actions.h".
 */

/**
 * Esta función se ejecuta cada vez que se emite un error de sintaxis.
 */





void yyerror(const char *string)
{
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i)
	{
		switch (yytext[i])
		{
		case '\n':
			LogErrorRaw("\\n");
		default:
			LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
 * Esta acción se corresponde con el no-terminal que representa el símbolo
 * inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
 * indica que efectivamente el programa de entrada se pudo generar con esta
 * gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
 */

int ConditionalsGrammarAction()
{
	LogDebug("[Bison] ConditionalsGrammarAction(%d)", 0);
	return 0;
}

Program ProgramGrammarAction(Mainscene mainscene)
{
	LogDebug("[Bison] ProgramGrammarAction()");
	/*
	 * "state" es una variable global que almacena el estado del compilador,
	 * cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	 * es utilizada en la función "main".
	 */
	Program program = Malloc(sizeof(struct ProgramNode));
	program->mainscene = mainscene;
	/*
	 * Por otro lado, "result" contiene el resultado de aplicar el análisis
	 * sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	 * en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	 * la expresión se computa on-the-fly, y es la razón por la cual esta
	 * variable es un simple entero, en lugar de un nodo.
	 */
	state.succeed = state.error_count == 0;
	state.program = program;

	return program;
}


Mainscene MainSceneGrammarAction(Body body, Expression expression) {
	LogDebug("[Bison] MainSceneGrammarAction()");
	Mainscene mainscene = Malloc(sizeof(struct MainSceneNode));
	mainscene->body = body;
	mainscene->expression = expression;
	return mainscene;
}


Expression ExpressionEmptyGrammarAction()
{
	LogDebug("\tExpressionEmptyGrammarAction");

	Expression expression = Malloc(sizeof(struct ExpressionNode));
	expression->type = EXP_LAMBDA;

	return expression;
}

Expression ExpressionFunctionGrammarAction(Function function, Expression to_expression)
{
	LogDebug("\tExpressionFunctionGrammarAction");

	Expression expression = Malloc(sizeof(struct ExpressionNode));
	expression->type = EXP_FUN_EXP;
	expression->function = function;
	expression->expression = to_expression;

	return expression;
}

Expression ExpressionConstanteGrammarAction(Constante to_constante, Expression to_expression)
{
	LogDebug("\tExpressionConstanteGrammarAction");

	Expression expression = Malloc(sizeof(struct ExpressionNode));
	expression->type = EXP_CONST_EXP;
	expression->constante = to_constante;
	expression->expression = to_expression;

	return expression;
}

Constante ConstanteGrammarAction(Value value){
	LogDebug("\tConstanteGrammarAction");

	Constante constante = Malloc(sizeof(struct ConstanteNode));
	constante->value = value;

	return constante;
}


Function FunctionGrammarAction(Type to_type, char * fun_name, ParametersDef parameters_def, Body to_body){
	LogDebug("\tFunctionGrammarAction");

	
	Function function = Malloc(sizeof(struct FunctionNode));
	function->type = to_type;
	function->var = fun_name;
	function->parameters_def = parameters_def;
	function->body = to_body;

	tryInsertIntoSymbolTable(fun_name,to_type);

	return function;
}

Type IntTypeGrammarAction()
{
	LogDebug("\tIntTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_INT;

	return type;
}

Type TextTypeGrammarAction()
{
	LogDebug("\tTextTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_TEXT;

	return type;
}

Type BoolTypeGrammarAction()
{
	LogDebug("\tBoolTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_BOOL;

	return type;
}

Type CharTypeGrammarAction()
{
	LogDebug("\tCharTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_CHAR;

	return type;
}

Type RgbTypeGrammarAction()
{
	LogDebug("\tRgbTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_RGB;

	return type;
}

Type BlockTypeGrammarAction()
{
	LogDebug("\tBlockTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_BLOCK;

	return type;
}

Type GObjectTypeGrammarAction()
{
	LogDebug("\tGObjectTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_GOBJECT;

	return type;
}

Type SceneTypeGrammarAction()
{
	LogDebug("\tSceneTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_SCENE;

	return type;
}

Type ButtonTypeGrammarAction()
{
	LogDebug("\tButtonTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_BUTTON;

	return type;
}

Type StringTypeGrammarAction()
{
	LogDebug("\tStringTypeGrammarAction");

	Type type = Malloc(sizeof(struct TypeNode));
	type->tType = T_STRING;

	return type;
}

ParametersDef EmptyParametersDefGrammarAction(){
	LogDebug("\tEmptyParametersGrammarAction");

	ParametersDef parameters_def = Malloc(sizeof(struct ParametersDefNode));
	parameters_def->pType = PD_LAMBDA;

	return parameters_def;
}

ParametersDef ParametersDefGrammarAction(Type type , char* varname){
	LogDebug("\tParametersGrammarAction");

	ParametersDef parameters_def = Malloc(sizeof(struct ParametersDefNode));
	parameters_def->pType = PD_TYPE_VAR;
	parameters_def->type = type;
	parameters_def->var = varname;

	tryInsertIntoSymbolTable(varname,type);

	return parameters_def;
}

ParametersDef ParametersDefCommaGrammarAction(Type type , char* varname, ParametersDef to_parameters_def){
	LogDebug("\tParametersCommaGrammarAction");

	ParametersDef parameters_def = Malloc(sizeof(struct ParametersDefNode));
	parameters_def->pType = PD_TYPE_VAR_COMMA_PD;
	parameters_def->type = type;
	parameters_def->var = varname;
	parameters_def->parametersdef = to_parameters_def;

	tryInsertIntoSymbolTable(varname,type);

	return parameters_def;
} 

Parameters EmptyParametersGrammarAction(){
	LogDebug("\tEmptyParametersGrammarAction");

	Parameters parameters = Malloc(sizeof(struct ParametersNode));
	parameters->pType = P_LAMBDA;

	return parameters;
}

Parameters ParametersGrammarAction(ReturnValue return_value){
	LogDebug("\tParametersGrammarAction");

	Parameters parameters = Malloc(sizeof(struct ParametersNode));
	parameters->pType = P_RETVAL;
	parameters->returnvalue = return_value;

	return parameters;
}

Parameters ParametersCommaGrammarAction(ReturnValue return_value, Parameters to_parameters){
	LogDebug("\tParametersCommaGrammarAction");

	Parameters parameters = Malloc(sizeof(struct ParametersNode));
	parameters->pType = P_RETVAL_PARAMS;
	parameters->returnvalue = return_value;
	parameters->parameters = to_parameters;

	return parameters;
}

Body BodyEmptyGrammarAction(){
	LogDebug("\tBodyEmptyGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_LAMBDA;

	return body;
}

Body BodyConditionalsGrammarAction(Conditionals conditionals, Body to_body){
	LogDebug("\tBodyConditionlsGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_CONDITIONALS_BODY;
	body->conditionals = conditionals;
	body->body = to_body;

	return body;
}

Body BodyThisAssignRetGrammarAction(Array array, Assignment assignment, ReturnValue returnValue, Body to_body){
	LogDebug("\tBodyThisArrayAssignmentRetGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_THIS_ARRAY_ASSIGNMENT_RET_BODY;
	body->array = array;
	body->assignment = assignment;
	body->returnvalue = returnValue;
	body->body = to_body;

	return body;
}

Body BodyThisArrayAssignFuncGrammarAction(Array array, Assignment assignment, Functionvalue functionvalue, Body to_body){
	LogDebug("\tBodyThisArrayAssignmentFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_THIS_ARRAY_ASSIGNMENT_FUNC_BODY;
	body->array = array;
	body->assignment = assignment;
	body->functionvalue = functionvalue;
	body->body = to_body;

	return body;
}

Body BodyFuncGrammarAction(Functionvalue functionvalue, Body to_body){
	LogDebug("\tBodyThisArrayAssignmentFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_FUNC_BODY;
	body->functionvalue = functionvalue;
	body->body = to_body;

	return body;
}

Body BodyThisArrayFuncGrammarAction(Array array,Functionvalue functionvalue ,Body to_body){
	LogDebug("\tBodyFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_THIS_ARRAY_FUNC_BODY;
	body->functionvalue = functionvalue;
	body->body = to_body;


	return body;
}

Body BodyTypeAssignConstGrammarAction(Type type, char * varname, Array array, Assignment assignment, Constant constant, Body to_body) {
	LogDebug("\tBodyThisArrayFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY;
	body->array = array;
	body->body = to_body;
	body->type = type;
	body->var = varname;
	body->assignment = assignment;
	body->constant = constant;

	tryInsertIntoSymbolTable(varname,type);

	return body;
}

Body BodyTypeArrayGrammarAction(Type type, char* varname, Array array, Body to_body){
	LogDebug("\tBodyTypeArrayGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_TYPE_VARNAME_ARRAY_BODY;
	body->type = type;
	body->var = varname;
	body->array = array;
	body->body = to_body;

	tryInsertIntoSymbolTable(varname,type);

	return body;
}

Body BodyTypeArrayAssignStringGrammarAction(Type type, char* varname, Array array, Assignment assignment, char * str ,Body to_body){
	LogDebug("\tBodyTypeVarnameArrayGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_STRING_BODY;
	body->type = type;
	body->var = varname;
	body->array = array;
	body->body = to_body;
	body->assignment = assignment;
	body->str = str;

	tryInsertIntoSymbolTable(varname,type);

	return body;
}

Body BodyVarArrayAssignConstGrammarAction(char* varname, Array array, Assignment assignment, Constant constant, Body to_body){
	LogDebug("\tBodyVarnameArrayAssignmentConstGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY;
	body->var = varname;
	body->array = array;
	body->assignment = assignment;
	body->constant = constant;
	body->body = to_body;

	return body;
}

Body BodyVarArrayAssignFuncGrammarAction(char* varname, Array array, Assignment assignment, Functionvalue functionvalue, Body to_body){
	LogDebug("\tBodyVarnameArrayAssignmentFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY;
	body->var = varname;
	body->array = array;
	body->assignment = assignment;
	body->functionvalue = functionvalue;
	body->body = to_body;

	return body;
}

Body BodyTypeVarArrayAssignFuncGrammarAction(Type type, char* varname, Array array, Assignment assignment, Functionvalue functionvalue, Body to_body){
	LogDebug("\tBodyTypeVarnameArrayAssignmentFuncGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY;
	body->type = type;
	body->var = varname;
	body->array = array;
	body->assignment = assignment;
	body->functionvalue = functionvalue;
	body->body = to_body;

	tryInsertIntoSymbolTable(varname,type);

	return body;
}

Body BodyVarArraySingleGrammarAction(char* varname, Array array, VarSingleAction varsingleaction, Body to_body){
	LogDebug("\tBodyVarnameArrayVarsingleGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_VARNAME_ARRAY_VARSINGLE_BODY;
	body->var = varname;
	body->array = array;
	body->varsingleaction = varsingleaction;
	body->body = to_body;

	return body;
}

Body BodyRetGrammarAction(ReturnValue returnvalue){
	LogDebug("\tBodyReturnRetGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_RETURN_RET;
	body->returnvalue = returnvalue;

	return body;
}

Body BodyOnClickGrammarAction(Functionvalue functionvalue, Body to_body){
	LogDebug("\tBodyOnClickGrammarAction");

	Body body = Malloc(sizeof(struct BodyNode));
	body->bType = BODY_ONCLICK;
	body->functionvalue = functionvalue;
	body->body = to_body;

	return body;
}


VarSingleAction IncrementVarSingleActionGrammarAction(){
	LogDebug("\tIncrementVarSingleActionGrammarAction");

	VarSingleAction varSingleAction = Malloc(sizeof(struct VarSingleActionNode));
	varSingleAction->type = S_INCREMENT;

	return varSingleAction;
} 

VarSingleAction DecrementVarSingleActionGrammarAction(){
	LogDebug("\tDecrementVarSingleActionGrammarAction");

	VarSingleAction varSingleAction = Malloc(sizeof(struct VarSingleActionNode));
	varSingleAction->type = S_DECREMENT;

	return varSingleAction;
}

Assignment EqualAssignmentGrammarAction(){
	LogDebug("\tEqualAssignmentGrammarAction");

	Assignment assignment = Malloc(sizeof(struct AssignmentNode));
	assignment->type = ASSIGNMENT_EQUAL;

	return assignment;
}

Assignment PlusEqualAssignmentGrammarAction(){
	LogDebug("\tPlusEqualAssignmentGrammarAction");

	Assignment assignment = Malloc(sizeof(struct AssignmentNode));
	assignment->type = ASSIGNMENT_PLUS_EQUAL;

	return assignment;
}

Assignment MinusEqualAssignmentGrammarAction(){
	LogDebug("\tMinusEqualAssignmentGrammarAction");

	Assignment assignment = Malloc(sizeof(struct AssignmentNode));
	assignment->type = ASSIGNMENT_MINUS_EQUAL;

	return assignment;
}

Assignment SlashEqualAssignmentGrammarAction(){
	LogDebug("\tSlashEqualAssignmentGrammarAction");

	Assignment assignment = Malloc(sizeof(struct AssignmentNode));
	assignment->type = ASSIGNMENT_SLASH_EQUAL;

	return assignment;
}

Assignment MulEqualAssignmentGrammarAction(){
	LogDebug("\tMulEqualAssignmentGrammarAction");

	Assignment assignment = Malloc(sizeof(struct AssignmentNode));
	assignment->type = ASSIGNMENT_MUL_EQUAL;

	return assignment;
}

FunctionAssignment FunctionAssignmentGrammarAction(Parameters to_parameters){
	LogDebug("\tFunctionAssignmentGrammarAction");

	FunctionAssignment function_assignment = Malloc(sizeof(struct FunctionAssignmentNode));
	function_assignment->parameters = to_parameters;

	return function_assignment;
}

Array EmptyArrayGrammarAction(){
	LogDebug("\tEmptyArrayGrammarAction");

	Array array = Malloc(sizeof(struct ArrayNode));
	array->aType = ARRAY_LAMBDA;
	
	return array;
}

Array VarnameArrayGrammarAction(char * arrayName, Array to_array){
	LogDebug("\tVarnameArrayGrammarAction");

	Array array = Malloc(sizeof(struct ArrayNode));
	array->aType = ARRAY_VARNAME_ARRAY;
	array->var = arrayName;
	array->array = to_array;

	return array;
}

Array IntegerArrayGrammarAction(int arrayValue, Array to_array){
	LogDebug("\tIntegerArrayGrammarAction");

	Array array = Malloc(sizeof(struct ArrayNode));
	array->aType = ARRAY_INTEGER_ARRAY;
	array->value = arrayValue;
	array->array = to_array;

	return array;
}

Array ArrayArrayGrammarAction(Array to_array){
	LogDebug("\tArrayArrayGrammarAction");

	Array array = Malloc(sizeof(struct ArrayNode));
	array->aType = ARRAY_ARRAY;
	array->array = to_array;

	return array;
}

ReturnValue ReturnValueGrammarAction(Constant constant){
	LogDebug("\tReturnValueGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CONSTANT;
	return_value->constant = constant;

	return return_value;
}

ReturnValue ReturnValueThisGrammarAction(){
	LogDebug("\tReturnValueThisGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_THIS;

	return return_value;
}

ReturnValue ReturnValueCharTextGrammarAction(){
	LogDebug("\tReturnValueCharTextGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CHAR_TEXT;

	return return_value;
}

ReturnValue  ReturnValueGconstantGrammarAction(GConstant getconstant){
	LogDebug("\tReturnValueGconstantGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_GCONSTANT;
	return_value->gconstant = getconstant;

	return return_value;
}


ReturnValue ReturnValueThisVarnameArrayGrammarAction(char* varname, Array array){
	LogDebug("\tReturnValueThisVarnameArrayGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_THIS_VAR_ARRAY;
	return_value->var = varname;
	return_value->array = array;

	return return_value;
}

ReturnValue ReturnValueThisFunctionvalueGrammarAction(Functionvalue functionvalue){
	LogDebug("\tReturnValueThisFunctionvalueGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_THIS_FUNC;
	return_value->functionvalue = functionvalue;

	return return_value;
}

ReturnValue  ReturnValueConstantSubGrammarAction(Constant constantLeft,Constant constantRight){
	LogDebug("\tReturnValueConstantSubGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CONS_SUB_CONS;
	return_value->leftconstant = constantLeft;
	return_value->rightconstant = constantRight;

	return return_value;
} 

ReturnValue  ReturnValueConstantAddGrammarAction(Constant constantLeft,Constant constantRight){
	LogDebug("\tReturnValueConstantAddGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CONS_ADD_CONS;
	return_value->leftconstant = constantLeft;
	return_value->rightconstant = constantRight;

	return return_value;
}

ReturnValue ReturnValueConstantMulGrammarAction(Constant constantLeft,Constant constantRight){
	LogDebug("\tReturnValueConstantMulGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CONS_MUL_CONS;
	return_value->leftconstant = constantLeft;
	return_value->rightconstant = constantRight;

	return return_value;
}

ReturnValue ReturnValueConstantDivGrammarAction(Constant constantLeft,Constant constantRight){
	LogDebug("\tReturnValueConstantDivGrammarAction");

	ReturnValue return_value = Malloc(sizeof(struct ReturnValueNode));
	return_value->rtType = RT_CONS_DIV_CONS;
	return_value->leftconstant = constantLeft;
	return_value->rightconstant = constantRight;

	return return_value;
}

Value TypeValueGrammarAction(Type type){
	LogDebug("\tTypeValueGrammarAction");

	Value value = Malloc(sizeof(struct ValueNode));
	value->vType = V_TYPE;
	value->type = type;

	return value;
}

Value FunctionValueValueGrammarAction(Functionvalue to_functionValue){
	LogDebug("\tFunctionValueValueGrammarAction");

	Value value = Malloc(sizeof(struct ValueNode));
	value->vType = V_FUNV;
	value->functionvalue = to_functionValue;

	return value;
}

Functionvalue FunctionValueGrammarAction(char* varname, Parameters parameters){
	LogDebug("\tFunctionValueGrammarAction");

	Functionvalue functionvalue = Malloc(sizeof(struct FunctionValueNode));
	functionvalue->functionName = varname;
	functionvalue->parameters = parameters;

	return functionvalue;
}

Conditionals WhenBodyGrammarAction(GSBoolean to_boolean, Body to_body){
	LogDebug("\tWhenBodyGrammarAction");
	
	Conditionals conditionals = Malloc(sizeof(struct ConditionalsNode));
	conditionals->conditionalsType = COND_WHEN;
	conditionals->boolean = to_boolean;
	conditionals->firstBody = to_body;

	return conditionals;
}

Conditionals WhenElseBodyGrammarAction(GSBoolean to_boolean, Body to_firstBody, Body to_secondBody){
	LogDebug("\tWhenElseBodyGrammarAction");
	
	Conditionals conditionals = Malloc(sizeof(struct ConditionalsNode));
	conditionals->conditionalsType = COND_WHEN_ELSE;
	conditionals->boolean = to_boolean;
	conditionals->firstBody = to_firstBody;
	conditionals->secondBody = to_secondBody;

	return conditionals;
}

Conditionals IfBodyGrammarAction(Negation to_negation, GSBoolean to_boolean, Body to_firstBody, IfOptions to_ifOptions){
	LogDebug("\tIfBodyGrammarAction");
	
	Conditionals conditionals = Malloc(sizeof(struct ConditionalsNode));
	conditionals->conditionalsType = COND_IF;
	conditionals->negation = to_negation;
	conditionals->boolean = to_boolean;
	conditionals->firstBody = to_firstBody;
	conditionals->ifoptions = to_ifOptions;

	return conditionals;
}

Conditionals ForBodyGrammarAction(char * to_varName, int to_rightValue,Negation to_negation, GSBoolean to_boolean, ForOptions to_forOptions,Body to_firstBody){
	LogDebug("\tForBodyGrammarAction");
	
	Conditionals conditionals = Malloc(sizeof(struct ConditionalsNode));
	conditionals->conditionalsType = COND_FOR;
	conditionals->negation = to_negation;
	conditionals->boolean = to_boolean;
	conditionals->firstBody = to_firstBody;
	conditionals->foroptions = to_forOptions;
	conditionals->varName = to_varName;
	conditionals->rightValue = to_rightValue;

	return conditionals;
}

Conditionals WhileBodyGrammarAction(Negation to_negation, GSBoolean to_boolean, Body to_firstBody){
	LogDebug("\tWhileBodyGrammarAction");
	
	Conditionals conditionals = Malloc(sizeof(struct ConditionalsNode));
	conditionals->conditionalsType = COND_WHILE;
	conditionals->negation = to_negation;
	conditionals->boolean = to_boolean;
	conditionals->firstBody = to_firstBody;

	return conditionals;
}

Negation EmptyNegationGrammarAction(){
	LogDebug("\tEmptyNegationGrammarAction");

	Negation negation = Malloc(sizeof(struct NegationNode));
	negation->nType = N_LAMBDA;

	return negation;
}

Negation NegationGrammarAction(){
	LogDebug("\tNegationGrammarAction");

	Negation negation = Malloc(sizeof(struct NegationNode));
	negation->nType = N_NOT;

	return negation;
}

ForOptions EmptyForOptionsGrammarAction(){
	LogDebug("\tEmptyForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_LAMBDA;

	return for_options;
}

ForOptions IncForOptionsGrammarAction(char * varname){
	LogDebug("\tIncForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_INCREMENT;
	for_options->var = varname;

	return for_options;
}

ForOptions DecForOptionsGrammarAction(char * varname){
	LogDebug("\tDecForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_DECREMENT;
	for_options->var = varname;

	return for_options;
}

ForOptions PlusEqualForOptionsGrammarAction(char * varname, int interger){
	LogDebug("\tPlusEqualForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_PLUS_EQUAL;
	for_options->var = varname;
	for_options->value = interger;

	return for_options;
}

ForOptions MinusEqualForOptionsGrammarAction(char * varname, int interger){
	LogDebug("\tMinusEqualForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_MINUS_EQUAL;
	for_options->var = varname;
	for_options->value = interger;

	return for_options;
}

ForOptions SlashEqualForOptionsGrammarAction(char * varname, int interger){
	LogDebug("\tSlashEqualForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_SLASH_EQUAL;
	for_options->var = varname;
	for_options->value = interger;

	return for_options;
}

ForOptions MulEqualForOptionsGrammarAction(char * varname, int interger){
	LogDebug("\tMulEqualForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_MUL_EQUAL;
	for_options->var = varname;
	for_options->value = interger;

	return for_options;
}

ForOptions EqualForOptionsGrammarAction(char * varname, Mathexp mathexp){
	LogDebug("\tEqualForOptionsGrammarAction");

	ForOptions for_options = Malloc(sizeof(struct ForOptionsNode));
	for_options->forOptionsType = FOR_EQUAL;
	for_options->var = varname;
	for_options->mathexp = mathexp;

	return for_options;
}

IfOptions EmptyIfOptionsGrammarAction(){
	LogDebug("\tEmptyIfOptionsGrammarAction");

	IfOptions if_options = Malloc(sizeof(struct IfOptionsNode));
	if_options->ifOptionsType = IF_OPTIONS_LAMBDA;

	return if_options;
}

IfOptions ElifBodyGrammarAction(GSBoolean boolean, Body body, IfOptions to_if_options){
	LogDebug("\tElifBodyGrammarAction");

	IfOptions if_options = Malloc(sizeof(struct IfOptionsNode));
	if_options->ifOptionsType = IF_OPTIONS_ELIF;
	if_options->boolean = boolean;
	if_options->body = body;
	if_options->ifOptions = to_if_options;

	return if_options;
}

IfOptions  ElseBodyGrammarAction(Body body){
	LogDebug("\tElseBodyGrammarAction");

	IfOptions if_options = Malloc(sizeof(struct IfOptionsNode));
	if_options->ifOptionsType = IF_OPTIONS_ELSE;
	if_options->body = body;

	return if_options;
}


GSBoolean MinorBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] MinorBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_LESS_THAN;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}

GSBoolean MajorBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] MajorBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_GREATER_THAN;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}

GSBoolean MinorEqualBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] MinorEqualBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_LESS_THAN_EQ;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}

GSBoolean MajorEqualBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] MajorEqualBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_GREATER_THAN_EQ;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}

GSBoolean EqualBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] EqualBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_EQ_EQ;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}


GSBoolean NotEqualBooleanGrammarAction(Mathexp leftmathexp, Mathexp rightmathexp) {
	LogDebug("[Bison] NotEqualBooleanGrammarAction(%d, %d)", leftmathexp, rightmathexp);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_NOT_EQ;
	boolean->leftMathExp = leftmathexp;
	boolean->rightMathExp = rightmathexp;
	return boolean;
}

GSBoolean OrBooleanGrammarAction(GSBoolean leftboolean, GSBoolean rightboolean) {
	LogDebug("[Bison] OrBooleanGrammarAction(%d, %d)", leftboolean, rightboolean);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_OR;
	boolean->leftBoolean = leftboolean;
	boolean->rightBoolean = rightboolean;
	return boolean;
}

GSBoolean AndBooleanGrammarAction(GSBoolean leftboolean, GSBoolean rightboolean) {
	LogDebug("[Bison] AndBooleanGrammarAction(%d, %d)", leftboolean, rightboolean);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_AND;
	boolean->leftBoolean = leftboolean;
	boolean->rightBoolean = rightboolean;
	return boolean;
}

GSBoolean InVarBooleanGrammarAction(char * varname, char * varname2) {
	LogDebug("[Bison] InVarBooleanGrammarAction(%d, %d)", varname, varname2);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_V_IN_V;
	boolean->lefVar = varname;
	boolean->rightVar = varname2;
	return boolean;
}

GSBoolean InGCBooleanGrammarAction(char * varname, GConstant gconstant) {
	LogDebug("[Bison] InGCBooleanGrammarAction(%d, %d)", varname, gconstant);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_V_IN_GC;
	boolean->lefVar = varname;
	boolean->gconstant = gconstant;
	return boolean;
}

GSBoolean VarnameBooleanGrammarAction(char * var) {
	LogDebug("[Bison] VarnameBooleanGrammarAction(%d)", var);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_VAR;
	boolean->lefVar = var;
	return boolean;
}

GSBoolean VarnameFuncBooleanGrammarAction(char * var, FunctionAssignment functionAssignment) {
	LogDebug("[Bison] VarnameFuncBooleanGrammarAction(%d, %d)", var, functionAssignment);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_FUNA;
	boolean->lefVar = var;
	boolean->functionassignment = functionAssignment;
	return boolean;
}

GSBoolean HitsVarBooleanGrammarAction(char * varname, char * varname2) {
	LogDebug("[Bison] HitsVarBooleanGrammarAction(%d, %d)", varname, varname2);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_V_HITS_V;
	boolean->lefVar = varname;
	boolean->rightVar = varname2;
	return boolean;
}

GSBoolean HitsGCBooleanGrammarAction(char * var, GConstant gconstant) {
	LogDebug("[Bison] HitsGCBooleanGrammarAction(%d, %d)", var, gconstant);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_V_HITS_GC;
	boolean->lefVar = var;
	boolean->gconstant = gconstant;
	return boolean;
}

GSBoolean ConditionalsBooleanGrammarAction(Conditionals conditionals) {
	LogDebug("[Bison] ConditionalsBooleanGrammarAction(%d)", conditionals);
	GSBoolean boolean = Malloc(sizeof(struct GSBooleanNode));
	boolean->booltype = BOOL_CONDITIONALS;
	boolean->conditionals = conditionals;
	return boolean;
}

Mathexp SubtractionExpressionGrammarAction(Mathexp leftMathexp, Mathexp rightMathexp) {
	LogDebug("[Bison] SubtractionExpressionGrammarAction(%d, %d)", leftMathexp, rightMathexp);
	Mathexp mathexp = Malloc(sizeof(struct MathexpNode));
	mathexp->mathExpType = MATH_EXP_SUB;
	mathexp->leftMathExp = leftMathexp;
	mathexp->rightMathExp = rightMathexp;
	return mathexp;
}

Mathexp MultiplicationExpressionGrammarAction(Mathexp leftMathexp, Mathexp rightMathexp) {
	LogDebug("[Bison] MultiplicationExpressionGrammarAction(%d, %d)", leftMathexp, rightMathexp);
	Mathexp mathexp = Malloc(sizeof(struct MathexpNode));
	mathexp->mathExpType = MATH_EXP_MUL;
	mathexp->leftMathExp = leftMathexp;
	mathexp->rightMathExp = rightMathexp;
	return mathexp;
}

Mathexp DivisionExpressionGrammarAction(Mathexp leftMathexp, Mathexp rightMathexp) {
	LogDebug("[Bison] DivisionExpressionGrammarAction(%d, %d)", leftMathexp, rightMathexp);
	Mathexp mathexp = Malloc(sizeof(struct MathexpNode));
	mathexp->mathExpType = MATH_EXP_DIV;
	mathexp->leftMathExp = leftMathexp;
	mathexp->rightMathExp = rightMathexp;
	return mathexp;
}

Mathexp FactorExpressionGrammarAction(Factor factor) {
	LogDebug("[Bison] FactorExpressionGrammarAction(%d)", factor);
	Mathexp mathexp = Malloc(sizeof(struct MathexpNode));
	mathexp->mathExpType = MATH_EXP_FACTOR;
	mathexp->factor = factor;
	return mathexp;
}

Mathexp AdditionExpressionGrammarAction(Mathexp leftMathexp, Mathexp rightMathexp) {
	LogDebug("[Bison] AdditionExpressionGrammarAction(%d, %d)", leftMathexp, rightMathexp);
	Mathexp mathexp = Malloc(sizeof(struct MathexpNode));
	mathexp->mathExpType = MATH_EXP_ADD;
	mathexp->leftMathExp = leftMathexp;
	mathexp->rightMathExp = rightMathexp;
	return mathexp;
}

Factor MathExpFactorGrammarAction(Mathexp mathexp) {
	LogDebug("[Bison] MathExpFactorGrammarAction(%d)", mathexp);
	Factor factor = Malloc(sizeof(struct FactorNode));
	factor->factorType = FACTOR_MATH_EXP;
	factor->mathexp = mathexp;
	return factor;
}

Factor ConstantFactorGrammarAction(Constant constant) {
	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", constant);
	Factor factor = Malloc(sizeof(struct FactorNode));
	factor->factorType = FACTOR_CONSTANT;
	factor->constant = constant;
	return factor;
}

Constant IntegerConstantGrammarAction(int value) {
	LogDebug("[Bison] IntegerConstantGrammarAction(%d)", value);
	Constant constant = Malloc(sizeof(struct ConstantNode));
	constant->cType = CONST_INT;
	constant->value = value;
	return constant;
}

Constant ConstantVarnameGrammarAction(char * varname) {
	LogDebug("[Bison] ConstantVarnameGrammarAction(%d)", varname);
	Constant constant = Malloc(sizeof(struct ConstantNode));
	constant->cType = CONST_VAR;
	constant->var = varname;
	return constant;
}

Constant ConstantSubstractIntegerGrammarAction(int value) {
	LogDebug("[Bison] ConstantSubstractIntegerGrammarAction(%d)", value);
	Constant constant = Malloc(sizeof(struct ConstantNode));
	constant->cType = CONST_SUB_INT;
	constant->value = value;
	return constant;
}

Constant IntegerPxConstantGrammarAction(int value) {
	LogDebug("[Bison] IntegerPxConstantGrammarAction(%d)", value);
	Constant constant = Malloc(sizeof(struct ConstantNode));
	constant->cType = CONST_INT_PX;
	constant->value = value;
	return constant;
}

Constant StringConstantGrammarAction(char * str) {
	LogDebug("[Bison] StringConstantGrammarAction(%d)", str);
	Constant constant = Malloc(sizeof(struct ConstantNode));
	constant->cType = CONST_STR;
	constant->str = str;
	return constant;
}

GConstant UpBorderConstantGrammarAction() {
	LogDebug("[Bison] UpBorderConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_UP_B;
	return gconstant;
}

GConstant DownBorderConstantGrammarAction() {
	LogDebug("[Bison] DownBorderConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_DOWN_B;
	return gconstant;
}

GConstant LeftBorderConstantGrammarAction() {
	LogDebug("[Bison] LeftBorderConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_LEFT_B;
	return gconstant;
}

GConstant RightBorderConstantGrammarAction() {
	LogDebug("[Bison] RightBorderConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_RIGHT_B;
	return gconstant;
}

GConstant UpConstantGrammarAction() {
	LogDebug("[Bison] UpConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_UP;
	return gconstant;
}

GConstant DownConstantGrammarAction() {
	LogDebug("[Bison] DownConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_DOWN;
	return gconstant;
}

GConstant LeftConstantGrammarAction() {
	LogDebug("[Bison] LeftConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_LEFT;
	return gconstant;
}

GConstant RightConstantGrammarAction() {
	LogDebug("[Bison] RightConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_RIGHT;
	return gconstant;
}

GConstant SpaceConstantGrammarAction() {
	LogDebug("[Bison] SpaceConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_SPACE;
	return gconstant;
}

GConstant EnterConstantGrammarAction() {
	LogDebug("[Bison] EnterConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_ENTER;
	return gconstant;
}

GConstant NokeyConstantGrammarAction() {
	LogDebug("[Bison] NokeyConstantGrammarAction()");
	GConstant gconstant = Malloc(sizeof(struct GConstantNode));
	gconstant->gcType = GC_NOKEY;
	return gconstant;
}