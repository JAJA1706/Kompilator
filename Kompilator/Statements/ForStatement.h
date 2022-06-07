#pragma once
#include "Statement.h"
#include "..\Expressions\ComplexExpression.h"

class ForStatement : public Statement
{
public:
	ForStatement(std::string identifier, UniqueComplexExpression range, std::vector<UniqueStatement> statements)
	{
		_identifier = identifier;
		_range = move(range);
		_forStatements = move(statements);
	}
	void execute(Stack& stack);
	void setIdent(std::string ident);

private:
	std::string _identifier;
	UniqueComplexExpression _range;
	std::vector<UniqueStatement> _forStatements;
};