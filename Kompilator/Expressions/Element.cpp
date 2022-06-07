#include "Element.h"
#include <algorithm>
#include <type_traits>

using namespace std;

void Element::execute(Stack& stack)
{
	returnValue retValue;
	if (_lambda) {
		_lambda->execute(stack);
		retValue = stack.getReturnedValue();
		stack.setReturnedValue(retValue);
		return;
	}

	TokenTypes varType;
	visit([&varType, &retValue](auto&& arg) {
		using T = decay_t<decltype(arg)>;
		if constexpr (is_same_v<T, int>) {
			retValue.first = TokenTypes::CONST_INT;
			retValue.second = arg;
		}
		else if constexpr (is_same_v<T, float>) {
			retValue.first = TokenTypes::CONST_FLOAT;
			retValue.second = arg;
		}
		else if constexpr (is_same_v<T, string>) {
			retValue.first = TokenTypes::CONST_STRING;
			retValue.second = arg;
		}
		else {
			throw("Nie mam pojecia co zawiodlo");
		}										}, _var);

	stack.setReturnedValue(retValue);

}
