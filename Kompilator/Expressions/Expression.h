#pragma once
#include "..\Statements\Statement.h"
#include "AdditionComponent.h"

class Expression : public Statement
{
public:
	Expression(bool isPositive, UniqueAdditionComponent& leftAddComponent, std::vector<bool> additionOperators, std::vector<UniqueAdditionComponent>& rightComponents)
	{
		_isPositive = isPositive;
		_leftAddComponent = move(leftAddComponent);
		_additionOperators = additionOperators;
		_rightComponents = move(rightComponents);
	}
	void execute(Stack& stack);
private:
	bool _isPositive;
	UniqueAdditionComponent _leftAddComponent;
	std::vector<bool> _additionOperators;
	std::vector<UniqueAdditionComponent> _rightComponents;
};

