#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../support/logger.h"
#include "generator.h"
#include "../semantic-analysis/symbol-table.h"


#define SOURCE_FILENAME "game.py"

FILE * source_file;

void Generator(Program program) {
	LogDebug("Generating program...");

	source_file = fopen(SOURCE_FILENAME,"w");
	
	if (source_file == NULL) {
		LogError("Could not open file %s", SOURCE_FILENAME);
		exit(1);
	}

	//Importamos librerias
	fprintf(source_file, "import pygame\n");
	fprintf(source_file, "import random\n");
	fprintf(source_file, "import sys\n");

	//Definimos constantes de pantalla
	fprintf(source_file, "WIDTH, HEIGHT = 800, 600\n");

	//Inicializamos pygame y seteamos la pantalla con su caption
	fprintf(source_file, "pygame.init()\n");
	fprintf(source_file, "screen = pygame.display.set_mode((WIDTH, HEIGHT))\n");
	fprintf(source_file, "pygame.display.set_caption(\"GameScript\")\n");

	//Seteamos el Bucle para que corra el juego indefinidamente
	fprintf(source_file, "clock = pygame.time.Clock()\n");
	fprintf(source_file, "running = True\n");
	fprintf(source_file, "while running:\n");
	fprintf(source_file, "\tpygame.display.flip()\n");
	fprintf(source_file, "\tclock.tick(60)\n");
    fprintf(source_file, "pygame.quit()\n");

	GenerateProgram(program);

	fclose(source_file);

	if (!state.game_generation) {
		return;
	}


	pid_t pid = fork();
	if (pid == 0) {
		char * args[] = {"/usr/bin/python3", SOURCE_FILENAME, NULL};
		execv(args[0], args);
	} else if (pid == -1) {
		LogError("Error al crear proceso para generar imágenes.");
	} else {
		int status;
		LogInfo("Generando videojuego");
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status)) {
			LogError("Error el videojuego");
		}
		else if (WEXITSTATUS(status) == 0) {
			LogInfo("Videojuego creado con exito!");
		}
	}

	LogDebug("Program generated.");

	return;
}

void GenerateProgram(Program program) {
	LogDebug("Generating program...");

	GenerateMainScene(program->mainscene);


	LogDebug("Program generated.");

	return;
}

void GenerateMainScene(Mainscene main_scene) {
	LogDebug("Generating main scene...");

	GenerateBody(main_scene->body);

	GenerateExpression(main_scene->expression);

	LogDebug("Main scene generated.");

	return;
}

void GenerateBody(Body body) {
	LogDebug("Generating body...");

	switch (body->bType) {
		case BODY_CONDITIONALS_BODY:
			LogDebug("Generating conditionals body...");
			GenerateConditionals(body->conditionals);
			GenerateBody(body->body);
			break;
		case BODY_THIS_ARRAY_ASSIGNMENT_RET_BODY:
			LogDebug("Generating this array assignment return body...");
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateReturnValue(body->returnvalue);
			GenerateBody(body->body);
			break;
		case BODY_THIS_ARRAY_ASSIGNMENT_FUNC_BODY:
			LogDebug("Generating this array assignment function body...");
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
		case BODY_FUNC_BODY:
			LogDebug("Generating function body...");
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
		case BODY_THIS_ARRAY_FUNC_BODY:
			LogDebug("Generating this array function body...");
			LogDebug("body->array: %p", body->array);
			GenerateArray(body->array);
			//hacemos logdebug del valor de bodu->functionvalue
			LogDebug("body->functionvalue: %p", body->functionvalue);
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
		case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY:
			LogDebug("Generating type varname array assignment constant body...");
			GenerateType(body->type);
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateConstant(body->constant);
			GenerateBody(body->body);
			break;
		case BODY_TYPE_VARNAME_ARRAY_BODY:
			LogDebug("Generating type varname array body...");
			GenerateType(body->type);
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateBody(body->body);
			break;
		case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_STRING_BODY:
			LogDebug("Generating type varname array assignment string body...");
			GenerateType(body->type);
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateString(body->str);
			GenerateBody(body->body);
			break;
		case BODY_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY:
			LogDebug("Generating varname array assignment constant body...");
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateConstant(body->constant);
			GenerateBody(body->body);
			break;
		case BODY_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY:
			LogDebug("Generating varname array assignment function body...");
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
		case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY:
			LogDebug("Generating type varname array assignment function body...");
			GenerateType(body->type);
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateAssignment(body->assignment);
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
		case BODY_VARNAME_ARRAY_VARSINGLE_BODY:
			LogDebug("Generating varname array varsingle body...");
			GenerateVarname(body->var);
			GenerateArray(body->array);
			GenerateVarSingleAction(body->varsingleaction);
			GenerateBody(body->body);
			break;
		case BODY_RETURN_RET:
			LogDebug("Generating return return body...");
			GenerateReturnValue(body->returnvalue);
			break;
		case BODY_ONCLICK:
			LogDebug("Generating onclick body...");
			GenerateFunctionValue(body->functionvalue);
			GenerateBody(body->body);
			break;
	}

	LogDebug("Body generated.");

	return;
}

void GenerateExpression(Expression expression) {
	LogDebug("Generating expression...");

	switch (expression->type) {
		case EXP_FUN_EXP:
			GenerateFunction(expression->function);
			GenerateExpression(expression->expression);
			break;
		case EXP_CONST_EXP:
			GenerateConstante(expression->constante);
			GenerateExpression(expression->expression);
			break;
	}

	LogDebug("Expression generated.");

	return;
}

void GenerateFunction(Function function) {
	LogDebug("Generating Function...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	GenerateType(function->type);
	GenerateVarname(function->var);
	GenerateParametersDef(function->parameters_def);
	GenerateBody(function->body);

	LogDebug("Function generated.");

	return;
}

void GenerateConstante(Constante constante) {
	LogDebug("Generating Constante...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	GenerateValue(constante->value);

	LogDebug("Constante generated.");

	return;
}

void GenerateType(Type type) {
	LogDebug("Generating Type...");

	switch (type->tType) {
		case T_INT:
			fprintf(source_file, "int");
			break;
		case T_TEXT:
			fprintf(source_file, "text");
			break;
		case T_BOOL:
			fprintf(source_file, "bool");
			break;
		case T_CHAR:
			fprintf(source_file, "char");
			break;
		case T_RGB:
			fprintf(source_file, "rgb");
			break;
		case T_BLOCK:
			fprintf(source_file, "block");
			break;
		case T_GOBJECT:
			fprintf(source_file, "gobject");
			break;
		case T_SCENE:
			fprintf(source_file, "scene");
			break;
		case T_BUTTON:
			fprintf(source_file, "button");
			break;
		case T_STRING:
			fprintf(source_file, "string");
			break;
	}

	LogDebug("Type generated.");
}

void GenerateParametersDef(ParametersDef parameters_def) {
	LogDebug("Generating ParametersDef...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(parameters_def->pType) {
		case P_RETVAL:
			fprintf(source_file, "retval");
			break;
		case P_RETVAL_PARAMS:
			fprintf(source_file, "retval_params");
			break;
	}

	LogDebug("ParametersDef generated.");

	return;
}

void GenerateParameters(Parameters parameters) {
	LogDebug("Generating Parameters...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(parameters->pType) {
		case P_LAMBDA:
			fprintf(source_file, "lambda");
			break;
		case P_RETVAL:
			fprintf(source_file, "retval");
			break;
		case P_RETVAL_PARAMS:
			fprintf(source_file, "retval_params");
			break;
	}

	LogDebug("Parameters generated.");

	return;
}

void GenerateVarSingleAction(VarSingleAction varsingleaction) {
	LogDebug("Generating VarSingleAction...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(varsingleaction->type) {
		case S_INCREMENT:
			fprintf(source_file, "++");
			break;
		case S_DECREMENT:
			fprintf(source_file, "--");
			break;
	}

	LogDebug("VarSingleAction generated.");

	return;
}

void GenerateAssignment(Assignment assignment) {
	LogDebug("Generating Assignment...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(assignment->type) {
		case ASSIGNMENT_EQUAL:
			fprintf(source_file, "=");
			break;
		case ASSIGNMENT_PLUS_EQUAL:
			fprintf(source_file, "+=");
			break;
		case ASSIGNMENT_MINUS_EQUAL:
			fprintf(source_file, "-=");
			break;
		case ASSIGNMENT_MUL_EQUAL:
			fprintf(source_file, "*=");
			break;
		case ASSIGNMENT_SLASH_EQUAL:
			fprintf(source_file, "/=");
			break;
	}

	LogDebug("Assignment generated.");

	return;
}

void GenerateFunctionAssignment(FunctionAssignment functionAssignment) {
	LogDebug("Generating FunctionAssignment...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	GenerateParameters(functionAssignment->parameters);

	LogDebug("FunctionAssignment generated.");

	return;
}

void GenerateArray(Array array) {
	LogDebug("Generating Array...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(array->aType) {
		case ARRAY_VARNAME_ARRAY:
			GenerateVarname(array->var);
			GenerateArray(array->array);
			fprintf(source_file, "varname_array");
			break;
		case ARRAY_INTEGER_ARRAY:
			GenerateInteger(array->value);
			GenerateArray(array->array);
			fprintf(source_file, "integer_array");
			break;
		case ARRAY_ARRAY:
		    GenerateArray(array->array);
			fprintf(source_file, "array");
			break;
		case ARRAY_LAMBDA:
			LogDebug("Array Lambda not implemented.");
			break;
	}

	LogDebug("Array generated.");

	return;
}

void GenerateReturnValue(ReturnValue returnvalue) {
	LogDebug("Generating ReturnValue...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(returnvalue->rtType) {
		case RT_CONSTANT:
			GenerateConstant(returnvalue->constant);
			fprintf(source_file, "constant");
			break;
		case RT_THIS:
			fprintf(source_file, "this");
			break;
		case RT_CHAR_TEXT:
			GenerateChar(returnvalue->c);
			fprintf(source_file, "char_text");
			break;
		case RT_GCONSTANT:
			GenerateGConstant(returnvalue->gconstant);
			fprintf(source_file, "gconstant");
			break;
		case RT_THIS_VAR_ARRAY:
			fprintf(source_file, "this_var_array");
			GenerateArray(returnvalue->array);
			break;
		case RT_CONS_SUB_CONS:
			fprintf(source_file, "cons_sub_cons");
			GenerateConstant(returnvalue->leftconstant);
			GenerateConstant(returnvalue->rightconstant);
			break;
		case RT_CONS_ADD_CONS:
			fprintf(source_file, "cons_add_cons");
			GenerateConstant(returnvalue->leftconstant);
			GenerateConstant(returnvalue->rightconstant);
			break;
		case RT_CONS_MUL_CONS:
			fprintf(source_file, "cons_mul_cons");
			GenerateConstant(returnvalue->leftconstant);
			GenerateConstant(returnvalue->rightconstant);
			break;
		case RT_CONS_DIV_CONS:
			fprintf(source_file, "cons_div_cons");
			GenerateConstant(returnvalue->leftconstant);
			GenerateConstant(returnvalue->rightconstant);
			break;
	}

	LogDebug("ReturnValue generated.");

	return;
}

void GenerateValue(Value value) {
	LogDebug("Generating Value...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(value->vType) {
		case V_TYPE:
			GenerateType(value->type);
			fprintf(source_file, "type");
			break;
		case V_FUNV:
			GenerateFunctionValue(value->functionvalue);
			fprintf(source_file, "functionvalue");
			break;
	} 

	LogDebug("Value generated.");

	return;
}

void GenerateFunctionValue(Functionvalue functionvalue) {
	LogDebug("Generating Functionvalue...");
	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINT
	GenerateVarname(functionvalue->functionName);
	GenerateParameters(functionvalue->parameters);

	LogDebug("Functionvalue generated.");
}

void GenerateConditionals(Conditionals conditionals) {
	LogDebug("Generating Conditionals...");
	
	switch(conditionals->conditionalsType) {
		case COND_WHEN:
			GenerateNegation(conditionals->negation);
			GenerateBoolean(conditionals->boolean);
			GenerateBody(conditionals->firstBody);
			break;
		case COND_WHEN_ELSE:
			GenerateNegation(conditionals->negation);
			GenerateBoolean(conditionals->boolean);
			GenerateBody(conditionals->firstBody);
			GenerateBody(conditionals->secondBody);
			break;
		case COND_IF:
			GenerateNegation(conditionals->negation);
			GenerateBoolean(conditionals->boolean);
			GenerateBody(conditionals->firstBody);
			GenerateIfOptions(conditionals->ifoptions);
			break;
		case COND_FOR:
			GenerateVarname(conditionals->varName);
			GenerateInteger(conditionals->rightValue);
			GenerateNegation(conditionals->negation);
			GenerateBoolean(conditionals->boolean);
			GenerateForOptions(conditionals->foroptions);
			GenerateBody(conditionals->firstBody);
			break;
		case COND_WHILE:
			GenerateNegation(conditionals->negation);
			GenerateBoolean(conditionals->boolean);
			GenerateBody(conditionals->firstBody);
			break; 
	}

	LogDebug("Conditionals generated.");
}

//--------------------------------------------------------------

void GenerateNegation(Negation negation) {
	LogDebug("Generating Negation...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(negation->nType) {
		case N_NOT:
			fprintf(source_file, "not");
			break;
	}

	LogDebug("Negation generated.");

	return;
}

void GenerateForOptions(ForOptions foroptions) {
	LogDebug("Generating ForOptions...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(foroptions->forOptionsType) {
		case S_INCREMENT:
			fprintf(source_file, "increment");
			break;
		case S_DECREMENT:
			fprintf(source_file, "decrement");
			break;
	}

	LogDebug("ForOptions generated.");

	return;
}

void GenerateIfOptions(IfOptions ifoptions) {
	LogDebug("Generating IfOptions...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(ifoptions->ifOptionsType) {
		case IF_OPTIONS_ELSE:
			fprintf(source_file, "else");
			break;
		case IF_OPTIONS_ELIF:
			GenerateBoolean(ifoptions->boolean);
			fprintf(source_file, "else_if");
			break;
	}

	LogDebug("IfOptions generated.");

	return;
}

void GenerateGSBoolean(GSBoolean boolean){
	LogDebug("Generating Boolean...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(boolean->booltype) {
		case BOOL_LESS_THAN:
			GenerateMathExpression(boolean->leftMathExp);
			GenerateMathExpression(boolean->rightMathExp);
			fprintf(source_file, "less_than");
			break;
		case BOOL_GREATER_THAN:
			GenerateMathExpression(boolean->leftMathExp);
			GenerateMathExpression(boolean->rightMathExp);
			fprintf(source_file, "greater_than");
			break;
		case BOOL_LESS_THAN_EQ:
			GenerateMathExpression(boolean->leftMathExp);
			GenerateMathExpression(boolean->rightMathExp);
			fprintf(source_file, "less_than_eq");
			break;
		case BOOL_GREATER_THAN_EQ:
			GenerateMathExpression(boolean->leftMathExp);
			GenerateMathExpression(boolean->rightMathExp);
			fprintf(source_file, "greater_than_eq");
			break;
		case BOOL_EQ_EQ:
			GenerateBoolean(boolean->leftBoolean);
			GenerateBoolean(boolean->rightBoolean);
			fprintf(source_file, "EQ_EQ");
			break;
		case BOOL_NOT_EQ:
			GenerateBoolean(boolean->leftBoolean);
			GenerateBoolean(boolean->rightBoolean);
			fprintf(source_file, "NOT_EQ");
			break;
		case BOOL_OR:
			GenerateBoolean(boolean->leftBoolean);
			GenerateBoolean(boolean->rightBoolean);
			fprintf(source_file, "OR");
			break;
		case BOOL_AND:
			GenerateBoolean(boolean->leftBoolean);
			GenerateBoolean(boolean->rightBoolean);
			fprintf(source_file, "AND");
			break;
		case BOOL_V_IN_V:
			GenerateVarname(boolean->lefVar);
			GenerateVarname(boolean->rightVar);
			fprintf(source_file, "v IN v");
			break;
		case BOOL_V_IN_GC:
			GenerateVarname(boolean->lefVar);
			GenerateVarname(boolean->rightVar);
			fprintf(source_file, "v IN gc");
			break;
		case BOOL_VAR:
			GenerateVarname(boolean->var);
			fprintf(source_file, "var");
			break;
		case BOOL_FUNA:
			GenerateVarname(boolean->var);
			GenerateFunctionAssignment(boolean->functionassignment);
			fprintf(source_file, "fun assignment");
			break;
		case BOOL_V_HITS_V:
			GenerateVarname(boolean->lefVar);
			GenerateVarname(boolean->rightVar);
			fprintf(source_file, "v_hits_v");
			break;
		case BOOL_V_HITS_GC:
			GenerateVarname(boolean->lefVar);
			GenerateVarname(boolean->rightVar);
			fprintf(source_file, "v_hits_gc");
			break;
		case BOOL_CONDITIONALS:
			GenerateConditionals(boolean->conditionals);
			fprintf(source_file, "conditional");
			break;
		}

	LogDebug("Boolean generated.");

	return;

}

void GenerateMathExpression(Mathexp mathExpression) {
	LogDebug("Generating MathExpression...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(mathExpression->mathExpType) {
		case MATH_EXP_ADD:
			GenerateMathExpression(mathExpression->leftMathExp);
			GenerateMathExpression(mathExpression->rightMathExp);
			fprintf(source_file, "mathExpression_add");
			break;
		case MATH_EXP_SUB:
			GenerateMathExpression(mathExpression->leftMathExp);
			GenerateMathExpression(mathExpression->rightMathExp);
			fprintf(source_file, "mathExpression_sub");
			break;
		case MATH_EXP_MUL:
			GenerateMathExpression(mathExpression->leftMathExp);
			GenerateMathExpression(mathExpression->rightMathExp);
			fprintf(source_file, "mathExpression_mul");
			break;
		case MATH_EXP_DIV:
			GenerateMathExpression(mathExpression->leftMathExp);
			GenerateMathExpression(mathExpression->rightMathExp);
			fprintf(source_file, "mathExpression_div");
			break;
		case MATH_EXP_FACTOR:
			GenerateFactor(mathExpression->factor);
			fprintf(source_file, "factor");
			break;
	}
	
	LogDebug("MathExpression generated.");

	return;
}

void GenerateFactor(Factor factor) {
	LogDebug("Generating Factor...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(factor->factorType) {
		case FACTOR_MATH_EXP:
			GenerateMathExpression(factor->mathexp);
			fprintf(source_file,"factor_math_exp");
			break;

		case FACTOR_CONSTANT:
			GenerateConstant(factor->constant);
			fprintf(source_file,"factor_constant");
			break;
	}

	LogDebug("Factor generated.");

	return;
}

void GenerateConstant(Constant constant) {
	LogDebug("Generating Constant...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(constant->cType) {
		case CONST_INT:
			GenerateInteger(constant->value);
			fprintf(source_file, "integer");
			break;
		case CONST_STR:
			GenerateString(constant->str);
			fprintf(source_file, "string");
			break;
		case CONST_VAR:
			GenerateVarname(constant->var);
			fprintf(source_file, "var");
			break;
		case CONST_SUB_INT:
			GenerateInteger(constant->value);
			fprintf(source_file, "sub_integer");
			break;
		case CONST_INT_PX:
			GenerateInteger(constant->value);
			fprintf(source_file, "integer_px");
			break;
	}

	LogDebug("Constant generated.");

	return;
}

void GenerateGConstant(GConstant gconstant) {
	LogDebug("Generating GConstant...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(gconstant->gcType) {
		case GC_UP_B:
			fprintf(source_file, "UP_BORDER");
			break;
		case GC_DOWN_B:
			fprintf(source_file, "DOWN_BORDER");
			break;
		case GC_LEFT_B:
			fprintf(source_file, "LEFT_BORDER");
			break;
		case GC_RIGHT_B:
			fprintf(source_file, "RIGHT_BORDER");
			break;
		case GC_UP:
			fprintf(source_file, "UP");
			break;
		case GC_DOWN:
			fprintf(source_file, "DOWN");
			break;
		case GC_LEFT:
			fprintf(source_file, "LEFT");
			break;
		case GC_RIGHT:
			fprintf(source_file, "RIGHT");
			break;
		case GC_SPACE:
			fprintf(source_file, "SPACE");
			break;
		case GC_ENTER:
			fprintf(source_file, "ENTER");
			break;
		case GC_NOKEY:
			fprintf(source_file, "NOKEY");
			break;
	}

	LogDebug("GConstant generated.");

	return;
}

void GenerateVarname(char * var) {
	LogDebug("Generating Varname...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	fprintf(source_file, "%s", var);

	LogDebug("Varname generated.");

	return;
}


void GenerateInteger(int value) {
	LogDebug("Generating Integer...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	fprintf(source_file, "%d", value);

	LogDebug("Integer generated.");

	return;
}	

void GenerateChar(char c) {
	LogDebug("Generating Char...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	fprintf(source_file, "%c", c);

	LogDebug("Char generated.");

	return;
}	

void GenerateString(char * str) {
	LogDebug("Generating String...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	fprintf(source_file, "%s", str);

	LogDebug("String generated.");

	return;
}

void GenerateBoolean(GSBoolean to_boolean) {
	LogDebug("Generating Boolean...");

	//TODO ACA HAY TERMINALES ASI QUE TENEMOS QUE HACER FPRINTF

	switch(to_boolean->booltype) {
		case BOOL_LESS_THAN:
			GenerateMathExpression(to_boolean->leftMathExp);
			GenerateMathExpression(to_boolean->rightMathExp);
			fprintf(source_file, "less_than");
			break;
		case BOOL_GREATER_THAN:
			GenerateMathExpression(to_boolean->leftMathExp);
			GenerateMathExpression(to_boolean->rightMathExp);
			fprintf(source_file, "greater_than");
			break;
		case BOOL_LESS_THAN_EQ:
			GenerateMathExpression(to_boolean->leftMathExp);
			GenerateMathExpression(to_boolean->rightMathExp);
			fprintf(source_file, "less_than_eq");
			break;
		case BOOL_GREATER_THAN_EQ:
			GenerateMathExpression(to_boolean->leftMathExp);
			GenerateMathExpression(to_boolean->rightMathExp);
			fprintf(source_file, "greater_than_eq");
			break;
		case BOOL_EQ_EQ:
			GenerateBoolean(to_boolean->leftBoolean);
			GenerateBoolean(to_boolean->rightBoolean);
			fprintf(source_file, "EQ_EQ");
			break;
		case BOOL_NOT_EQ:
			GenerateBoolean(to_boolean->leftBoolean);
			GenerateBoolean(to_boolean->rightBoolean);
			fprintf(source_file, "NOT_EQ");
			break;
		case BOOL_OR:
			GenerateBoolean(to_boolean->leftBoolean);
			GenerateBoolean(to_boolean->rightBoolean);
			fprintf(source_file, "OR");
			break;
		case BOOL_AND:
			GenerateBoolean(to_boolean->leftBoolean);
			GenerateBoolean(to_boolean->rightBoolean);
			fprintf(source_file, "AND");
			break;
		case BOOL_V_IN_V:
			GenerateVarname(to_boolean->lefVar);
			GenerateVarname(to_boolean->rightVar);
			fprintf(source_file, "v IN v");
			break;
		case BOOL_V_IN_GC:
			GenerateVarname(to_boolean->lefVar);
			GenerateVarname(to_boolean->rightVar);
			fprintf(source_file, "v IN gc");
			break;
		case BOOL_VAR:
			GenerateVarname(to_boolean->var);
			fprintf(source_file, "var");
			break;
		case BOOL_FUNA:
			GenerateVarname(to_boolean->var);
			GenerateFunctionAssignment(to_boolean->functionassignment);
			fprintf(source_file, "fun assignment");
			break;
		case BOOL_V_HITS_V:
			GenerateVarname(to_boolean->lefVar);
			GenerateVarname(to_boolean->rightVar);
			fprintf(source_file, "v_hits_v");
			break;
		case BOOL_V_HITS_GC:
			GenerateVarname(to_boolean->lefVar);
			GenerateVarname(to_boolean->rightVar);
			fprintf(source_file, "v_hits_gc");
			break;
		case BOOL_CONDITIONALS:
			GenerateConditionals(to_boolean->conditionals);
			fprintf(source_file, "conditional");
			break;
		}
}
