#include "Statement.h"

//jezeli sa rowne zwroc true, przyjmujemy ze int z float sa rowne
bool Statement::compareTypes(TokenTypes first, TokenTypes second) {
	if (first == TokenTypes::INTEGER || first == TokenTypes::CONST_INT || first == TokenTypes::FLOAT || first == TokenTypes::CONST_FLOAT) {
		if (second == TokenTypes::INTEGER || second == TokenTypes::CONST_INT || second == TokenTypes::FLOAT || second == TokenTypes::CONST_FLOAT) {
			return true;
		}
		else {
			return false;
		}
	}

	if (first == TokenTypes::STRING || first == TokenTypes::CONST_STRING) {
		if (second == TokenTypes::STRING || second == TokenTypes::CONST_STRING) {
			return true;
		}
		else {
			return false;
		}
	}

	if (first == TokenTypes::FUNCTION) {
		if (second == TokenTypes::FUNCTION) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

bool Statement::isInt(TokenTypes type)
{
	if (type == TokenTypes::INTEGER || type == TokenTypes::CONST_INT)
		return true;
	else
		return false;
}

bool Statement::isFloat(TokenTypes type)
{
	if (type == TokenTypes::FLOAT || type == TokenTypes::CONST_FLOAT)
		return true;
	else
		return false;
}

bool Statement::isString(TokenTypes type)
{
	if (type == TokenTypes::STRING || type == TokenTypes::CONST_STRING)
		return true;
	else
		return false;
}

bool Statement::isNumber(TokenTypes type)
{
	if (isInt(type) || isFloat(type))
		return true;
	else
		return false;
}
