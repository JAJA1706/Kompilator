#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
#include "Statement.h"
#include "..\Expressions\ComplexExpression.h"

class ReturnStatement : public Statement
{
public:
	ReturnStatement(UniqueComplexExpression complexExpression)
	{
		_complexExpression = move(complexExpression);
	}

	void execute(Stack& stack);
private:
	UniqueComplexExpression _complexExpression;
};

#endif
