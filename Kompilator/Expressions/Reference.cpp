#include "Reference.h"

void Reference::execute(Stack& stack)
{
	stack.setReturnedValue(make_pair(TokenTypes::REFERENCE, _identifier));
}

std::string Reference::getIdentifier()
{
	return _identifier;
}
