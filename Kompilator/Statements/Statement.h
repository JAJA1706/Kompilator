#ifndef STATEMENT_H
#define STATEMENT_H
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include "..\Lekser\TokenTypes.h"
#include "..\Program\Stack.h"

typedef std::unique_ptr<class VariableDefinition> UniqueVariableDefinition;
typedef std::unique_ptr<class FunctionDefinition> UniqueFunctionDefinition;
typedef std::unique_ptr<class Statement> UniqueStatement;
typedef std::unique_ptr<class IfStatement> UniqueIfStatement;
typedef std::unique_ptr<class AssignStatement> UniqueAssignStatement;
typedef std::unique_ptr<class CallStatement> UniqueCallStatement;
typedef std::unique_ptr<class RestOfComplexExpression> UniqueRestOfComplexExpression;
typedef std::unique_ptr<class ComplexExpression> UniqueComplexExpression;
typedef std::unique_ptr<class Expression> UniqueExpression;
typedef std::unique_ptr<class AdditionComponent> UniqueAdditionComponent;
typedef std::unique_ptr<class MultiplyComponent> UniqueMultiplyComponent;
typedef std::unique_ptr<class ArgumentList> UniqueArgumentList;
typedef std::unique_ptr<class ParameterList> UniqueParameterList;
typedef std::unique_ptr<class Closure> UniqueClosure;
typedef std::unique_ptr<class Element> UniqueElement;
typedef std::unique_ptr<class Lambda> UniqueLambda;

class Stack;

class Statement
{
public:
	virtual ~Statement() = 0 {}
	virtual void execute(Stack& stack) = 0;
	static bool compareTypes(TokenTypes first, TokenTypes second);
	static bool isInt(TokenTypes type);
	static bool isFloat(TokenTypes type);
	static bool isString(TokenTypes type);
	static bool isNumber(TokenTypes type);
};


#endif