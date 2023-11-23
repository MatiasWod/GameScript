#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../support/logger.h"
#include "generator.h"
#include "../semantic-analysis/symbol-table.h"

#define SOURCE_FILENAME "game.py"

#define MAX_VARNAME 255
#define MAX_TYPE 7
#define MAX_PARAMS 255
#define MAX_BODY 255
#define MAX_TEXT 255
#define MAX_COND_TYPE 255

#define DEFAULT_POSITION_X 0
#define DEFAULT_POSITION_Y 0
#define INITIAL_SIZE 10

FILE *source_file;
boolean before_context = true;
boolean gobject_context = false;
boolean function_context = false;
boolean variable_context = false;
boolean conditional_context = false;

char *beforeCycle = NULL;
char *inCycle = NULL;
char **Scenes;
int scene_count = 0;
char **gobject_array = NULL;

int gobjects_count = 0;
int array_capacity = 0;

int block_size = 20;

unsigned int scope_current = 0;

boolean inCycle_context = false;

void printTabs(int tabs)
{
	for (int i = 0; i < tabs; i++)
	{
		appendString("\t");
	}
}

void appendString(const char *text)
{
	if (!inCycle_context)
	{
		if (beforeCycle == NULL)
		{
			beforeCycle = malloc(10 * sizeof(char)); // Asignación inicial de memoria si es nulo
			if (beforeCycle == NULL)
			{
				printf("Error: No se pudo asignar memoria para beforeCycle.\n");
				return;
			}
			beforeCycle[0] = '\0'; // Inicializar el string como vacío
		}

		size_t current_length = strlen(beforeCycle);
		size_t text_length = strlen(text);

		// Redimensionar el string para agregar el texto
		char *temp = realloc(beforeCycle, current_length + text_length + 1);
		if (temp != NULL)
		{
			beforeCycle = temp;						 // Asignar el nuevo espacio redimensionado
			strncat(beforeCycle, text, text_length); // Agregar texto al string
		}
		else
		{
			printf("Error: No se pudo redimensionar la memoria para beforeCycle.\n");
		}
	}
	else
	{
		if (inCycle == NULL)
		{
			inCycle = malloc(10 * sizeof(char)); // Asignación inicial de memoria si es nulo
			if (inCycle == NULL)
			{
				printf("Error: No se pudo asignar memoria para beforeCycle.\n");
				return;
			}
			inCycle[0] = '\0'; // Inicializar el string como vacío
		}

		size_t current_length = strlen(inCycle);
		size_t text_length = strlen(text);

		// Redimensionar el string para agregar el texto
		char *temp = realloc(inCycle, current_length + text_length + 1);
		if (temp != NULL)
		{
			inCycle = temp;						 // Asignar el nuevo espacio redimensionado
			strncat(inCycle, text, text_length); // Agregar texto al string
		}
		else
		{
			printf("Error: No se pudo redimensionar la memoria para inCycle.\n");
		}
	}
}

void fillMovementParams()
{
	// switch (body->functionvalue->parameters->returnvalue->gconstant->gcType)
	// {
	// case /* constant-expression */:
	// 	/* code */
	// 	break;

	// default:
	// 	break;
	// }
}

void Generator(Program program)
{
	LogDebug("Generating program...");

	source_file = fopen(SOURCE_FILENAME, "w");

	if (source_file == NULL)
	{
		LogError("Could not open file %s", SOURCE_FILENAME);
		exit(1);
	}

	// Importamos librerias
	fprintf(source_file, "import pygame\n");
	fprintf(source_file, "import random\n");
	fprintf(source_file, "import sys\n");

	// Definimos constantes de pantalla
	fprintf(source_file, "WIDTH, HEIGHT = 800, 600\n");

	// Inicializamos pygame y seteamos la pantalla con su caption
	fprintf(source_file, "pygame.init()\n");
	fprintf(source_file, "screen = pygame.display.set_mode((WIDTH, HEIGHT))\n");
	fprintf(source_file, "pygame.display.set_caption(\"GameScript\")\n");

	fprintf(source_file, "block_size = 20\n");

	fprintf(source_file, "clock = pygame.time.Clock()\n");
	fprintf(source_file, "running = True\n");

	//finish function
	fprintf(source_file, "def finish(text):\n");
	fprintf(source_file, "\tprint(text)\n");
	fprintf(source_file, "\tglobal running\n");
	fprintf(source_file, "\trunning = False\n");

	// Seteamos la funcion when_in_
	fprintf(source_file, "def when_in_string(leftGobject,rightGobject):\n");
	fprintf(source_file, "\treturn leftGobject.pos[0] < rightGobject.pos[0] + rightGobject.width and leftGobject.pos[0] + leftGobject.width > rightGobject.pos[0] and leftGobject.pos[1] < rightGobject.pos[1] + rightGobject.height and leftGobject.pos[1] + leftGobject.height > rightGobject.pos[1]\n");

	fprintf(source_file, "def bounceX(gobject):\n");
	fprintf(source_file, "\tgobject.speedx = -gobject.speedx\n");
	fprintf(source_file, "\tgobject.pos[0] += gobject.speedx\n");
	fprintf(source_file, "\tgobject.pos[1] += gobject.speedy\n");

	fprintf(source_file, "def bounceY(gobject):\n");
	fprintf(source_file, "\tgobject.speedy = -gobject.speedy\n");
	fprintf(source_file, "\tgobject.pos[0] += gobject.speedx\n");
	fprintf(source_file, "\tgobject.pos[1] += gobject.speedy\n");

	fprintf(source_file, "def stop(gobject):\n");
	fprintf(source_file, "\tgobject.speedx = 0\n");
	fprintf(source_file, "\tgobject.speedy = 0\n");

	fprintf(source_file, "def when_in_RIGHT_BORDER_string(gobject):\n");
	fprintf(source_file, "\treturn gobject.pos[0] > WIDTH\n");

	fprintf(source_file, "def when_in_LEFT_BORDER_string(gobject):\n");
	fprintf(source_file, "\treturn gobject.pos[0] < 0\n");

	fprintf(source_file, "def when_in_UP_BORDER_string(gobject):\n");
	fprintf(source_file, "\treturn gobject.pos[1] < 0\n");

	fprintf(source_file, "def when_in_DOWN_BORDER_string(gobject):\n");
	fprintf(source_file, "\treturn gobject.pos[1] > HEIGHT\n");

	GenerateProgram(program);

	fclose(source_file);

	if (!state.game_generation)
	{
		return;
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		char *args[] = {"/usr/bin/python3", SOURCE_FILENAME, NULL};
		execv(args[0], args);
	}
	else if (pid == -1)
	{
		LogError("Error al crear proceso para generar imágenes.");
	}
	else
	{
		int status;
		LogInfo("Generando videojuego");
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			LogError("Error el videojuego");
		}
		else if (WEXITSTATUS(status) == 0)
		{
			LogInfo("Videojuego creado con exito!");
		}
	}

	LogDebug("Program generated.");

	return;
}

void GenerateProgram(Program program)
{
	LogDebug("Generating program...");

	GenerateMainScene(program->mainscene);
	if (beforeCycle != NULL)
	{
		fprintf(source_file, "%s", beforeCycle);
		free(beforeCycle);
	}

	/*def draw():
	screen.fill((0, 0, 0))*/
	fprintf(source_file, "def draw():\n");
	fprintf(source_file, "\tscreen.fill((0, 0, 0))\n");

	for (int i = 0; i < gobjects_count; i++)
	{
		fprintf(source_file, "\tpygame.draw.rect(screen, (255, 255, 255), (%s.pos[0], %s.pos[1], %s.width, %s.height))\n", gobject_array[i], gobject_array[i], gobject_array[i], gobject_array[i]);
	}

	fprintf(source_file, "\tpygame.display.update()\n");

	// fprintf(source_file,beforeCycle);

	// Seteamos el Bucle para que corra el juego indefinidamente
	fprintf(source_file, "while running:\n");
	fprintf(source_file, "\tfor event in pygame.event.get():\n");
	fprintf(source_file, "\t\tif event.type == pygame.QUIT:\n");
	fprintf(source_file, "\t\t\trunning = False\n");

	fprintf(source_file, inCycle);
	free(inCycle);

	fprintf(source_file, "\tdraw()\n");
	for (int i = scene_count; i > 0; i--)
	{
		// fprintf(source_file, Scenes[i]);
	}

	// fprintf(source_file, Scenes[0]);

	fprintf(source_file, "\tpygame.display.flip()\n");
	fprintf(source_file, "\tclock.tick(60)\n");
	fprintf(source_file, "pygame.quit()\n");
	LogDebug("Program generated.");

	return;
}

void GenerateMainScene(Mainscene main_scene)
{
	LogDebug("Generating main scene...");

	GenerateExpression(main_scene->expression);

	GenerateBody(main_scene->body);

	return;
}

void GenerateBody(Body body)
{
	LogDebug("Generating body...");

	printTabs(scope_current);
	switch (body->bType)
	{
	case BODY_CONDITIONALS_BODY:
		LogDebug("Generating conditionals body...");
		conditional_context = true;
		GenerateConditionals(body->conditionals);
		conditional_context = false;
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
		GenerateBody(body->body);
		break;
	case BODY_FUNC_BODY:
		LogDebug("Generating functionvalue body...");
		char str[100];
		const char *delimeter = ".";
		char *dotPosition = strchr(body->functionvalue->functionName, delimeter[0]);
		if (dotPosition != NULL)
		{
			size_t length = dotPosition - body->functionvalue->functionName; // Calcular la longitud de la porción antes del punto
			char subString[length + 1];										 // Crear un array para almacenar la porción antes del punto

			// Copiar la porción antes del punto a subString
			strncpy(subString, body->functionvalue->functionName, length);
			subString[length] = '\0'; // Agregar el carácter nulo al final para formar una cadena válida
			LogDebug("body->array->var: %s", body->functionvalue->functionName);
			if (strstr(body->functionvalue->functionName, "setSpeed") != NULL)
			{
				LogDebug("%d", body->functionvalue->parameters->returnvalue->constant->value);
				appendString("\n");
				printTabs(scope_current);
				sprintf(str, "%sSpeed = %d\n", subString, body->functionvalue->parameters->returnvalue->constant->value);
				appendString(str);
				LogDebug("DESPUES");
			}
			if (strstr(body->functionvalue->functionName, "setMovement") != NULL)
			{
				inCycle_context = true;
				scope_current++;
				appendString("\n");
				printTabs(scope_current);
				sprintf(str, "%s.assignKeys(%sSpeed ,", subString, subString);
				appendString(str);
				GenerateFunctionValue(body->functionvalue);
				appendString(")\n");
				printTabs(scope_current);
				appendString(subString);
				appendString(".updateMovement()\n");
				scope_current--;
				inCycle_context = false;
			}
			if (strstr(body->functionvalue->functionName, "setPosition") != NULL)
			{	
				if(inCycle_context){
					printTabs(scope_current);
				}
				sprintf(str, "%s.pos = [", subString);
				appendString(str);
				GenerateFunctionValue(body->functionvalue);
				appendString("]\n");
			}

			if (strstr(body->functionvalue->functionName, "randomMovement") != NULL)
			{
				sprintf(str, "\n%s.randomMovement(%sSpeed)\n", subString, subString);
				appendString(str);
				inCycle_context = true;
				scope_current++;
				printTabs(scope_current);
				sprintf(str, "%s.updateMovement()\n", subString);
				appendString(str);
				scope_current--;
				inCycle_context = false;
			}
			if (strstr(body->functionvalue->functionName, "bounceX") != NULL)
			{	
				printTabs(scope_current);
				LogDebug("ESTO ES ACA 1: skere");
				sprintf(str, "bounceX(%s)\n", subString);
				appendString(str);
			}
			if (strstr(body->functionvalue->functionName, "bounceY") != NULL)
			{
				printTabs(scope_current);
				sprintf(str, "bounceY(%s)\n", subString);
				appendString(str);
			}
			if (strstr(body->functionvalue->functionName, "stop") != NULL)
			{
				printTabs(scope_current);
				sprintf(str, "stop(%s)\n", subString);
				appendString(str);
			}
		}
		if (strstr(body->functionvalue->functionName, "finish") != NULL)
			{
				printTabs(scope_current);
				sprintf(str, "finish(\"%s\")\n", body->functionvalue->parameters->returnvalue->constant->str);
				appendString(str);
			}
		GenerateBody(body->body);
		break;
	case BODY_THIS_ARRAY_FUNC_BODY:
		LogDebug("Generating this array function body...");
		LogDebug("body->array: %p", body->array);
		GenerateArray(body->array);
		// hacemos logdebug del valor de bodu->functionvalue
		LogDebug("body->functionvalue: %p", body->functionvalue);
		GenerateFunctionValue(body->functionvalue);
		GenerateBody(body->body);
		break;
	case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY:
		LogDebug("type varname = const");
		appendString("\n");
		GenerateVarname(body->var);
		appendString(" = ");
		GenerateConstant(body->constant);
		GenerateBody(body->body);
		break;
	case BODY_TYPE_VARNAME_ARRAY_BODY:
		LogDebug("Generating type varname array body...");
		switch (body->type->tType)
		{
		case T_BLOCK:
			// GenerateBlock(body);
			break;
		}
		GenerateBody(body->body);
		break;
	case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_STRING_BODY:
		LogDebug("Generating type varname array assignment string body...");
		variable_context = true;
		GenerateType(body->type);
		GenerateVarname(body->var);
		GenerateArray(body->array);
		GenerateAssignment(body->assignment);
		GenerateString(body->str);
		variable_context = false;
		GenerateBody(body->body);
		break;
	case BODY_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY:
		LogDebug("Generating varname array assignment constant body...");
		variable_context = true;
		GenerateVarname(body->var);
		GenerateArray(body->array);
		GenerateAssignment(body->assignment);
		GenerateConstant(body->constant);
		variable_context = false;
		GenerateBody(body->body);
		break;
	case BODY_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY:
		LogDebug("Generating varname array assignment function body...");
		GenerateBody(body->body);
		break;
	case BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY:
		LogDebug("Generating type varname array assignment function body...");
		switch (body->type->tType)
		{
		case T_INT:
			break;
		case T_TEXT:
			break;
		case T_BOOL:
			break;
		case T_CHAR:
			break;
		case T_RGB:
			break;
		case T_BLOCK:
			break;
		case T_GOBJECT:
			char str[100];
			sprintf(str, "\n%s = %s()\n", body->var, body->functionvalue->functionName);
			appendString(str);
			newGobject(body->var);
			break;
		case T_SCENE:
			break;
		case T_BUTTON:
			break;
		case T_STRING:
			break;
		}
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
	appendString("\n");

	return;
}

void ContainsGobjectInits(Body body)
{
	boolean hitbox_set = false;
	Body current_body = body;

	if (current_body->bType == BODY_TYPE_VARNAME_ARRAY_BODY)
	{
		if (strcmp(current_body->var, "hitbox") == 0 && current_body->type->tType == T_BLOCK)
		{
			if (current_body->array != NULL && current_body->array->value > 0 && current_body->array->array != NULL && current_body->array->array->value > 0 && !hitbox_set)
			{
				char str[100];
				sprintf(str, " pos = [0, 0],width = %d*block_size, height = %d*block_size, speedx = 0, speedy = 0):\n", current_body->array->value, current_body->array->array->value);
				appendString(str);
				hitbox_set = true;
			}
		}
	}
}

void newGobject(const char *new_string)
{
	// Verificar si se necesita redimensionar el array
	if (gobjects_count >= array_capacity)
	{
		array_capacity = (array_capacity == 0) ? INITIAL_SIZE : array_capacity * 2;
		char **temp = realloc(gobject_array, sizeof(char *) * array_capacity);
		if (temp == NULL)
		{
			printf("Error: No se pudo asignar memoria para el array.\n");
			return;
		}
		gobject_array = temp;
	}

	// Asignar memoria para el nuevo string y copiar su contenido
	gobject_array[gobjects_count] = malloc(strlen(new_string) + 1);
	if (gobject_array[gobjects_count] == NULL)
	{
		printf("Error: No se pudo asignar memoria para el nuevo string.\n");
		return;
	}
	strcpy(gobject_array[gobjects_count], new_string);
	gobjects_count++;
}

void GenerateExpression(Expression expression)
{
	LogDebug("Generating expression...");
	switch (expression->type)
	{
	case EXP_FUN_EXP:
		function_context = true;
		GenerateFunction(expression->function);
		GenerateExpression(expression->expression);
		function_context = false;
		break;
	case EXP_CONST_EXP:
		GenerateConstante(expression->constante);
		GenerateExpression(expression->expression);
		break;
	}

	LogDebug("Expression generated.");

	return;
}

void GenerateFunction(Function function)
{
	LogDebug("Generating Function...");

	switch (function->type->tType)
	{
	case T_INT:
		appendString("def ");
		appendString(function->var);
		appendString("(");
		GenerateParametersDef(function->parameters_def);
		break;
	case T_TEXT:
		break;
	case T_BOOL:
		break;
	case T_CHAR:
		break;
	case T_RGB:
		break;
	case T_BLOCK:
		break;
	case T_GOBJECT:
		appendString("class ");
		appendString(function->var);
		appendString(":\n");
		appendString("\tdef __init__(self,"); //, pos=[0, 0], width=20, height=20, speedx=0, speedy=0):\n"
		ContainsGobjectInits(function->body);
		appendString("\t\tself.pos = pos\n\t\tself.width = width\n\t\tself.height = height\n\t\tself.speedx = speedx\n\t\tself.speedy = speedy\n");
		appendString("\tdef assignKeys(self, user_speed=0, upkey=pygame.BLEND_MULT, downkey=pygame.BLEND_MULT, rightkey=pygame.BLEND_MULT, leftkey=pygame.BLEND_MULT):\n");
		appendString("\t\tkeys = pygame.key.get_pressed()\n\t\tself.speedx = 0\n\t\tself.speedy = 0\n\t\tif keys[upkey]:\n\t\t\tself.speedy = -user_speed\n\t\tif keys[downkey]:\n\t\t\tself.speedy = user_speed\n\t\tif keys[rightkey]:\n\t\t\tself.speedx = user_speed\n\t\tif keys[leftkey]:\n\t\t\tself.speedx = -user_speed\n");
		appendString("\tdef updateMovement(self):\n");
		appendString("\t\tself.pos[0] += self.speedx\n\t\tself.pos[1] += self.speedy\n");
		appendString("\tdef randomMovement(self, userSpeed = 0):\n");
		appendString("\t\tself.speedx = userSpeed*random.choice((-1,1))\n");
		appendString("\t\tself.speedy = userSpeed*random.choice((-1,1))\n");

		break;
	case T_SCENE:
		break;
	case T_BUTTON:
		break;
	case T_STRING:
		break;
	}

	GenerateBody(function->body);

	LogDebug("Function generated.");

	return;
}

void GenerateConstante(Constante constante)
{
	LogDebug("Generating Constante...");

	GenerateValue(constante->value);

	LogDebug("Constante generated.");

	return;
}

void GenerateType(Type type)
{
	LogDebug("Generating Type...");

	switch (type->tType)
	{
	case T_INT:
		break;
	case T_TEXT:
		break;
	case T_BOOL:
		break;
	case T_CHAR:
		break;
	case T_RGB:
		break;
	case T_BLOCK:
		break;
	case T_GOBJECT:
		break;
	case T_SCENE:
		break;
	case T_BUTTON:
		break;
	case T_STRING:
		break;
	}

	LogDebug("Type generated.");
}

void GenerateParametersDef(ParametersDef parameters_def)
{
	LogDebug("Generating ParametersDef...");

	switch (parameters_def->pType)
	{
	case PD_TYPE_VAR:
		LogDebug("ESTO ES ACA 8: %s", beforeCycle);
		appendString(parameters_def->var);
		appendString("):\n");
		// sprintf(beforeCycle + strlen(beforeCycle), "%s):\n", parameters_def->var);
		break;
	case PD_TYPE_VAR_COMMA_PD:
		LogDebug("ESTO ES ACA 8: %s", beforeCycle);
		appendString(parameters_def->var);
		appendString(", ");
		// sprintf(beforeCycle + strlen(beforeCycle), "%s, ", parameters_def->var);
		GenerateParametersDef(parameters_def->parametersdef);
		break;
	}

	LogDebug("ParametersDef generated.");

	return;
}

void GenerateParameters(Parameters parameters)
{
	LogDebug("Generating Parameters...");

	switch (parameters->pType)
	{
	case P_LAMBDA:
		break;
	case P_RETVAL:
		GenerateReturnValue(parameters->returnvalue);

		break;
	case P_RETVAL_PARAMS:
		GenerateReturnValue(parameters->returnvalue);
		appendString(", ");
		GenerateParameters(parameters->parameters);
		break;
	}

	LogDebug("Parameters generated.");

	return;
}

void GenerateVarSingleAction(VarSingleAction varsingleaction)
{
	LogDebug("Generating VarSingleAction...");

	switch (varsingleaction->type)
	{
	case S_INCREMENT:
		appendString("+= 1\n");
		break;
	case S_DECREMENT:
		appendString("-= 1\n");
		break;
	}

	LogDebug("VarSingleAction generated.");

	return;
}

void GenerateAssignment(Assignment assignment)
{
	LogDebug("Generating Assignment...");

	switch (assignment->type)
	{
	case ASSIGNMENT_EQUAL:
		break;
	case ASSIGNMENT_PLUS_EQUAL:
		break;
	case ASSIGNMENT_MINUS_EQUAL:
		break;
	case ASSIGNMENT_MUL_EQUAL:
		break;
	case ASSIGNMENT_SLASH_EQUAL:
		break;
	}

	LogDebug("Assignment generated.");

	return;
}

void GenerateFunctionAssignment(FunctionAssignment functionAssignment)
{
	LogDebug("Generating FunctionAssignment...");

	GenerateParameters(functionAssignment->parameters);

	LogDebug("FunctionAssignment generated.");

	return;
}

void GenerateArray(Array array)
{
	LogDebug("Generating Array...");

	switch (array->aType)
	{
	case ARRAY_VARNAME_ARRAY:
		printTabs(scope_current);
		GenerateVarname(array->var);
		GenerateArray(array->array);
		break;
	case ARRAY_INTEGER_ARRAY:
		printTabs(scope_current);
		GenerateInteger(array->value);
		GenerateArray(array->array);
		break;
	case ARRAY_ARRAY:
		printTabs(scope_current);
		GenerateArray(array->array);
		break;
	case ARRAY_LAMBDA:
		break;
	}

	LogDebug("Array generated.");

	return;
}

void GenerateReturnValue(ReturnValue returnvalue)
{
	LogDebug("Generating ReturnValue...");

	switch (returnvalue->rtType)
	{
	case RT_CONSTANT:
		GenerateConstant(returnvalue->constant);
		break;
	case RT_THIS:
		break;
	case RT_CHAR_TEXT:
		GenerateChar(returnvalue->c);
		break;
	case RT_GCONSTANT:
		GenerateGConstant(returnvalue->gconstant);
		break;
	case RT_THIS_VAR_ARRAY:
		GenerateArray(returnvalue->array);
		break;
	case RT_CONS_SUB_CONS:
		GenerateConstant(returnvalue->leftconstant);
		appendString(" - ");
		GenerateConstant(returnvalue->rightconstant);
		break;
	case RT_CONS_ADD_CONS:
		GenerateConstant(returnvalue->leftconstant);
		appendString(" + ");
		GenerateConstant(returnvalue->rightconstant);
		break;
	case RT_CONS_MUL_CONS:
		GenerateConstant(returnvalue->leftconstant);
		appendString(" * ");
		GenerateConstant(returnvalue->rightconstant);
		break;
	case RT_CONS_DIV_CONS:
		GenerateConstant(returnvalue->leftconstant);
		appendString(" / ");
		GenerateConstant(returnvalue->rightconstant);
		break;
	}

	LogDebug("ReturnValue generated.");

	return;
}

void GenerateValue(Value value)
{
	LogDebug("Generating Value...");

	switch (value->vType)
	{
	case V_TYPE:
		GenerateType(value->type);
		break;
	case V_FUNV:
		GenerateFunctionValue(value->functionvalue);
		break;
	}

	LogDebug("Value generated.");

	return;
}

void GenerateFunctionValue(Functionvalue functionvalue)
{
	LogDebug("Generating Functionvalue...");
	GenerateParameters(functionvalue->parameters);

	LogDebug("Functionvalue generated.");
}

void GenerateConditionals(Conditionals conditionals)
{
	LogDebug("Generating Conditionals...");

	switch (conditionals->conditionalsType)
	{
	case COND_WHEN:
		inCycle_context = true;
		scope_current++;
		printTabs(scope_current);
		appendString("if ");
		GenerateBoolean(conditionals->boolean);
		appendString(":\n");
		GenerateBody(conditionals->firstBody);
		scope_current--;
		inCycle_context = false;
		break;
	case COND_WHEN_ELSE:
		GenerateNegation(conditionals->negation);
		GenerateBoolean(conditionals->boolean);
		GenerateBody(conditionals->firstBody);
		GenerateBody(conditionals->secondBody);
		break;
	case COND_IF:
		inCycle_context = true;
		scope_current++;
		printTabs(scope_current);
		appendString("if ");
		GenerateNegation(conditionals->negation);
		GenerateBoolean(conditionals->boolean);
		appendString(":\n");
		GenerateBody(conditionals->firstBody);
		scope_current--;
		inCycle_context = false;
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

void GenerateNegation(Negation negation)
{
	LogDebug("Generating Negation...");

	switch (negation->nType)
	{
	case N_NOT:
		break;
	}

	LogDebug("Negation generated.");

	return;
}

void GenerateForOptions(ForOptions foroptions)
{
	LogDebug("Generating ForOptions...");

	switch (foroptions->forOptionsType)
	{
	case S_INCREMENT:
		break;
	case S_DECREMENT:
		break;
	}

	LogDebug("ForOptions generated.");

	return;
}

void GenerateIfOptions(IfOptions ifoptions)
{
	LogDebug("Generating IfOptions...");

	switch (ifoptions->ifOptionsType)
	{
	case IF_OPTIONS_ELSE:
		inCycle_context = true;
		scope_current++;
		printTabs(scope_current);
		appendString("else ");
		GenerateBoolean(ifoptions->boolean);
		appendString(":\n");
		GenerateBody(ifoptions->body);
		scope_current--;
		inCycle_context = false;
		break;
	case IF_OPTIONS_ELIF:
		inCycle_context = true;
		scope_current++;
		printTabs(scope_current);
		appendString("elif ");
		GenerateBoolean(ifoptions->boolean);
		appendString(":\n");
		GenerateBody(ifoptions->body);
		scope_current--;
		inCycle_context = false;
		break;
	}

	LogDebug("IfOptions generated.");

	return;
}

void GenerateGSBoolean(GSBoolean boolean)
{
	LogDebug("Generating Boolean...");

	switch (boolean->booltype)
	{
	case BOOL_LESS_THAN:
		GenerateMathExpression(boolean->leftMathExp);
		GenerateMathExpression(boolean->rightMathExp);
		break;
	case BOOL_GREATER_THAN:
		GenerateMathExpression(boolean->leftMathExp);
		GenerateMathExpression(boolean->rightMathExp);
		break;
	case BOOL_LESS_THAN_EQ:
		GenerateMathExpression(boolean->leftMathExp);
		GenerateMathExpression(boolean->rightMathExp);
		break;
	case BOOL_GREATER_THAN_EQ:
		GenerateMathExpression(boolean->leftMathExp);
		GenerateMathExpression(boolean->rightMathExp);
		break;
	case BOOL_EQ_EQ:
		GenerateBoolean(boolean->leftBoolean);
		GenerateBoolean(boolean->rightBoolean);
		break;
	case BOOL_NOT_EQ:
		GenerateBoolean(boolean->leftBoolean);
		GenerateBoolean(boolean->rightBoolean);
		break;
	case BOOL_OR:
		GenerateBoolean(boolean->leftBoolean);
		GenerateBoolean(boolean->rightBoolean);
		break;
	case BOOL_AND:
		GenerateBoolean(boolean->leftBoolean);
		GenerateBoolean(boolean->rightBoolean);
		break;
	case BOOL_V_IN_V:
		GenerateVarname(boolean->lefVar);
		appendString(", ");
		GenerateVarname(boolean->rightVar);
		appendString("):\n");
		break;
	case BOOL_V_IN_GC:
		GenerateVarname(boolean->lefVar);
		GenerateVarname(boolean->rightVar);
		break;
	case BOOL_VAR:
		GenerateVarname(boolean->var);
		break;
	case BOOL_FUNA:
		GenerateVarname(boolean->var);
		GenerateFunctionAssignment(boolean->functionassignment);
		break;
	case BOOL_V_HITS_V:
		GenerateVarname(boolean->lefVar);
		GenerateVarname(boolean->rightVar);
		break;
	case BOOL_V_HITS_GC:
		GenerateVarname(boolean->lefVar);
		GenerateVarname(boolean->rightVar);
		break;
	case BOOL_CONDITIONALS:
		GenerateConditionals(boolean->conditionals);
		break;
	}

	LogDebug("Boolean generated.");

	return;
}

void GenerateMathExpression(Mathexp mathExpression)
{
	LogDebug("Generating MathExpression...");

	switch (mathExpression->mathExpType)
	{
	case MATH_EXP_ADD:
		GenerateMathExpression(mathExpression->leftMathExp);
		appendString(" + ");
		GenerateMathExpression(mathExpression->rightMathExp);
		break;
	case MATH_EXP_SUB:
		GenerateMathExpression(mathExpression->leftMathExp);
		appendString(" - ");
		GenerateMathExpression(mathExpression->rightMathExp);
		break;
	case MATH_EXP_MUL:
		GenerateMathExpression(mathExpression->leftMathExp);
		appendString(" * ");
		GenerateMathExpression(mathExpression->rightMathExp);
		break;
	case MATH_EXP_DIV:
		GenerateMathExpression(mathExpression->leftMathExp);
		appendString(" / ");
		GenerateMathExpression(mathExpression->rightMathExp);
		break;
	case MATH_EXP_FACTOR:
		GenerateFactor(mathExpression->factor);
		break;
	}

	LogDebug("MathExpression generated.");

	return;
}

void GenerateFactor(Factor factor)
{
	LogDebug("Generating Factor...");

	switch (factor->factorType)
	{
	case FACTOR_MATH_EXP:
		GenerateMathExpression(factor->mathexp);
		break;

	case FACTOR_CONSTANT:
		GenerateConstant(factor->constant);
		break;
	}

	LogDebug("Factor generated.");

	return;
}

void convertToUpper(const char *input)
{
	// Iterar a través de cada carácter del string
	for (size_t i = 0; i < strlen(input); i++)
	{
		// Convertir el carácter a mayúsculas y agregarlo a tu estructura de datos
		char upperChar = toupper(input[i]);
	}
}

void GenerateConstant(Constant constant)
{
	LogDebug("Generating Constant...");

	switch (constant->cType)
	{
	case CONST_INT:
		char numerito[255];
		sprintf(numerito, "%d", constant->value);
		appendString(numerito);
		// appendString("\n");
		break;
	case CONST_STR:
		appendString("pygame.K_");
		convertToUpper(constant->str);
		appendString(constant->str);
		break;
	case CONST_VAR:
		GenerateVarname(constant->var);
		break;
	case CONST_SUB_INT:
		GenerateInteger(constant->value);
		break;
	case CONST_INT_PX:
		GenerateInteger(constant->value);
		break;
	}

	LogDebug("Constant generated.");

	return;
}

void GenerateGConstant(GConstant gconstant)
{
	LogDebug("Generating GConstant...");

	switch (gconstant->gcType)
	{
	case GC_UP_B:
		appendString("UP_BORDER");
		break;
	case GC_DOWN_B:
		appendString("DOWN_BORDER");
		break;
	case GC_LEFT_B:
		appendString("LEFT_BORDER");
		break;
	case GC_RIGHT_B:
		appendString("RIGHT_BORDER");
		break;
	case GC_UP:
		appendString("pygame.K_UP");
		break;
	case GC_DOWN:
		appendString("pygame.K_DOWN");
		break;
	case GC_LEFT:
		appendString("pygame.K_LEFT");
		break;
	case GC_RIGHT:
		appendString("pygame.K_RIGHT");
		break;
	case GC_SPACE:
		break;
	case GC_ENTER:
		break;
	case GC_NOKEY:
		appendString("pygame.BLEND_MULT");
		break;
	}

	LogDebug("GConstant generated.");

	return;
}

void GenerateVarname(char *var)
{
	LogDebug("Generating Varname...");
	printTabs(scope_current);
	appendString(var);
	LogDebug("Varname generated.");

	return;
}

void GenerateInteger(int value)
{
	LogDebug("Generating Integer...");

	LogDebug("Integer generated.");

	return;
}

void GenerateChar(char c)
{
	LogDebug("Generating Char...");
	appendString(c);

	LogDebug("Char generated.");

	return;
}

void GenerateString(char *str)
{
	LogDebug("Generating String...");

	LogDebug("String generated.");

	return;
}

void GenerateBoolean(GSBoolean to_boolean)
{
	LogDebug("Generating Boolean...");

	switch (to_boolean->booltype)
	{
	case BOOL_LESS_THAN:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" < ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_GREATER_THAN:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" > ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_LESS_THAN_EQ:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" <= ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_GREATER_THAN_EQ:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" >= ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_EQ_EQ:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" == ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_NOT_EQ:
		GenerateMathExpression(to_boolean->leftMathExp);
		appendString(" != ");
		GenerateMathExpression(to_boolean->rightMathExp);
		break;
	case BOOL_OR:
		GenerateBoolean(to_boolean->leftBoolean);
		appendString(" or ");
		GenerateBoolean(to_boolean->rightBoolean);
		break;
	case BOOL_AND:
		GenerateBoolean(to_boolean->leftBoolean);
		appendString(" and ");
		GenerateBoolean(to_boolean->rightBoolean);
		break;
	case BOOL_V_IN_V:
		appendString("when_in_string(");
		GenerateVarname(to_boolean->lefVar);
		appendString(", ");
		GenerateVarname(to_boolean->rightVar);
		appendString(")");
		break;
	case BOOL_V_IN_GC:
		appendString("when_in_");
		GenerateGConstant(to_boolean->gconstant);
		appendString("_string(");
		GenerateVarname(to_boolean->lefVar);
		appendString(")");
		break;
	case BOOL_VAR:
		GenerateVarname(to_boolean->var);
		break;
	case BOOL_FUNA:
		GenerateVarname(to_boolean->var);
		GenerateFunctionAssignment(to_boolean->functionassignment);
		break;
	case BOOL_V_HITS_V:
		GenerateVarname(to_boolean->lefVar);
		GenerateVarname(to_boolean->rightVar);
		break;
	case BOOL_V_HITS_GC:
		GenerateVarname(to_boolean->lefVar);
		GenerateGConstant(to_boolean->gconstant);
		break;
	case BOOL_CONDITIONALS:
		GenerateConditionals(to_boolean->conditionals);
		break;
	}
}
