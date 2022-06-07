#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "..\Expressions\ComplexExpression.h"
class IfStatement : public Statement
{
public:
	IfStatement(UniqueComplexExpression& exp, std::vector<UniqueStatement>& ifStatements, std::vector<UniqueStatement>& elseStatements) : _complexExpression(nullptr)
	{
		_complexExpression = std::move(exp);
		_ifStatements = std::move(ifStatements);
		_elseStatements = std::move(elseStatements);
	}
	void execute(Stack& stack);

private:
	UniqueComplexExpression _complexExpression;
	std::vector<UniqueStatement> _ifStatements;
	std::vector<UniqueStatement> _elseStatements;
};
#endif