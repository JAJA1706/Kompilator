#pragma once
#include "..\Statements\Statement.h"
#include "Lambda.h"

class Element : public Statement
{
public:
	Element(std::variant<int, float, std::string> var, UniqueLambda& lambda)
	{
		_var = var;
		_lambda = move(lambda);
	}
	void execute(Stack& stack);

private:
	std::variant<int, float, std::string> _var;
	UniqueLambda _lambda;
};

