#pragma once
#include "..\Statements\Statement.h"
#include "Reference.h"

class ParameterList : public Statement
{
public:
	ParameterList(std::vector<TokenTypes> variableTypes, std::vector<std::variant<Reference, std::string>> list)
	{
		_variableTypes = variableTypes;
		_list = list;
	}
	void execute(Stack& stack);
	std::vector<TokenTypes> getTypes();
	std::vector<std::variant<Reference, std::string>> getParameters();
private:
	std::vector<TokenTypes> _variableTypes;
	std::vector<std::variant<Reference, std::string>> _list;
};

