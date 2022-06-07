#ifndef VARIABLE_DEFINITION_H
#define VARIABLE_DEFINITION_H
#include "Statement.h"
#include "AssignStatement.h"

class VariableDefinition : public Statement
{
public:
	VariableDefinition(TokenTypes variableType, std::string leftIdentifier, UniqueAssignStatement assignStatement)
	{
		_variableType = variableType;
		_leftIdentifier = leftIdentifier;
		_assignStatement = move(assignStatement);
	}
	void execute(Stack& stack);

private:
	TokenTypes _variableType;
	std::string _leftIdentifier;
	UniqueAssignStatement _assignStatement;
};

#endif