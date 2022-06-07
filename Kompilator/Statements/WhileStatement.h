#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H
#include "..\Expressions\ComplexExpression.h"

class WhileStatement : public Statement
{
public:
	WhileStatement(UniqueComplexExpression& expression, std::vector<UniqueStatement>& statements) 
	{
		_complexExpression = move(expression);
		_whileStatements = move(statements);

	}
	void execute(Stack& stack);
private:
	UniqueComplexExpression _complexExpression;
	std::vector<UniqueStatement> _whileStatements;
};

#endif