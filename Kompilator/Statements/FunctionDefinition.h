#pragma once
#include "Statement.h"
#include "..\Expressions\ParameterList.h"
#include "..\Expressions\ArgumentList.h"

class FunctionDefinition
{
public:
	FunctionDefinition(std::string identifier, UniqueParameterList parameterList, std::vector<UniqueStatement> statements)
	{
		_identifier = identifier;
		_parameterList = move(parameterList);
		_statements = move(statements);
	}
	void execute(Stack& stack, std::vector<passedValue> arguments);
	std::string getIdent();

private:
	std::string _identifier;
	UniqueParameterList _parameterList;
	std::vector<UniqueStatement> _statements;
};