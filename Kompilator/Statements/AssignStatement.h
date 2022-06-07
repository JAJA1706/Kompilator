#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H
#include "Statement.h"
#include "..\Expressions\ComplexExpression.h"

class AssignStatement : public Statement
{
public:
	AssignStatement(UniqueComplexExpression expression)
	{
		_identifier = "";
		_complexExpression = move(expression);
	}
	void execute(Stack& stack);
	void setIdent(std::string ident);

private:
	std::string _identifier;
	UniqueComplexExpression _complexExpression;
};
#endif