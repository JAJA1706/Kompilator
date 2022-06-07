#include "Stack.h"
#include <algorithm>
#include <type_traits>
#include "..\Statements\FunctionDefinition.h"
#include "..\Statements\VariableDefinition.h"
#include "Program.h"

using namespace std;

Stack::Stack() {
	returnWasCalled = false;
}

void Stack::defineFunction(string name, FunctionDefinition* function)
{
	_functionDefinitions.insert(make_pair(name, function));
}

void Stack::defineVariable(string name, TokenTypes variableType)
{
	_variableDefinitions.insert(make_pair(name, variableType));
}

void Stack::setVariableValue(string name, variant<int, float, string, Lambda*> value)
{
	//stara dobra petla for jest duzo czytelniejsza :(
	auto iter = find_if(_variableValues.begin(), _variableValues.end(), [name](pair<string, variant<int, float, string, Lambda*>> x)
		{return x.first == name; });
	if (iter != _variableValues.end()) {
		checkTypeCompatibility(name, value);
		iter->second = value;
	}
	else {
		checkTypeCompatibility(name, value);
		_variableValues.push_back(make_pair(name, value));
	}

}

std::variant<int, float, std::string, Lambda*> Stack::getVariableValue(std::string name)
{
	auto iter = find_if(_variableValues.begin(), _variableValues.end(), [name](pair<string, variant<int, float, string, Lambda*>> x)
		{return x.first == name; });
	if (iter != _variableValues.end()) {
		return iter->second;
	}
	else{
		throw("Proba uzycia niezadeklarowanej zmiennej");
	}
}

TokenTypes Stack::getVariableDefinition(string name)
{
	unordered_map<string, TokenTypes>::iterator iter;
	iter = _variableDefinitions.find(name);
	if ( iter == _variableDefinitions.end() )
		return TokenTypes::UNKNOWN;
	else
		return iter->second;
}

vector<string> Stack::getAllVariableNames()
{
	vector<string> allVariables;
	for (auto var : _variableDefinitions)
		allVariables.push_back(var.first);

	return allVariables;
}

FunctionDefinition* Stack::getFunctionDefinition(std::string name)
{
	unordered_map<string, FunctionDefinition*>::iterator iter;
	iter = _functionDefinitions.find(name);
	if (iter == _functionDefinitions.end())
		return nullptr;
	else
		return iter->second;
}

returnValue Stack::getReturnedValue()
{
	return _returnedValue;
}

void Stack::setReturnedValue(returnValue retValue)
{
	_returnedValue = retValue;
}

void Stack::checkTypeCompatibility(string name, variant<int, float, string, Lambda*>& value)
{
	TokenTypes varType = getVariableDefinition(name);
	if (varType != TokenTypes::UNKNOWN) {
		visit([varType,&value](auto&& arg) {
			using T = decay_t<decltype(arg)>;
			if constexpr (is_same_v<T, int>) {
				if (varType != TokenTypes::INTEGER && varType != TokenTypes::CONST_INT)
					if(varType != TokenTypes::FLOAT && varType != TokenTypes::CONST_FLOAT)
						throw exception("Proba przypisania zlej wartosci do Integera");
					else {
						value = (float)arg;
					}
			}
			else if constexpr (is_same_v<T, float>) {
				if (varType != TokenTypes::FLOAT && varType != TokenTypes::CONST_FLOAT)
					if (varType != TokenTypes::INTEGER && varType != TokenTypes::CONST_INT)
						throw exception("Proba przypisania zlej wartosci do Integera");
					else {
						value = (int)arg;
					}
			}
			else if constexpr (is_same_v<T, string>) {
				if (varType != TokenTypes::STRING && varType != TokenTypes::CONST_STRING)
					throw exception("Proba przypisania zlej wartosci do Stringa");
			}
			else if constexpr (is_same_v<T, Lambda*>)
				if (varType != TokenTypes::FUNCTION)
					throw exception("Proba przypisania zlej wartosci do Lambdy"); }, value);
	}
}