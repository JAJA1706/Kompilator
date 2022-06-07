#include <iostream>
#include <cmath>
#include "Lekser.h"

using namespace std;

Lekser::Lekser(UniqueSource source) {
	_source = move(source);
	currentChar = _source->getNextChar();
}

Token Lekser::getNextToken() {
	Token token;

	skipWhiteSpace();
	if (currentChar == EOF)
		return Token(TokenTypes::END_OF_FILE, TokenCategory::OTHER, "", getPosition());

	token = buildComment();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;
	
	token = buildNumber();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;

	token = buildString();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;

	token = buildIdentifierOrKeyword();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;

	token = buildSeparator();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;

	token = buildOperator();
	if (token.getType() != TokenTypes::UNKNOWN)
		return token;

	return Token();
}

int Lekser::getPosition()
{
	return _source->getPosition();
}

void Lekser::skipWhiteSpace() {
	while ( isspace(currentChar) ) {
		currentChar = _source->getNextChar();
	}
}

Token Lekser::buildComment() {
	if (currentChar != '/')
		return Token();

	char peek = _source->peekNextChar();
	if (peek == '/') {
		string value;

		while (currentChar != '\n' && currentChar != EOF) {
			value.push_back(currentChar);
			currentChar = _source->getNextChar();
			if (value.length() > MAX_STRING_LEN) {
				value.erase(0, 2);
				return Token(TokenTypes::COMMENT, TokenCategory::CONSTS, value, getPosition(), "", "Przekroczono maksymalna dlugosc stringa!");
			}
		}

		currentChar = _source->getNextChar();
		value.erase(0, 2);
		return Token(TokenTypes::COMMENT, TokenCategory::SEPARATOR, value, getPosition());
	}	

	return Token();
}

Token Lekser::buildNumber() {
	if (!isdigit(currentChar))
		return Token();

	long long value;
	bool isNumberTooBig = false;
	value = 0;///*(long long)*/currentChar - '0';
	//currentChar = _source->getNextChar();
	while (isdigit(currentChar) && currentChar != EOF) {
		value = value * 10 + currentChar - '0';
		currentChar = _source->getNextChar();

		if (currentChar == '.') {
			int digitsAfterDot = 0;
			currentChar = _source->getNextChar();

			while (isdigit(currentChar) && currentChar != EOF) {
				value = value * 10 + currentChar - '0';
				currentChar = _source->getNextChar();
				++digitsAfterDot;
			}

			double floatValue;
			floatValue = value / pow(10, digitsAfterDot);
			return Token(TokenTypes::CONST_FLOAT, TokenCategory::CONSTS, (float)floatValue, getPosition());
		}

		if (value > std::numeric_limits<int>::max()) {
			isNumberTooBig = true;
		}
	}

	int intValue = value;
	if (isNumberTooBig)
		return Token(TokenTypes::CONST_INT, TokenCategory::CONSTS, intValue, getPosition(), "Liczba byla za duza, przekroczona limit int'a");

	return Token(TokenTypes::CONST_INT, TokenCategory::CONSTS, intValue, getPosition());
}

Token Lekser::buildString() {
	if (currentChar != '\"')
		return Token();

	string value = "";
	currentChar = _source->getNextChar();
	while (currentChar != '\"' ) {
		if(currentChar == EOF)
			return Token(TokenTypes::UNKNOWN, TokenCategory::OTHER, value, getPosition(), "Nie zamknieto stringu");
		value.push_back(currentChar);
		currentChar = _source->getNextChar();
		if (value.length() > MAX_STRING_LEN)
			return Token(TokenTypes::CONST_STRING, TokenCategory::CONSTS, value, getPosition(),"","Przekroczono maksymalna dlugosc stringa!");
	}

	currentChar = _source->getNextChar();
	return Token(TokenTypes::CONST_STRING, TokenCategory::CONSTS, value, getPosition());
}

Token Lekser::buildIdentifierOrKeyword(){
	if( currentChar != '_' && !isalpha(currentChar) )
		return Token();

	string value = "";
	value.push_back(currentChar);
	currentChar = _source->getNextChar();

	while (currentChar == '_' || isalnum(currentChar) && currentChar != EOF) {
		value.push_back(currentChar);
		currentChar = _source->getNextChar();
		if (value.length() > MAX_STRING_LEN)
			return Token(TokenTypes::CONST_STRING, TokenCategory::CONSTS, value, getPosition(), "", "Przekroczono maksymalna dlugosc stringa!");
	}

	TokenTypes keyWordType = checkForKeyWord(value);
	if( keyWordType != TokenTypes::UNKNOWN )
		return Token(keyWordType, TokenCategory::KEYWORD, value, getPosition());

	string upperCaseValue = "";
	for (int i = 0; i < value.length(); ++i) {
		char ch = toupper(value[i]);
		upperCaseValue.push_back(ch);
	}
		
	keyWordType = checkForKeyWord(upperCaseValue);
	if (keyWordType != TokenTypes::UNKNOWN)
		return Token(keyWordType, TokenCategory::KEYWORD, upperCaseValue, getPosition());

	return Token(TokenTypes::IDENTIFIER, TokenCategory::IDENTIFIER, value, getPosition());
}

TokenTypes Lekser::checkForKeyWord(string value) const{
	map<string, TokenTypes>::const_iterator iter;
	iter = keyWordMap.find(value);

	if (iter != keyWordMap.end())
		return iter->second;
	else
		return TokenTypes::UNKNOWN;
}

Token Lekser::buildSeparator(){
	map<char, TokenTypes>::const_iterator iterSepar;
	iterSepar = separatorMap.find(currentChar);

	if (iterSepar != separatorMap.end()) {
		string value = "";
		value.push_back(currentChar);
		currentChar = _source->getNextChar();
		return Token(iterSepar->second, TokenCategory::SEPARATOR, value, getPosition());
	}

	return Token();
}

Token Lekser::buildOperator() {
	string value = "";
	value.push_back(currentChar);
	map<string, TokenTypes>::const_iterator iterOperSingle, iterOperDouble;

	iterOperSingle = operatorMap.find(value);
	if (iterOperSingle != operatorMap.end()) {
		currentChar = _source->getNextChar();
		value.push_back(currentChar);
		iterOperDouble = operatorMap.find(value);

		if(iterOperDouble != operatorMap.end()){
			currentChar = _source->getNextChar();
			return Token(iterOperDouble->second, TokenCategory::COMPARISION_OPERATOR, iterOperDouble->first, getPosition());
		}
		else if (iterOperSingle->first == ">" || iterOperSingle->first == "<")
			return Token(iterOperSingle->second, TokenCategory::COMPARISION_OPERATOR, iterOperSingle->first, getPosition());
		else
			return Token(iterOperSingle->second, TokenCategory::OPERATOR, iterOperSingle->first, getPosition());
	}

	return Token();
}

const map<string, TokenTypes> Lekser::keyWordMap = { {"START", TokenTypes::START},
													 {"STOP", TokenTypes::STOP},
													 {"DEF", TokenTypes::DEF},
													 {"END", TokenTypes::END},
													 {"return", TokenTypes::RETURN},
													 {"IF", TokenTypes::IF},
													 {"ELSE", TokenTypes::ELSE},
													 {"FOR", TokenTypes::FOR},
													 {"IN", TokenTypes::IN},
													 {"RANGE", TokenTypes::RANGE},
													 {"WHILE", TokenTypes::WHILE},
													 {"int", TokenTypes::INTEGER},
													 {"float", TokenTypes::FLOAT},
													 {"string", TokenTypes::STRING},
													 {"function", TokenTypes::FUNCTION},
													 {"ref", TokenTypes::REFERENCE},
													 {"lambda", TokenTypes::LAMBDA} };

const map<char, TokenTypes> Lekser::separatorMap = { {'(', TokenTypes::OPEN_ROUND_BRACKET},
													 {')', TokenTypes::CLOSED_ROUND_BRACKET},
													 {'[', TokenTypes::OPEN_SQUARE_BRACKET},
													 {']', TokenTypes::CLOSED_SQUARE_BRACKET},
													 {';', TokenTypes::SEMICOLON},
													 {',', TokenTypes::COMMA} };

const map<string, TokenTypes> Lekser::operatorMap = {{"=", TokenTypes::ASSIGN},
													 {"!", TokenTypes::NEGATION},
													 {"+", TokenTypes::PLUS},
													 {"-", TokenTypes::MINUS},
													 {"*", TokenTypes::MULTIPLICATION},
													 {"/", TokenTypes::DIVISION},
													 {">", TokenTypes::GREATER},
													 {"<", TokenTypes::SMALLER},
													 {"==", TokenTypes::EQUAL},
													 {"!=", TokenTypes::UNEQUAL},
													 {">=", TokenTypes::NOT_SMALLER},
													 {"<=", TokenTypes::NOT_GREATER} };