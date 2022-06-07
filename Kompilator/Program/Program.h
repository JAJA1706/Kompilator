#pragma once
#include <vector>
#include "..\Statements\Statement.h"

class Program
{
public:
	Program(std::vector<UniqueVariableDefinition> variables, std::vector<UniqueFunctionDefinition> functions, std::vector<UniqueStatement> statements);
	~Program() { _globalStack = Stack(); }
	FunctionDefinition* getFunction(int index);
	Statement* getStatement(int index);
	Stack getStack();
	int execute();


	static Stack _globalStack;
private:
	Stack _stack;
	std::vector<UniqueVariableDefinition> _variables;
	std::vector<UniqueFunctionDefinition> _functions;
	std::vector<UniqueStatement> _mainFunctionStatements;


};

