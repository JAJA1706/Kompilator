#include <iostream>
#include "CallStatement.h"
#include "..\Expressions\ArgumentList.h"

using namespace std;

void CallStatement::execute(Stack& stack)
{
	FunctionDefinition* function;
	Lambda* lambda;
	std::vector<returnValue> passedValues = visitArgumentList(_argumentList, stack);

	function = stack.getFunctionDefinition(_identifier);
	TokenTypes lambdaType = stack.getVariableDefinition(_identifier);

	if (function) {
		function->execute(stack, passedValues);
	}
	else if( lambdaType == TokenTypes::FUNCTION){
		lambda = get<Lambda*>(stack.getVariableValue(_identifier));
		lambda->setIsCalled(true);
		lambda->execute(stack, passedValues);
	}
	else if (_identifier == "print") {
		printValues(passedValues);
	}
	else {
		throw ("Proba odwolanie sie do nieistniejacej funkcji");
	}
}

void CallStatement::setIdent(std::string ident)
{
	_identifier = ident;
}


vector<passedValue> CallStatement::visitArgumentList(UniqueArgumentList& _argumentList, Stack& stack)
{
	vector<passedValue> passedValues;
	for (int i = 0; i < _argumentList->getArguments().size(); ++i) {
		returnValue retValue;
		Reference* reference;
		UniqueComplexExpression* complexExpression;
		if (reference = get_if<Reference>(&_argumentList->getArguments()[i])) {
			reference->execute(stack);
			retValue = stack.getReturnedValue();
		}
		else if (complexExpression = get_if<UniqueComplexExpression>(&_argumentList->getArguments()[i])) {
			(*complexExpression)->execute(stack);
			retValue = stack.getReturnedValue();
		}
		else {
			throw("Nie powinno Cie tu byc...");
		}
		passedValues.push_back(retValue);
	}

	return passedValues;
}

void CallStatement::printValues(std::vector<passedValue> values)
{
	for (int i = 0; i < values.size(); ++i) {
		visit([](auto&& arg) { cout << arg << " "; }, values[i].second);
	}
	cout << "\n";
}
