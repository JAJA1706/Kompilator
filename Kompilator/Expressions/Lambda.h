#pragma once
#include "..\Statements\Statement.h"
#include "ArgumentList.h"
#include "Closure.h"
#include "ParameterList.h"

class Lambda
{
public:
	Lambda(UniqueClosure closure, UniqueParameterList parameters, std::vector<UniqueStatement> statements, bool isCalled, UniqueArgumentList arguments)
	{
		_closure = move(closure);
		_parameters = move(parameters);
		_statements = move(statements);
		_isCalled = isCalled;
		_alwaysCalled = isCalled;
		_arguments = move(arguments);
	}
	void execute(Stack& stack, std::vector<passedValue> arguments = std::vector<passedValue>() );
	void setIsCalled(bool isCalled);

private:
	UniqueClosure _closure;
	UniqueParameterList _parameters;
	std::vector<UniqueStatement> _statements;
	bool _isCalled;
	bool _alwaysCalled;
	UniqueArgumentList _arguments;

};

