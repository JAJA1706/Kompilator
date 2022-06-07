#include "MultiplyComponent.h"

void MultiplyComponent::execute(Stack& stack)
{
	returnValue retValue;
	if(_element){
		_element->execute(stack);
		retValue = stack.getReturnedValue();
		stack.setReturnedValue(retValue);
		return;
	}

	if (_callStatement) {
		_callStatement->execute(stack);
		return;
	}

	if (_identifier != "") {
		retValue.second = stack.getVariableValue(_identifier);
		retValue.first = stack.getVariableDefinition(_identifier);
		stack.setReturnedValue(retValue);
		return;
	}

	if (_complexExpression) {
		_complexExpression->execute(stack);
		retValue = stack.getReturnedValue();
		stack.setReturnedValue(retValue);
		return;
	}

	throw("jakim cudem to zostalo zparsowane");
}
