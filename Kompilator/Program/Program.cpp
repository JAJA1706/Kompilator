#include "Program.h"
#include "..\Statements\FunctionDefinition.h"
#include "..\Statements\VariableDefinition.h"

using namespace std;
Stack Program::_globalStack = Stack();

Program::Program(vector<UniqueVariableDefinition> variables, vector<UniqueFunctionDefinition> functions, vector<UniqueStatement> statements) {
	_variables = move(variables);
	_functions = move(functions);
	_mainFunctionStatements = move(statements);
}

FunctionDefinition* Program::getFunction(int index) {
	return _functions[index].get();
}

Statement* Program::getStatement(int index) {
	return _mainFunctionStatements[index].get();
}

Stack Program::getStack()
{
	return _stack;
}

int Program::execute()
{
	for (int i = 0; i < _functions.size(); ++i) {
		_globalStack.defineFunction(_functions[i]->getIdent(), _functions[i].get());
	}

	for (int i = 0; i < _variables.size(); ++i) {
		_variables[i]->execute(_globalStack);
	}
	_stack = _globalStack;

	for (int i = 0; i < _mainFunctionStatements.size() && !_stack.returnWasCalled; ++i) {
		_mainFunctionStatements[i]->execute(_stack);
	}

	return 0;
}
