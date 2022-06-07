#pragma once
#include "..\Statements\Statement.h"
#include "Expression.h"

class ComplexExpression : public Statement
{
public:
	ComplexExpression(bool isNegation, UniqueExpression& leftExpression, std::string operatorType, UniqueExpression& rightExpression)
	{
		_isNegation = isNegation;
		_leftExpression = move(leftExpression);
		_operatorType = operatorType;
		_rightExpression = move(rightExpression);
	}
	~ComplexExpression() {}
	void execute(Stack& stack);

	template<class Left, class Right>
	static void compareNumericValues(std::string operatorType, Left left, Right right, Stack& stack, bool negation = false);

private:

	bool _isNegation;
	UniqueExpression _leftExpression;
	std::string _operatorType;
	UniqueExpression _rightExpression;
};

template<class Left, class Right>
inline void ComplexExpression::compareNumericValues(std::string operatorType, Left left, Right right, Stack& stack, bool negation )
{
	if (operatorType == "==") {
		if (left == right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
	if (operatorType == "!=") {
		if (left != right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
	if (operatorType == ">") {
		if (left > right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
	if (operatorType == ">=") {
		if (left >= right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
	if (operatorType == "<") {
		if (left < right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
	if (operatorType == "<=") {
		if (left <= right)
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, !negation));
		else
			stack.setReturnedValue(std::make_pair(TokenTypes::CONST_INT, negation));
		return;
	}
}
