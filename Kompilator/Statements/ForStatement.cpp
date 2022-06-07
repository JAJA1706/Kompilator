#include "ForStatement.h"

using namespace std;

void ForStatement::execute(Stack& stack)
{
	if( TokenTypes::UNKNOWN == stack.getVariableDefinition(_identifier) )
		throw("Uzyto niezadeklarowanej zmiennej w petli FOR");

	Stack forStack(stack);
	variant<int, float, string, Lambda*> variableValue;
	returnValue range;
	bool isPassed;

	do {
		isPassed = false;
		variableValue = forStack.getVariableValue(_identifier);
		int* currentVariableValue = get_if<int>(&variableValue);
		if (currentVariableValue == nullptr)
			throw("W petli FOR mozna uzyc tylko INTEGERA");

		_range->execute(forStack);
		range = forStack.getReturnedValue();

		if (int* val = get_if<int>(&range.second)) {
			if (*currentVariableValue < *val) {
				isPassed = true;
			}
		}
		else if (float* val = get_if<float>(&range.second)) {
			if (*currentVariableValue < *val) {
				isPassed = true;
			}
		}

		if (isPassed) {
			for (int i = 0; i < _forStatements.size() && !forStack.returnWasCalled; ++i) {
				_forStatements[i]->execute(forStack);
			}
		}

		if (!forStack.returnWasCalled && isPassed) {
			int nextIteration = get<int>(forStack.getVariableValue(_identifier));
			forStack.setVariableValue(_identifier, ++nextIteration);
		}

	} while (isPassed && !forStack.returnWasCalled);

	vector<string> referencedVariables = stack.getAllVariableNames();
	for (int i = 0; i < referencedVariables.size(); ++i) {
		stack.setVariableValue(referencedVariables[i], forStack.getVariableValue(referencedVariables[i]));
	}

	if (forStack.returnWasCalled) {
		stack.returnWasCalled = true;
		stack.setReturnedValue(forStack.getReturnedValue());
	}
}

void ForStatement::setIdent(std::string ident)
{
	_identifier = ident;
}
