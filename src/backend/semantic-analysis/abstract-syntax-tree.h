#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

typedef struct ProgramNode *Program;
typedef struct MainSceneNode *Mainscene;
typedef struct ExpressionNode *Expression;
typedef struct FactorNode *Factor;
typedef struct ConstantNode *Constant;
typedef struct ConditionalsNode *Conditionals;
typedef struct GSBooleanNode *GSBoolean;
typedef struct FunctionNode *Function;
typedef struct TypeNode *Type;
typedef struct ParametersDefNode *ParametersDef;
typedef struct ParametersNode *Parameters;
typedef struct BodyNode *Body;
typedef struct ValueNode *Value;
typedef struct FunctionValueNode *Functionvalue;
typedef struct MathexpNode *Mathexp;
typedef struct ConstanteNode *Constante;
typedef struct ArrayNode *Array;
typedef struct AssignmentNode *Assignment;
typedef struct ReturnValueNode *ReturnValue;
typedef struct VarSingleActionNode *VarSingleAction;
typedef struct NegationNode *Negation;
typedef struct GConstantNode *GConstant;
typedef struct FunctionAssignmentNode *FunctionAssignment;
typedef struct ForOptionsNode *ForOptions;
typedef struct IfOptionsNode *IfOptions;
typedef struct StrNode *Str;
typedef struct CharacterNode *Character;

struct ProgramNode
{
	Mainscene mainscene;
};

struct MainSceneNode
{
	Body body;
	Expression expression;
};

typedef enum {
    EXP_LAMBDA,
    EXP_FUN_EXP,
    EXP_CONST_EXP
} ExpressionType;

struct ExpressionNode
{
	ExpressionType type;
	Expression expression;
	Function function;
	Constante constante;
};

typedef enum {
	BODY_LAMBDA,
	BODY_CONDITIONALS_BODY,
	BODY_THIS_ARRAY_ASSIGNMENT_RET_BODY,
	BODY_THIS_ARRAY_ASSIGNMENT_FUNC_BODY,
	BODY_FUNC_BODY,
	BODY_THIS_ARRAY_FUNC_BODY,
	BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY,
	BODY_TYPE_VARNAME_ARRAY_BODY,
	BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_STRING_BODY,
	BODY_VARNAME_ARRAY_ASSIGNMENT_CONST_BODY,
	BODY_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY,
	BODY_TYPE_VARNAME_ARRAY_ASSIGNMENT_FUNC_BODY,
	BODY_VARNAME_ARRAY_VARSINGLE_BODY,
	BODY_RETURN_RET,
	BODY_ONCLICK
}BodyType;

struct BodyNode
{
	BodyType bType;
	char * var;
	char * str;
	Body body;
	Conditionals conditionals;
	Array array;
	Assignment assignment;
	ReturnValue returnvalue;
	Functionvalue functionvalue;
	Type type;
	Constant constant;
	VarSingleAction varsingleaction;
};

struct FunctionNode
{
	Type type;
	char * var;
	ParametersDef parameters_def;
	Body body;
};

struct ConstanteNode
{
	Value value;
};

typedef enum {
	COND_WHEN,
	COND_WHEN_ELSE,
	COND_IF,
	COND_FOR,
	COND_WHILE
} ConditionalsType;

struct ConditionalsNode
{
	ConditionalsType conditionalsType;
	Body firstBody;
	Body secondBody;
	Negation negation;
	GSBoolean boolean;
	char * varName;
	int rightValue;
	ForOptions foroptions;
	IfOptions ifoptions;
};

typedef enum {
	ARRAY_LAMBDA,
	ARRAY_VARNAME_ARRAY,
	ARRAY_INTEGER_ARRAY,
	ARRAY_ARRAY
} ArrayType;

struct ArrayNode
{
	ArrayType aType;
	char * var;
	int value;
	Array array;
};

typedef enum
{
	ASSIGNMENT_EQUAL,
	ASSIGNMENT_PLUS_EQUAL,
	ASSIGNMENT_MINUS_EQUAL,
	ASSIGNMENT_SLASH_EQUAL,
	ASSIGNMENT_MUL_EQUAL
} AssignmentType;

struct AssignmentNode
{
	AssignmentType type;
};

typedef enum
{
	RT_CONSTANT,
	RT_THIS,
	RT_CHAR_TEXT,
	RT_GCONSTANT,
	RT_THIS_VAR_ARRAY,
	RT_THIS_FUNC,
	RT_CONS_SUB_CONS,
	RT_CONS_ADD_CONS,
	RT_CONS_MUL_CONS,
	RT_CONS_DIV_CONS
} ReturnValueType;

struct ReturnValueNode
{
	ReturnValueType rtType;
	Expression expression;
	Constant constant;
	GConstant gconstant;
	Array array;
	Functionvalue functionvalue;
	Constant leftconstant;
	Constant rightconstant;
	char * var;
	char c;
};

typedef enum
{
	GC_UP_B,
	GC_DOWN_B,
	GC_LEFT_B,
	GC_RIGHT_B,
	GC_UP,
	GC_DOWN,
	GC_LEFT,
	GC_RIGHT,
	GC_SPACE,
	GC_ENTER,
	GC_NOKEY
} GConstantType;

struct GConstantNode
{
	GConstantType gcType;
};

struct FunctionValueNode
{
	char *functionName;
	Parameters parameters;
};

typedef enum
{
	T_INT,
	T_TEXT,
	T_BOOL,
	T_CHAR,
	T_RGB,
	T_BLOCK,
	T_GOBJECT,
	T_SCENE,
	T_BUTTON,
	T_STRING
} TypeType;

struct TypeNode
{
	TypeType tType;
};

typedef enum{
	CONST_INT,
	CONST_VAR,
	CONST_SUB_INT,
	CONST_INT_PX,
	CONST_STR
}ConstantType;

struct ConstantNode
{
    ConstantType cType;
	int value;
    char * var;
    char * str;
};

typedef enum
{
	S_INCREMENT,
	S_DECREMENT
} SingleActionType;

struct VarSingleActionNode
{
	SingleActionType type;
};

typedef enum
{
	P_LAMBDA,
	P_RETVAL,
	P_RETVAL_PARAMS
} ParametersType;

struct ParametersNode
{
	ParametersType pType;
	ReturnValue returnvalue;
	Parameters parameters;
};

typedef enum
{
	PD_LAMBDA,
	PD_TYPE_VAR,
	PD_TYPE_VAR_COMMA_PD,
} ParametersDefType;

struct ParametersDefNode
{
	ParametersDefType pType;
	Type type;
	ParametersDef parametersdef;
	char *var;
};

typedef enum
{
	V_TYPE,
	V_FUNV
} ValueType;

struct ValueNode
{
	ValueType vType;
	Functionvalue functionvalue;
	Type type;
};

typedef enum
{
	N_LAMBDA,
	N_NOT
} NegationType;

struct NegationNode
{
	NegationType nType;
};

typedef enum
{
	BOOL_LESS_THAN,
	BOOL_GREATER_THAN,
	BOOL_LESS_THAN_EQ,
	BOOL_GREATER_THAN_EQ,
	BOOL_EQ_EQ,
	BOOL_NOT_EQ,
	BOOL_OR,
	BOOL_AND,
	BOOL_V_IN_V,
	BOOL_V_IN_GC,
	BOOL_VAR,
	BOOL_FUNA,
	BOOL_V_HITS_V,
	BOOL_V_HITS_GC,
	BOOL_CONDITIONALS
} BooleanType;

struct GSBooleanNode
{
	BooleanType booltype;
	Mathexp leftMathExp;
	Mathexp rightMathExp;
	GSBoolean leftBoolean;
	char *lefVar;
	char *rightVar;
	GSBoolean rightBoolean;
	char *var;
	GConstant gconstant;
	FunctionAssignment functionassignment;
	Conditionals conditionals;
};

typedef enum
{
	MATH_EXP_SUB,
	MATH_EXP_MUL,
	MATH_EXP_DIV,
	MATH_EXP_FACTOR,
	MATH_EXP_ADD
} MathExpType;

struct MathexpNode
{
	Mathexp leftMathExp;
	Mathexp rightMathExp;
	MathExpType mathExpType;
	Factor factor;
};

typedef enum{
	IF_OPTIONS_LAMBDA,
	IF_OPTIONS_ELIF,
	IF_OPTIONS_ELSE,
}IfOptionsType;

struct IfOptionsNode{
	Body body;
	GSBoolean boolean;
	IfOptions ifOptions;
	IfOptionsType ifOptionsType;
};

typedef enum {
	FOR_LAMBDA,
    FOR_INCREMENT,
    FOR_DECREMENT,
    FOR_PLUS_EQUAL,
    FOR_MINUS_EQUAL,
    FOR_SLASH_EQUAL,
    FOR_MUL_EQUAL,  
    FOR_EQUAL
}ForOptionsType;

struct ForOptionsNode {
	ForOptionsType forOptionsType;
	Mathexp mathexp;
	char * var;
	int value;
};

typedef enum
{
	FACTOR_MATH_EXP,
	FACTOR_CONSTANT
} FactorType;

struct FactorNode
{
	FactorType factorType;
	Mathexp mathexp;
	Constant constant;
};

struct FunctionAssignmentNode {
	Parameters parameters;
};
#endif
