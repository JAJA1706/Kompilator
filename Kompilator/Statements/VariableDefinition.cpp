#include "VariableDefinition.h"

void VariableDefinition::execute(Stack& stack)
{
	stack.defineVariable(_leftIdentifier, _variableType);
	if (_assignStatement) {
		_assignStatement->execute(stack);
		returnValue retValue = stack.getReturnedValue();
		if (!compareTypes(retValue.first, _variableType))
			throw("Proba przypisania nieodpowiedniego typu do " + _leftIdentifier);

		stack.setVariableValue(_leftIdentifier, retValue.second);
	}
	else {
		switch (_variableType) {
		case TokenTypes::INTEGER:
			stack.setVariableValue(_leftIdentifier, 0);
			break;
		case TokenTypes::FLOAT:
			stack.setVariableValue(_leftIdentifier, float(0));
			break;
		case TokenTypes::STRING:
			stack.setVariableValue(_leftIdentifier, "");
			break;
		case TokenTypes::FUNCTION:
			stack.setVariableValue(_leftIdentifier, nullptr);
			break;
		default:
			throw("Nie powinno Cie tu byc");
		}
	}
}