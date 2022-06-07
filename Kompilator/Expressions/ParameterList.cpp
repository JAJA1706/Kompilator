#include "ParameterList.h"

void ParameterList::execute(Stack& stack)
{
}

std::vector<TokenTypes> ParameterList::getTypes()
{
	return _variableTypes;
}

std::vector<std::variant<Reference, std::string>> ParameterList::getParameters()
{
	return _list;
}
