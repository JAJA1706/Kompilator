#include "FunctionDefinition.h"
#include "..\Program\Program.h"
#include "..\Statements\VariableDefinition.h"

using namespace std;

void FunctionDefinition::execute(Stack& stack, std::vector<passedValue> arguments)
{
	int parametersNumber = _parameterList->getParameters().size();
	if (arguments.size() != parametersNumber)
		throw("Liczba argumentow w wywolaniu funkcji " + _identifier + " rozni sie od liczby parametrow");

	vector<pair<string, string>> referencedVariables;
	Stack functionStack(Program::_globalStack);
	for (int i = 0; i < parametersNumber; ++i) {
		TokenTypes parameterType = _parameterList->getTypes()[i];
		Reference* ref;

		if ( !Statement::compareTypes(parameterType, arguments[i].first)) {
			if (ref = get_if<Reference>(&_parameterList->getParameters()[i])) {
				string argumentName = get<string>(arguments[i].second);
				string parameterName = get<Reference>(_parameterList->getParameters()[i]).getIdentifier();

				referencedVariables.push_back(make_pair(argumentName, parameterName));

				functionStack.defineVariable(parameterName, parameterType);
				variant<int, float, string, Lambda*> referenceValue = stack.getVariableValue(argumentName);
				functionStack.setVariableValue(parameterName, referenceValue);
			}
			else {
				throw("Przekazany argument nie jest tego samego typu co parametr");
			}
		}
		else {
			string parameterName = get<string>(_parameterList->getParameters()[i]);
			functionStack.defineVariable(parameterName, parameterType);
			functionStack.setVariableValue(parameterName, arguments[i].second);
		}
	}
	
	for (int i = 0; i < _statements.size() && !functionStack.returnWasCalled; ++i) {
		_statements[i]->execute(functionStack);
	}

	for (int i = 0; i < referencedVariables.size(); ++i) {
		variant<int, float, string, Lambda*> value = functionStack.getVariableValue( referencedVariables[i].second );
		stack.setVariableValue(referencedVariables[i].first, value);
	}

	if (!functionStack.returnWasCalled)
		stack.setReturnedValue(make_pair(TokenTypes::UNKNOWN, 0));
	else
		stack.setReturnedValue(functionStack.getReturnedValue());
}

std::string FunctionDefinition::getIdent()
{
	return _identifier;
}
