#pragma once
#include "..\Statements\Statement.h"
#include "..\Statements\CallStatement.h"
#include "ComplexExpression.h"
#include "Element.h"

class MultiplyComponent : public Statement
{
public:
	MultiplyComponent(UniqueElement& element, std::string identifier, UniqueCallStatement callStatement, UniqueComplexExpression expression)
	{
		_element = move(element);
		_identifier = identifier;
		_callStatement = move(callStatement);
		_complexExpression = move(expression);
	}
	void execute(Stack& stack);

private:
	UniqueElement _element;
	std::string _identifier;
	UniqueCallStatement _callStatement;
	UniqueComplexExpression _complexExpression;
};

