#ifndef LEKSER_H
#define LEKSER_H
#include <map>
#include "Source.h"
#include "Token.h"

class Lekser
{
private:
	UniqueSource _source;
	char currentChar;
	static const std::map<std::string, TokenTypes> keyWordMap;
	static const std::map<char, TokenTypes> separatorMap;
	static const std::map<std::string, TokenTypes> operatorMap;

	const int MAX_STRING_LEN = 64;

	void skipWhiteSpace();
	Token buildComment();
	Token buildNumber();
	Token buildString();
	Token buildIdentifierOrKeyword();
	Token buildSeparator();
	Token buildOperator();
	TokenTypes checkForKeyWord(std::string value) const;

public:
	Lekser(UniqueSource source);
	Token getNextToken();
	int getPosition();
};

#endif