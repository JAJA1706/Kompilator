#include "Lambda.h"
#include "..\Program\Program.h"
#include "..\Statements\VariableDefinition.h"

using namespace std;

void Lambda::execute(Stack& stack, std::vector<passedValue> passedArguments)
{
	if (_isCalled || _alwaysCalled) {
		vector<passedValue> arguments;
		if (!_arguments) {
			arguments = passedArguments;
		}
		else {
			arguments = CallStatement::visitArgumentList(_arguments, stack);
		}

		int parametersNumber = _parameters->getParameters().size();
		if (arguments.size() != parametersNumber)
			throw("Liczba argumentow w wywolaniu lambdy rozni sie od liczby parametrow");

		vector<pair<string, string>> referencedVariables;
		Stack lambdaStack(Program::_globalStack);

		vector<variant<Reference, string>> closure;
		if (_closure) {
			closure = _closure->getClosure();
		}
		for (int i = 0; i < closure.size(); ++i) {
			string identifier;
			if (Reference* reference = get_if<Reference>(&closure[i])) {
				identifier = reference->getIdentifier();
				referencedVariables.push_back(make_pair(identifier, identifier));
			}
			else if (string* visitString = get_if<string>(&closure[i])) {
				identifier = *visitString;
			}

			TokenTypes variableType = stack.getVariableDefinition(identifier);
			if (variableType == TokenTypes::UNKNOWN)
				throw("W domknieciu lambdy podano nie istniejaca zmienna");
			lambdaStack.defineVariable(identifier, variableType);
			lambdaStack.setVariableValue(identifier, stack.getVariableValue(identifier));
		}

		for (int i = 0; i < parametersNumber; ++i) {
			TokenTypes parameterType = _parameters->getTypes()[i];
			Reference* ref;

			if (!Statement::compareTypes(parameterType, arguments[i].first)) {
				if (ref = get_if<Reference>(&_parameters->getParameters()[i])) {
					string argumentName = get<string>(arguments[i].second);
					string parameterName = get<Reference>(_parameters->getParameters()[i]).getIdentifier();

					referencedVariables.push_back(make_pair(argumentName, parameterName));

					lambdaStack.defineVariable(parameterName, parameterType);
					variant<int, float, string, Lambda*> referenceValue = stack.getVariableValue(argumentName);
					lambdaStack.setVariableValue(parameterName, referenceValue);
				}
				else {
					throw("Przekazany argument nie jest tego samego typu co parametr");
				}
			}
			else {
				string parameterName = get<string>(_parameters->getParameters()[i]);
				lambdaStack.defineVariable(parameterName, parameterType);
				lambdaStack.setVariableValue(parameterName, arguments[i].second);
			}
		}

		for (int i = 0; i < _statements.size() && !lambdaStack.returnWasCalled; ++i) {
			_statements[i]->execute(lambdaStack);
		}

		for (int i = 0; i < referencedVariables.size(); ++i) {
			variant<int, float, string, Lambda*> value = lambdaStack.getVariableValue(referencedVariables[i].second);
			stack.setVariableValue(referencedVariables[i].first, value);
		}

		if (!lambdaStack.returnWasCalled)
			stack.setReturnedValue(make_pair(TokenTypes::UNKNOWN, 0));
		else
			stack.setReturnedValue(lambdaStack.getReturnedValue());

		_isCalled = false;
	}
	else{
		stack.setReturnedValue(make_pair(TokenTypes::FUNCTION, this));
	}

}

void Lambda::setIsCalled(bool isCalled)
{
	_isCalled = isCalled;
}
