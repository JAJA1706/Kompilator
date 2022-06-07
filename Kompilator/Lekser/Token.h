#ifndef TOKEN_H
#define TOKEN_H
#include <variant>
#include <string>
#include <memory>
#include "TokenTypes.h"


class Token {
private:
	TokenTypes _type;
	TokenCategory _category;
	std::variant<int, float, std::string> _value;
	int _position;
	std::string _warning;
	std::string _error;
public:
	Token(TokenTypes type = TokenTypes::UNKNOWN, TokenCategory category = TokenCategory::OTHER, std::variant<int, float, std::string> value = "", int position = 0, std::string warning = "", std::string error = "" );
	TokenTypes getType() const;
	TokenCategory getCategory() const;
	const std::variant<int, float, std::string>& getValue() const;
	const std::string getWarning() const;
	const std::string getError() const;
	const int getPosition() const;

};

#endif

