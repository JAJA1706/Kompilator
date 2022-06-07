#include "AdditionComponent.h"

using namespace std;

void AdditionComponent::execute(Stack& stack)
{
	returnValue retValueFirst;
	bool floatWasUsed = false;

	_leftMultiComponent->execute(stack);
	retValueFirst = stack.getReturnedValue();

	for (int i = 0; i < _rightMultiComponents.size(); ++i) {
		bool isMultiplication;
		returnValue retValueNext;

		isMultiplication = _multiOperators[i];
		_rightMultiComponents[i]->execute(stack);
		retValueNext = stack.getReturnedValue();

		if (isInt(retValueFirst.first)) {
			if (isInt(retValueNext.first)) {
				if (isMultiplication) {
					retValueFirst.second = get<int>(retValueFirst.second) * get<int>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<int>(retValueFirst.second) / get<int>(retValueNext.second);
				}
			}
			else if (isFloat(retValueNext.first)) {
				if (isMultiplication) {
					retValueFirst.second = get<int>(retValueFirst.second) * get<float>(retValueNext.second);
					floatWasUsed = true;
				}
				else {
					retValueFirst.second = get<int>(retValueFirst.second) / get<float>(retValueNext.second);
					floatWasUsed = true;
				}
			}
			else {
				throw("Nie mozna mnozyc/dzielic dwoch roznych typow");
			}
		}
		else if (isFloat(retValueFirst.first)) {
			if (isInt(retValueNext.first)) {
				if (isMultiplication) {
					retValueFirst.second = get<float>(retValueFirst.second) * get<int>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<float>(retValueFirst.second) / get<int>(retValueNext.second);
				}
			}
			else if (isFloat(retValueNext.first)) {
				if (isMultiplication) {
					retValueFirst.second = get<float>(retValueFirst.second) * get<float>(retValueNext.second);
				}
				else {
					retValueFirst.second = get<float>(retValueFirst.second) / get<float>(retValueNext.second);
				}
			}
			else {
				throw("Nie mozna mnozyc/dzielic dwoch roznych typow");
			}
		}
		else {
			throw("Mnozyc/Dzielic mozna tylko Inty i Floaty ");
		}
	}

	if (floatWasUsed)
		stack.setReturnedValue(make_pair(TokenTypes::FLOAT, retValueFirst.second));
	else
		stack.setReturnedValue(retValueFirst);
}
