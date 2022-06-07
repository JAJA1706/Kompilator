#include "ArgumentList.h"

void ArgumentList::execute(Stack& stack) {}

std::vector<std::variant<Reference, UniqueComplexExpression>>& ArgumentList::getArguments()
{
	return _list;
}
