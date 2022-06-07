#pragma once
#include "..\Statements\Statement.h"
#include "Expression.h"

class RestOfComplexExpression : public Statement
{
public:
	RestOfComplexExpression(std::string operatorType, UniqueExpression& rightExpression)
	{
		_operatorType = operatorType;
		_rightExpression = move(rightExpression);
	}
	void execute(Stack& stack);
	void setIdent(std::string ident);
private:
	std::string _identifier;
	std::string _operatorType;
	UniqueExpression _rightExpression;
};

