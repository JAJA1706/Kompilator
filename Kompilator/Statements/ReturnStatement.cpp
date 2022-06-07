#include "ReturnStatement.h"

void ReturnStatement::execute(Stack& stack)
{
	if (!_complexExpression) {
		stack.returnWasCalled = true;
		stack.setReturnedValue(std::make_pair(TokenTypes::UNKNOWN, 0));
		return;
	}

	_complexExpression->execute(stack);
	stack.returnWasCalled = true;
}
