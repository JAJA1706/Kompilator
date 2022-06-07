#include "WhileStatement.h"

using namespace std;

void WhileStatement::execute(Stack& stack)
{
	Stack whileStack(stack);
	bool isPassed;
	do {
		isPassed = false;
		_complexExpression->execute(whileStack);
		returnValue retValue = whileStack.getReturnedValue();

		if (!isNumber(retValue.first))
			throw("Spodziewano sie wartosci bool/liczbowej w instrukcji WHILE");


		if (int* val = get_if<int>(&retValue.second)) {
			if (*val != 0) {
				isPassed = true;
			}
		}
		else if (float* val = get_if<float>(&retValue.second)) {
			if (*val != 0) {
				isPassed = true;
			}
		}

		if (isPassed) {
			for (int i = 0; i < _whileStatements.size() && !whileStack.returnWasCalled; ++i) {
				_whileStatements[i]->execute(whileStack);
			}
		}
	} while (isPassed && !whileStack.returnWasCalled);

	vector<string> referencedVariables = stack.getAllVariableNames();
	for (int i = 0; i < referencedVariables.size(); ++i) {
		stack.setVariableValue(referencedVariables[i], whileStack.getVariableValue(referencedVariables[i]));
	}

	if (whileStack.returnWasCalled) {
		stack.returnWasCalled = true;
		stack.setReturnedValue(whileStack.getReturnedValue());
	}
}
