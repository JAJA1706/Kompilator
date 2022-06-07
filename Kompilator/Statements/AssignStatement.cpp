#include "AssignStatement.h"

void AssignStatement::execute(Stack& stack)
{
	_complexExpression->execute(stack);
	if (_identifier != "")
		stack.setVariableValue(_identifier, stack.getReturnedValue().second);
}

void AssignStatement::setIdent(std::string ident)
{
	_identifier = ident;
}
