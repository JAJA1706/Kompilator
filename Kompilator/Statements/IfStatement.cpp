#include "IfStatement.h"

using namespace std;

void IfStatement::execute(Stack& stack)
{
	_complexExpression->execute(stack);
	returnValue retVal = stack.getReturnedValue();

	if (!isNumber(retVal.first))
		throw("Spodziewano sie wartosci bool/liczbowej w instrukcji IF");

	bool isPassed = false;
	if (int* val = get_if<int>(&retVal.second)) {
		if (*val != 0) {
			isPassed = true;
		}
	}
	else if (float* val = get_if<float>(&retVal.second)) {
		if (*val != 0) {
			isPassed = true;
		}
	}

	Stack ifStack(stack);
	if (isPassed) {
		for (int i = 0; i < _ifStatements.size() && !ifStack.returnWasCalled; ++i) {
			_ifStatements[i]->execute(ifStack);
		}
	}
	else {
		for (int i = 0; i < _elseStatements.size() && !ifStack.returnWasCalled; ++i) {
			_elseStatements[i]->execute(ifStack);
		}
	}

	vector<string> referencedVariables = stack.getAllVariableNames();
	for (int i = 0; i < referencedVariables.size(); ++i) {
		stack.setVariableValue(referencedVariables[i], ifStack.getVariableValue(referencedVariables[i]));
	}

	if (ifStack.returnWasCalled) {
		stack.returnWasCalled = true;
		stack.setReturnedValue(ifStack.getReturnedValue());
	}

}
