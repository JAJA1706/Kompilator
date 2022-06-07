#pragma once
#include "Statement.h"
#include "FunctionDefinition.h"

class CallStatement : public Statement
{
public:
	CallStatement(UniqueArgumentList& argumentList)
	{
		_argumentList = move(argumentList);
	}
	void execute(Stack& stack);
	void setIdent(std::string ident);
	static std::vector<passedValue> visitArgumentList(UniqueArgumentList& _argumentList, Stack& stack);
private:
	void printValues(std::vector<passedValue> values);
	std::string _identifier;
	UniqueArgumentList _argumentList;
};