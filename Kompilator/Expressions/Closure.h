#pragma once
#include "..\Statements\Statement.h"
#include "Reference.h"

class Closure : public Statement
{
public:
	Closure(std::vector<std::variant<Reference, std::string>> list)
	{
		_list = list;
	}
	void execute(Stack& stack);
	std::vector<std::variant<Reference, std::string>> getClosure();
private:
	std::vector<std::variant<Reference, std::string>> _list;
};

