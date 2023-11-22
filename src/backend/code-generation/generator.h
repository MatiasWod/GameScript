#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER
#include "../semantic-analysis/abstract-syntax-tree.h"

void Generator(Program program);

void GenerateProgram(Program program);

void GenerateMainScene(Mainscene main_scene);

void GenerateBody(Body body);

void GenerateExpression(Expression expression);

void GenerateFunction(Function function);

void GenerateConstante(Constante constante);

void GenerateType(Type type);

void GenerateParametersDef(ParametersDef parameters_def);

void GenerateParameters(Parameters parameters);

void GenerateVarSingleAction(VarSingleAction varsingleaction);

void GenerateAssignment(Assignment assignment);

void GenerateFunctionAssignment(FunctionAssignment functionAssignment);

void GenerateArray(Array array);

void GenerateReturnValue(ReturnValue returnvalue);

void GenerateValue(Value value);

void GenerateFunctionValue(Functionvalue functionvalue);

void GenerateConditionals(Conditionals conditionals);

void GenerateNegation(Negation negation);

void GenerateForOptions(ForOptions foroptions);

void GenerateIfOptions(IfOptions ifoptions);

void GenerateBoolean(GSBoolean boolean);

void GenerateMathExpression(Mathexp mathExpression);

void GenerateFactor(Factor factor);

void GenerateConstant(Constant constant);

void GenerateGConstant(GConstant gconstant);

void GenerateVarname(char * var);


void GenerateInteger(int value);

void GenerateChar(char c);

void GenerateString(char * str);

#endif
