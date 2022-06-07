#pragma once
#include "..\Statements\Statement.h"
#include "ComplexExpression.h"
#include "Reference.h"

class ArgumentList : public Statement
{
public:
	ArgumentList(std::vector<std::variant<Reference, UniqueComplexExpression>>& list)
	{
		_list = move(list);
	}
	void execute(Stack& stack);
	std::vector<std::variant<Reference, UniqueComplexExpression>>& getArguments();
private:
	std::vector<std::variant<Reference, UniqueComplexExpression>> _list;
};

