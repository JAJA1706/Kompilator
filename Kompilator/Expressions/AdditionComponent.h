#pragma once
#include "..\Statements\Statement.h"
#include "MultiplyComponent.h"

class AdditionComponent : public Statement
{
public:
	AdditionComponent(UniqueMultiplyComponent& leftMultiComponent, std::vector<bool> multiOperators, std::vector<UniqueMultiplyComponent>& rightMultiComponent)
	{
		_leftMultiComponent = move(leftMultiComponent);
		_multiOperators = multiOperators;
		_rightMultiComponents = move(rightMultiComponent);
	}
	void execute(Stack& stack);

private:
	UniqueMultiplyComponent _leftMultiComponent;
	std::vector<bool> _multiOperators;
	std::vector < UniqueMultiplyComponent> _rightMultiComponents;
};

