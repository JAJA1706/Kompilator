#include "Closure.h"

void Closure::execute(Stack& stack)
{
}

std::vector<std::variant<Reference, std::string>> Closure::getClosure()
{
	return _list;
}
