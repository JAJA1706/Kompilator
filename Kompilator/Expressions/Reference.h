#pragma once
#include "..\Statements\Statement.h"

class Reference : public Statement
{
public:
	Reference(std::string ident) : _identifier(ident) {}
	void execute(Stack& stack);
	std::string getIdentifier();
private:
	std::string _identifier;
};

