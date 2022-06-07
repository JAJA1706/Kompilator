#pragma once
#include <unordered_map>
#include <vector>
#include <variant>


class FunctionDefinition;
class Lambda;
enum class TokenTypes;

typedef std::pair<TokenTypes, std::variant<int, float, std::string, Lambda*>> returnValue;
typedef returnValue passedValue;

class Stack
{
public:
	Stack();
	void defineFunction( std::string name, FunctionDefinition* function);
	void defineVariable( std::string name, TokenTypes variableType);
	void setVariableValue(std::string name, std::variant<int, float, std::string, Lambda*> value);
	std::variant<int, float, std::string, Lambda*> getVariableValue(std::string name);
	TokenTypes getVariableDefinition(std::string name);
	std::vector<std::string> getAllVariableNames();
	FunctionDefinition* getFunctionDefinition(std::string name);
	returnValue getReturnedValue();
	void setReturnedValue(returnValue retValue);

	bool returnWasCalled;

private:
	void checkTypeCompatibility(std::string name, std::variant<int, float, std::string, Lambda*>& value);

	std::unordered_map<std::string, FunctionDefinition*> _functionDefinitions;
	std::unordered_map<std::string, TokenTypes> _variableDefinitions;
	std::vector<std::pair<std::string, std::variant<int, float, std::string, Lambda*>>> _variableValues;
	std::pair<TokenTypes, std::variant<int, float, std::string, Lambda*>> _returnedValue;

	//static std::unordered_map<std::string, TokenTypes> _globalVariableDefinitions;
	//static std::vector<std::pair<std::string, std::variant<int, float, std::string, Lambda*>>> _globalVariableValues;

};

