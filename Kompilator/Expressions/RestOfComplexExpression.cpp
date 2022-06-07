#include "RestOfComplexExpression.h"

using namespace std;

void RestOfComplexExpression::execute(Stack& stack)
{
	returnValue leftValue;
	leftValue = make_pair(stack.getVariableDefinition(_identifier), stack.getVariableValue(_identifier));

	if (_operatorType == "") {
		stack.setReturnedValue(leftValue);
		return;
	}

	returnValue rightValue;
	_rightExpression->execute(stack);
	rightValue = stack.getReturnedValue();

	if (!compareTypes(leftValue.first, rightValue.first))
		throw("Proba porownania dwoch roznych typow");

	if (isInt(leftValue.first)) {
		if (isInt(rightValue.first)) {
			ComplexExpression::compareNumericValues<int, int>(_operatorType, get<int>(leftValue.second), get<int>(rightValue.second), stack);
			return;
		}
		if (isFloat(rightValue.first)) {
			ComplexExpression::compareNumericValues<int, float>(_operatorType, get<int>(leftValue.second), get<float>(rightValue.second), stack);
			return;
		}
	}
	else if (isFloat(leftValue.first)) {
		if (isInt(rightValue.first)) {
			ComplexExpression::compareNumericValues<float, int>(_operatorType, get<float>(leftValue.second), get<int>(rightValue.second), stack);
			return;
		}
		if (isFloat(rightValue.first)) {
			ComplexExpression::compareNumericValues<float, float>(_operatorType, get<float>(leftValue.second), get<float>(rightValue.second), stack);
			return;
		}
	}
	else if (isString(leftValue.first)) {
		if (isString(rightValue.first)) {
			ComplexExpression::compareNumericValues<string, string>(_operatorType, get<string>(leftValue.second), get<string>(rightValue.second), stack);
			return;
		}
	}
	else {
		throw("Porownywac mozna tylko Inty, Floaty i Stringi");
	}
}

void RestOfComplexExpression::setIdent(std::string ident)
{
	_identifier = ident;
}
