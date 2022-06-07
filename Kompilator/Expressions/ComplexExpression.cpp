#include "ComplexExpression.h"
#include "Expression.h"

using namespace std;

void ComplexExpression::execute(Stack& stack) {
	returnValue leftValue;
	_leftExpression->execute(stack);
	leftValue = stack.getReturnedValue();

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
			compareNumericValues<int, int>(_operatorType, get<int>(leftValue.second), get<int>(rightValue.second), stack, _isNegation );
			return;
		}
		if (isFloat(rightValue.first)) {
			compareNumericValues<int, float>(_operatorType, get<int>(leftValue.second), get<float>(rightValue.second), stack, _isNegation );
			return;
		}
	}
	else if (isFloat(leftValue.first)) {
		if (isInt(rightValue.first)) {
			compareNumericValues<float, int>(_operatorType, get<float>(leftValue.second), get<int>(rightValue.second), stack, _isNegation );
			return;
		}
		if (isFloat(rightValue.first)) {
			compareNumericValues<float, float>(_operatorType, get<float>(leftValue.second), get<float>(rightValue.second), stack, _isNegation );
			return;
		}
	}
	else if (isString(leftValue.first)) {
		if (isString(rightValue.first)) {
			compareNumericValues<string, string>(_operatorType, get<string>(leftValue.second), get<string>(rightValue.second), stack, _isNegation );
			return;
		}
	}
	else {
		throw("Porownywac mozna tylko Inty, Floaty i Stringi");
	}
}