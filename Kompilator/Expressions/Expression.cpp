#include "Expression.h"

using namespace std;

void Expression::execute(Stack& stack) {
	returnValue retValueFirst;
	bool floatWasUsed = false;

	_leftAddComponent->execute(stack);
	retValueFirst = stack.getReturnedValue();
	if (!_isPositive) {
		if (isInt(retValueFirst.first)) {
			retValueFirst.second = -get<int>(retValueFirst.second);
		}
		else if (isFloat(retValueFirst.first)) {
			retValueFirst.second = -get<float>(retValueFirst.second);
			floatWasUsed = true;
		}
		else {
			throw("Niepotrzebny minus przy wyrazeniu");
		}
	}


	for (int i = 0; i < _rightComponents.size(); ++i) {
		bool isSum;
		returnValue retValueNext;

		isSum = _additionOperators[i];
		_rightComponents[i]->execute(stack);
		retValueNext = stack.getReturnedValue();

		if (isInt(retValueFirst.first)) {
			if (isInt(retValueNext.first)) {
				if (isSum) {
					retValueFirst.second = get<int>(retValueFirst.second) + get<int>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<int>(retValueFirst.second) - get<int>(retValueNext.second);
				}
			}
			else if (isFloat(retValueNext.first)) {
				if (isSum) {
					retValueFirst.second = get<int>(retValueFirst.second) + get<float>(retValueNext.second);
					floatWasUsed = true;
				}
				else {
					retValueFirst.second = get<int>(retValueFirst.second) - get<float>(retValueNext.second);
					floatWasUsed = true;
				}
			}
			else {
				throw("Nie mozna dodac dwoch roznych typow");
			}	
		}
		else if (isFloat(retValueFirst.first)) {
			if (isInt(retValueNext.first)) {
				if (isSum) {
					retValueFirst.second = get<float>(retValueFirst.second) + get<int>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<float>(retValueFirst.second) - get<int>(retValueNext.second);
				}
			}
			else if (isFloat(retValueNext.first)) {
				if (isSum) {
					retValueFirst.second = get<float>(retValueFirst.second) + get<float>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<float>(retValueFirst.second) - get<float>(retValueNext.second);
				}
			}
			else {
				throw("Nie mozna dodac dwoch roznych typow");
			}
		}
		else if (isString(retValueFirst.first)) {
			if (isString(retValueNext.first)) {
				if (isSum) {
					retValueFirst.second = get<string>(retValueFirst.second) + get<string>(retValueNext.second);
				}
				else {
					throw("Nie mozna odjac stringow");
				}
			}
			else {
				throw("Nie mozna dodac dwoch roznych typow");
			}
		}
		else {
			throw("Dodawac/Odejmowac mozna tylko Inty, Floaty i Stringi");
		}
			
	}

	if (floatWasUsed)
		stack.setReturnedValue(make_pair(TokenTypes::FLOAT, retValueFirst.second));
	else
		stack.setReturnedValue(retValueFirst);
}