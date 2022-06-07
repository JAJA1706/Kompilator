#include <string>
#include "Parser.h"
#include "ParserException.h"

using namespace std;

//Program = { definicja_funkcji | definicja_zmiennej }, "START", { działanie }, "STOP";
Program Parser::parseProgram() {
	if (_lekser == nullptr)
		throw ParserException("Niezainicjalizowany lekser", _lekser->getPosition());
	_token = _lekser->getNextToken();

	vector<UniqueVariableDefinition> vecVariables;
	vector<UniqueFunctionDefinition> vecFunctions;
	vector<UniqueStatement> vecStatements;
	UniqueVariableDefinition variable;
	UniqueFunctionDefinition function;
	UniqueStatement state;

	bool isDefinitions;
	do {
		isDefinitions = false;

		variable = parseVariableDefinition();
		if (variable != nullptr) {
			vecVariables.push_back( move(variable) );
			checkForSemicolon();
			isDefinitions = true;
		}
		function = parseFunctionDefinition();
		if (function != nullptr) {
			vecFunctions.push_back(move(function));
			isDefinitions = true;
		}
	} while (isDefinitions);

	if ( !expect(TokenTypes::START) )
		throw ParserException("Brak slowa kluczowego START", _lekser->getPosition());
	_token = _lekser->getNextToken();

	while (state = parseStatement() ) {
		vecStatements.push_back( move(state) );
	}

	if( !expect(TokenTypes::STOP) )
		throw ParserException("Brak slowa kluczowego STOP", _lekser->getPosition());

	return Program(move(vecVariables), move(vecFunctions), move(vecStatements));
}

UniqueFunctionDefinition Parser::parseFunctionDefinition() {
	if(!expect(TokenTypes::DEF))
		return nullptr;
	_token = _lekser->getNextToken();

	string identifier;
	UniqueParameterList parameterList;
	vector<UniqueStatement> statements;

	if (!expect(TokenTypes::IDENTIFIER))
		throw ParserException("Brak Identyfikatora", _lekser->getPosition());
	identifier = get<string>(_token.getValue());
	_token = _lekser->getNextToken();

	if (!expect(TokenTypes::OPEN_ROUND_BRACKET))
		throw ParserException("Nieotwarty nawias przy definicji funkcji", _lekser->getPosition());
	_token = _lekser->getNextToken();

	parameterList = parseParameterList();

	if (!expect(TokenTypes::CLOSED_ROUND_BRACKET))
		throw ParserException("Niezamkniety nawias po parametrach funkcji", _lekser->getPosition());
	_token = _lekser->getNextToken();

	UniqueStatement state;
	while (state = parseStatement()) {
		statements.push_back(move(state));
	}

	if (!expect(TokenTypes::END))
		throw ParserException("Spodziewano sie END", _lekser->getPosition());
	_token = _lekser->getNextToken();

	return make_unique<FunctionDefinition>(identifier, move(parameterList), move(statements));
}

UniqueStatement Parser::parseStatement() {
	UniqueStatement state;

	if (state = parseVariableDefinition()) {
		checkForSemicolon();
		return state;
	}
	if (state = parseAssignOrCallOrComplexExpression()) {
		checkForSemicolon();
		return state;
	}
	if (state = parseReturnFromFunction()) {
		checkForSemicolon();
		return state;
	}
	if (state = parseComplexExpression()){
		checkForSemicolon();
		return state;
	}
	if (state = parseForStatement()) {
		return state;
	}
	if (state = parseWhileStatement()) {
		return state;
	}
	if (state = parseIfStatement()) {
		return state;
	}

	return nullptr;
}

//definicja_zmiennej = typ_zmiennej, identyfikator, [przypisanie do zmiennej];
UniqueVariableDefinition Parser::parseVariableDefinition() {
	if( !expectVariableType() )
		return nullptr;
	TokenTypes varType = _token.getType();
	string leftIdent;
	bool isThereAssign = false;
	UniqueElement elem;
	string rightIdent;

	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::IDENTIFIER) )
		throw ParserException("Brak Identyfikatora", _lekser->getPosition());
	leftIdent = get<string>(_token.getValue());

	_token = _lekser->getNextToken();
	UniqueAssignStatement assignStatement = parseAssignStatement();
	return make_unique<VariableDefinition>(varType, leftIdent, move(assignStatement));
}

//przypisanie_wywołanie_wyrażenie = identyfikator, przypisanie_do_zmiennej | wywołanie_funcji | reszta_wyrażenia_złożonego;
UniqueStatement Parser::parseAssignOrCallOrComplexExpression() {
	if ( !expect(TokenTypes::IDENTIFIER) )
		return nullptr;

	string identifier = get<string>(_token.getValue());
	_token = _lekser->getNextToken();
	UniqueAssignStatement assignStatement = parseAssignStatement();
	if (assignStatement != nullptr) {
		assignStatement->setIdent(identifier);
		return assignStatement;
	}

	UniqueCallStatement callStatement = parseCallStatement();
	if (callStatement != nullptr) {
		callStatement->setIdent(identifier);
		return callStatement;
	}

	UniqueRestOfComplexExpression restOfComplexExpression = parseRestOfComplexExpression();
	if (restOfComplexExpression != nullptr) {
		restOfComplexExpression->setIdent(identifier);
		return restOfComplexExpression;
	}

	throw ParserException("Niepoprawne poprzedzenie identyfikatora", _lekser->getPosition());
}

//przypisanie_do_zmiennej = "=", wyrażenie;
UniqueAssignStatement Parser::parseAssignStatement() {
	if ( !expect(TokenTypes::ASSIGN) )
		return nullptr;

	_token = _lekser->getNextToken();
	UniqueComplexExpression complexExpression = parseComplexExpression();
	if (complexExpression == nullptr)
		throw ParserException("brak prawej strony przypisania", _lekser->getPosition());


	return make_unique<AssignStatement>( move(complexExpression) );
}

//wywołanie_funkcji	= "(", [lista_argumentów], ")";
UniqueCallStatement Parser::parseCallStatement() {
	if ( !expect(TokenTypes::OPEN_ROUND_BRACKET) )
		return nullptr;

	_token = _lekser->getNextToken();
	UniqueArgumentList argumentList = parseArgumentList();
	
	if ( !expect(TokenTypes::CLOSED_ROUND_BRACKET) )
		throw ParserException("Niezamkniety nawias wywolania funkcji", _lekser->getPosition());

	_token = _lekser->getNextToken();
	return make_unique<CallStatement>(argumentList);
}

//reszta_wyrażenia_złożonego = [operator_porównania, wyrażenie]
UniqueRestOfComplexExpression Parser::parseRestOfComplexExpression() {
	string operatorType;
	UniqueExpression rightExpression;
	if ( expect(TokenCategory::COMPARISION_OPERATOR) )
		operatorType = get<string>(_token.getValue());
	else
		return make_unique<RestOfComplexExpression>("", rightExpression);

	_token = _lekser->getNextToken();
	rightExpression = parseExpression();
	if (rightExpression == nullptr)
		throw ParserException("Prawa strona porównania jest nieprawidlowa", _lekser->getPosition());

	return make_unique<RestOfComplexExpression>(operatorType, rightExpression);
}

//powrót_z_funkcji = "return", [complexExpression];
UniqueStatement Parser::parseReturnFromFunction() {
	if ( !expect(TokenTypes::RETURN) )
		return nullptr;
	_token = _lekser->getNextToken();
	
	UniqueComplexExpression complexExpression = parseComplexExpression();
	return make_unique<ReturnStatement>(move(complexExpression));
}

//blok_petli_for = "FOR", identyfikator, "IN RANGE", wyrażenie_złożone, { działanie }, "END";
UniqueStatement Parser::parseForStatement() {
	if ( !expect(TokenTypes::FOR) )
		return nullptr;

	string ident;
	UniqueComplexExpression range;
	vector<UniqueStatement> statements;
	UniqueStatement state;
	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::IDENTIFIER) )
		throw ParserException("Brak identyfikatora w petli FOR", _lekser->getPosition());
	ident = get<string>(_token.getValue());

	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::IN) )
		throw ParserException("Czy chodzilo Ci o \"IN RANGE\"", _lekser->getPosition());
	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::RANGE) )
		throw ParserException("Czy chodzilo Ci o \"IN RANGE\"", _lekser->getPosition());

	_token = _lekser->getNextToken();
	range = parseComplexExpression();
	if (range == nullptr)
		throw ParserException("Nie podano ilosci obiegow petli FOR", _lekser->getPosition());

	while (state = parseStatement()) {
		statements.push_back(move(state));
	}

	if ( !expect(TokenTypes::END) )
		throw ParserException("Nie zamknieto petli FOR", _lekser->getPosition());
	_token = _lekser->getNextToken();

	return make_unique<ForStatement>(ident, move(range), move(statements));
}

//blok_petli_while	= "WHILE", "(", wyrażenie_złożone, ")", {działanie}, "END";
UniqueStatement Parser::parseWhileStatement() {
	if ( !expect(TokenTypes::WHILE) )
		return nullptr;

	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::OPEN_ROUND_BRACKET) )
		throw ParserException("Brak nawiasu otwierajacego po WHILE", _lekser->getPosition());

	_token = _lekser->getNextToken();
	UniqueComplexExpression compExpression = parseComplexExpression();
	if (compExpression == nullptr)
		throw ParserException("Zle skonstruowany warunek", _lekser->getPosition());

	if ( !expect(TokenTypes::CLOSED_ROUND_BRACKET) )
		throw ParserException("Brak nawiasu zamykajacego po warunku", _lekser->getPosition());

	UniqueStatement state;
	vector<UniqueStatement> whileStatements;
	_token = _lekser->getNextToken();
	while (state = parseStatement()) {
		whileStatements.push_back(move(state));
	}

	if ( !expect(TokenTypes::END) )
		throw ParserException("Nie zamknieta petla WHILE", _lekser->getPosition());
	_token = _lekser->getNextToken();

	return make_unique<WhileStatement>(compExpression, whileStatements);
}

//blok_if	= "IF", "(", wyrażenie_złożone, ")", {działanie}, ("END" | "ELSE", { działanie }, "END");
UniqueStatement Parser::parseIfStatement() {
	if ( !expect(TokenTypes::IF) )
		return nullptr;

	_token = _lekser->getNextToken();
	if ( !expect(TokenTypes::OPEN_ROUND_BRACKET) )
		throw ParserException("Brak nawiasu otwierajacego po IF", _lekser->getPosition());

	_token = _lekser->getNextToken();
	UniqueComplexExpression compExpression = parseComplexExpression();
	if (compExpression == nullptr)
		throw ParserException("Zle skonstruowany warunek", _lekser->getPosition());

	if( !expect(TokenTypes::CLOSED_ROUND_BRACKET) )
		throw ParserException("Brak nawiasu zamykajacego po warunku", _lekser->getPosition());

	UniqueStatement state;
	vector<UniqueStatement> ifStatements;
	vector<UniqueStatement> elseStatements;
	_token = _lekser->getNextToken();
	while (state = parseStatement()) {
		ifStatements.push_back(move(state));
	}
	
	if ( expect(TokenTypes::END) ) {
		_token = _lekser->getNextToken();
		return make_unique<IfStatement>(compExpression, ifStatements, elseStatements);
	}

	else if ( expect(TokenTypes::ELSE) ) {
		_token = _lekser->getNextToken();
		while (state = parseStatement()) {
			elseStatements.push_back(move(state));
		}

		if ( expect(TokenTypes::END) ) {
			_token = _lekser->getNextToken();
			return make_unique<IfStatement>(compExpression, ifStatements, elseStatements);
		}

		throw ParserException("Nie zamknieto bloku ELSE", _lekser->getPosition());
	}

	throw ParserException("Nie zamknieto bloku IF", _lekser->getPosition());
}

//wyrażenie_złożone	= [negacja], wyrażenie, [operator_porównania, wyrażenie];
UniqueComplexExpression Parser::parseComplexExpression() {
	bool isNegation = false;
	if ( expect(TokenTypes::NEGATION) ) {
		isNegation = true;
		_token = _lekser->getNextToken();
	}

	UniqueExpression leftExpression = parseExpression();
	UniqueExpression rightExpression;
	if (leftExpression == nullptr)
		return nullptr;

	string operatorType;
	if ( expect(TokenCategory::COMPARISION_OPERATOR) )
		operatorType = get<string>(_token.getValue());
	else
		return make_unique<ComplexExpression>(isNegation, leftExpression, "", rightExpression);

	_token = _lekser->getNextToken();
	rightExpression = parseExpression();
	if (rightExpression == nullptr)
		throw ParserException("Prawa strona porównania jest nieprawidlowa", _lekser->getPosition());

	return make_unique<ComplexExpression>(isNegation, leftExpression, operatorType, rightExpression);
}

//wyrażenie	= [znak_liczby], składnik_dodawania, {operator_dodawania, składnik_dodawania};
UniqueExpression Parser::parseExpression() {
	bool isPositive = true;
	if ( expect(TokenTypes::MINUS) ) {
		isPositive = false;
		_token = _lekser->getNextToken();
	}
	else if ( expect(TokenTypes::PLUS) )
		_token = _lekser->getNextToken();

	UniqueAdditionComponent leftComponent = parseAdditionComponent();
	if (leftComponent == nullptr)
		return nullptr;

	vector<bool> additionOperators;
	vector<UniqueAdditionComponent> rightComponents;
	while ( expect(TokenTypes::MINUS) || expect(TokenTypes::PLUS) ) {
		bool isSum = true;
		if ( expect(TokenTypes::MINUS) )
			isSum = false;

		additionOperators.push_back(isSum);
		_token = _lekser->getNextToken();
		UniqueAdditionComponent rightComponent = parseAdditionComponent();
		if (rightComponent == nullptr)
			throw ParserException("Nieprawidlowy skladnik dodawania", _lekser->getPosition());

		rightComponents.push_back( move(rightComponent));
	}

	return make_unique<Expression>(isPositive, leftComponent, additionOperators, rightComponents);
}

//składnik_dodawania = składnik_mnożenia, {operator_mnożenia, składnik_mnożenia};
UniqueAdditionComponent Parser::parseAdditionComponent() {
	UniqueMultiplyComponent leftMultiComponent;
	leftMultiComponent = parseMulitplyComponent();
	if (leftMultiComponent == nullptr)
		return nullptr;

	vector<bool> multiplyOperators;
	vector<UniqueMultiplyComponent> rightComponents;
	while ( expect(TokenTypes::MULTIPLICATION) || expect(TokenTypes::DIVISION) ) {
		bool isMultiply = true;
		if ( expect(TokenTypes::DIVISION) )
			isMultiply = false;

		multiplyOperators.push_back(isMultiply);
		_token = _lekser->getNextToken();
		UniqueMultiplyComponent rightComponent = parseMulitplyComponent();
		if (rightComponent == nullptr)
			throw ParserException("Nieprawidlowy skladnik mnozenia", _lekser->getPosition());

		rightComponents.push_back(move(rightComponent));
	}

	return make_unique<AdditionComponent>(leftMultiComponent, multiplyOperators, rightComponents);
}

//składnik_mnożenia	= element | (zmienna, [wywołanie_funkcji]) | "(", wyrażenie_złożone, ")";
UniqueMultiplyComponent Parser::parseMulitplyComponent() {
	UniqueElement element;
	string identifier;
	UniqueCallStatement callStatement;
	UniqueComplexExpression complexExpression;

	element = parseElement();
	if (element != nullptr) {
		return make_unique<MultiplyComponent>(element, identifier, move(callStatement), move(complexExpression));
	}

	if ( expect(TokenTypes::IDENTIFIER)) {
		identifier = get<string>(_token.getValue());
		_token = _lekser->getNextToken();
		callStatement = parseCallStatement();
		if (callStatement != nullptr)
			callStatement->setIdent(identifier);
		return make_unique<MultiplyComponent>(element, identifier, move(callStatement), move(complexExpression));
	}

	else if ( expect(TokenTypes::OPEN_ROUND_BRACKET) ) {
		_token = _lekser->getNextToken();
		complexExpression = parseComplexExpression();
		if (complexExpression == nullptr)
			throw ParserException("Spodziewano sie wyrazenia", _lekser->getPosition());

		if ( !expect(TokenTypes::CLOSED_ROUND_BRACKET) )
			throw ParserException("Nie zamknieto nawiasu", _lekser->getPosition());

		_token = _lekser->getNextToken();
		return make_unique<MultiplyComponent>(element, identifier, move(callStatement), move(complexExpression));
	}
	
	return nullptr;
}

//element = string | liczba | liczba_rzeczywista | lambda;
UniqueElement Parser::parseElement() {
	variant<int, float, std::string> elemValue;
	UniqueLambda lambda;
	if ( expect(TokenTypes::CONST_STRING) ||
		 expect(TokenTypes::CONST_FLOAT)  ||
		 expect(TokenTypes::CONST_INT) ) {

		elemValue = _token.getValue();
		_token = _lekser->getNextToken();
		return make_unique<Element>( elemValue, lambda);
	}

	lambda = parseLambda();
	if (lambda != nullptr)
		return make_unique<Element>(elemValue, lambda);
	
	return nullptr;
}

//lambda = "lambda", "[", [domknięcie], "]", "(", lista_parametrów, ")", { działanie }, "END",["(", [lista_argumentów], ")"];
UniqueLambda Parser::parseLambda() {
	if( !expect(TokenTypes::LAMBDA) )
		return nullptr;
	_token = _lekser->getNextToken();

	UniqueClosure closure;
	UniqueParameterList parameters;
	std::vector<UniqueStatement> statements;
	bool isCalled = false;
	UniqueArgumentList arguments;

	if (!expect(TokenTypes::OPEN_SQUARE_BRACKET))
		throw ParserException("spodziewano sie otwarcia kwadratowego nawiasu po \"lambda\"", _lekser->getPosition());
	_token = _lekser->getNextToken();

	closure = parseClosure();

	if (!expect(TokenTypes::CLOSED_SQUARE_BRACKET))
		throw ParserException("spodziewano sie zamkniecia nawiasu po domknieciu", _lekser->getPosition());
	_token = _lekser->getNextToken();

	if (!expect(TokenTypes::OPEN_ROUND_BRACKET))
		throw ParserException("spodziewano sie otwarcia nawiasu parametrow po domknieciu", _lekser->getPosition());
	_token = _lekser->getNextToken();

	parameters = parseParameterList();

	if (!expect(TokenTypes::CLOSED_ROUND_BRACKET))
		throw ParserException("spodziewano sie zamkniecia nawiasu parametrow", _lekser->getPosition());
	_token = _lekser->getNextToken();

	UniqueStatement state;
	while (state = parseStatement()) {
		statements.push_back(move(state));
	}

	if (!expect(TokenTypes::END))
		throw ParserException("brak slowa kluczowego END", _lekser->getPosition());
	_token = _lekser->getNextToken();

	if (!expect(TokenTypes::OPEN_ROUND_BRACKET))
		return make_unique<Lambda>(move(closure), move(parameters), move(statements), isCalled, move(arguments));
	_token = _lekser->getNextToken();

	isCalled = true;
	arguments = parseArgumentList();

	if (!expect(TokenTypes::CLOSED_ROUND_BRACKET))
		throw ParserException("nie zamknieto nawiasu po argumentach", _lekser->getPosition());
	_token = _lekser->getNextToken();
	
	return make_unique<Lambda>(move(closure), move(parameters), move(statements), isCalled, move(arguments));
}

//stara lista_argumentów = (referencja | element | (zmienna, [function_call])), {",", (referencja | element | (zmienna, [function_call])};
//lista_argumentów= (referencja | wyrażenie_złożone), { ",", (referencja | wyrażenie_złożone) };
UniqueArgumentList Parser::parseArgumentList() {
	vector<variant<Reference, UniqueComplexExpression>> argumentList;
	
	if( !getReferenceOrComplexExpression(argumentList) )
		return make_unique<ArgumentList>(argumentList);

	while (expect(TokenTypes::COMMA))
	{
		_token = _lekser->getNextToken();
		if (!getReferenceOrComplexExpression(argumentList))
			throw ParserException("Spodziewano sie argumentu", _lekser->getPosition());
	}

	return make_unique<ArgumentList>(argumentList);
}

//lista_parametrów = typ_zmiennej, (referencja | identyfikator), { ",", typ_zmiennej, (referencja | identyfikator) };
UniqueParameterList Parser::parseParameterList() {
	vector<TokenTypes> variableTypes;
	vector<variant<Reference, string>> parametersList;
	if (!expectVariableType()) {
		return make_unique<ParameterList>(variableTypes, parametersList);
	}
	else {
		variableTypes.push_back(_token.getType());
		_token = _lekser->getNextToken();
	}
	
	if (!getReferenceOrIdentifier(parametersList))
		throw ParserException("Spodziewano sie zmiennej po podaniu typu", _lekser->getPosition());


	while( expect(TokenTypes::COMMA) ){
		_token = _lekser->getNextToken();
		if ( expectVariableType() ) {
			variableTypes.push_back(_token.getType());
			_token = _lekser->getNextToken();
		}
		else
			throw("Nie podano typu zmiennej po przecinku", _lekser->getPosition());

		if ( !getReferenceOrIdentifier(parametersList) )
			throw ParserException("Spodziewano sie zmiennej po podaniu typu", _lekser->getPosition());
	}

	return make_unique<ParameterList>(variableTypes, parametersList);
}

//domkniecie = (referencja | zmienna), {",", (referencja | zmienna)};
UniqueClosure Parser::parseClosure() {
	vector<variant<Reference, string>> closureList;

	if (!getReferenceOrIdentifier(closureList))
		return make_unique<Closure>(closureList);

	while (expect(TokenTypes::COMMA))
	{
		_token = _lekser->getNextToken();
		if (!getReferenceOrIdentifier(closureList))
			throw ParserException("Spodziewano sie argumentu", _lekser->getPosition());
	}

	return make_unique<Closure>(closureList);
}

bool Parser::expect(TokenTypes type) const {
	if (_token.getType() == type)
		return true;
	else
		return false;
}
bool Parser::expect(TokenCategory category) const {
	if (_token.getCategory() == category)
		return true;
	else
		return false;
}
bool Parser::expectVariableType() const {
	if (expect(TokenTypes::INTEGER) || expect(TokenTypes::FLOAT)
	 || expect(TokenTypes::STRING)  || expect(TokenTypes::FUNCTION))
		return true;
	else
		return false;
}

bool Parser::getReferenceOrComplexExpression(vector<variant<Reference, UniqueComplexExpression>>& argumentList){

	if (expect(TokenTypes::REFERENCE)) {
		_token = _lekser->getNextToken();
		if (expect(TokenTypes::IDENTIFIER)) {
			argumentList.push_back(Reference(get<string>(_token.getValue())));
			_token = _lekser->getNextToken();
			return true;
		}
		else
			throw ParserException("Referencja bez identyfikatora", _lekser->getPosition());
	}

	UniqueComplexExpression complExpression = parseComplexExpression();
	if (complExpression != nullptr) {
		argumentList.push_back(move(complExpression));
		return true;
	}
	
	return false;
}

bool Parser::getReferenceOrIdentifier(vector<variant<Reference, string>>& vec) {
	if (expect(TokenTypes::REFERENCE)) {
		_token = _lekser->getNextToken();
		if (expect(TokenTypes::IDENTIFIER)) {
			vec.push_back(Reference(get<string>(_token.getValue())));
			_token = _lekser->getNextToken();
			return true;
		}
		else
			throw ParserException("Referencja bez identyfikatora", _lekser->getPosition());
	}

	if (expect(TokenTypes::IDENTIFIER)) {
		vec.push_back(get<string>(_token.getValue()));
		_token = _lekser->getNextToken();
		return true;
	}

	return false;
}

void Parser::checkForSemicolon()
{
	if ( !expect(TokenTypes::SEMICOLON) )
		throw ParserException("brak srednika", _lekser->getPosition());
	_token = _lekser->getNextToken();
}
