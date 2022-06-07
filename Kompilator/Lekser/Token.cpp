#include "Token.h"

Token::Token(TokenTypes type, TokenCategory category, std::variant<int, float, std::string> value, int position, std::string warning, std::string error)
	: _type(type), _category(category), _value(value), _position(position), _warning(warning), _error(error) {};

TokenTypes Token::getType() const {
	return _type;
}

TokenCategory Token::getCategory() const {
	return _category;
}
const std::variant<int, float, std::string>& Token::getValue() const {
	return _value;
}

const std::string Token::getWarning() const{
	return std::string();
}

const std::string Token::getError() const{
	return std::string();
}

const int Token::getPosition() const
{
	return _position;
}
