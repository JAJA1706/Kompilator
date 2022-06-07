#ifndef PARSER_H
#define PARSER_H
#include "..\Lekser\Lekser.h"
#include "..\Program\Program.h"
#include "..\Statements\IfStatement.h"
#include "..\Statements\AssignStatement.h"
#include "..\Statements\CallStatement.h"
#include "..\Statements\ForStatement.h"
#include "..\Statements\WhileStatement.h"
#include "..\Statements\VariableDefinition.h"
#include "..\Statements\ReturnStatement.h"
#include "..\Statements\FunctionDefinition.h"
#include "..\Expressions\ComplexExpression.h"
#include "..\Expressions\Expression.h"
#include "..\Expressions\RestOfComplexExpression.h"

class Parser
{
public:
	Parser(Lekser* lekser = nullptr) : _lekser(lekser) {}
	Program parseProgram();

private: 
	UniqueFunctionDefinition parseFunctionDefinition();
	UniqueStatement parseStatement();
	UniqueVariableDefinition parseVariableDefinition();
	UniqueStatement parseAssignOrCallOrComplexExpression();
	UniqueStatement parseReturnFromFunction();
	UniqueAssignStatement parseAssignStatement();
	UniqueCallStatement parseCallStatement();
	UniqueArgumentList parseArgumentList();
	UniqueParameterList parseParameterList();
	UniqueClosure parseClosure();
	UniqueRestOfComplexExpression parseRestOfComplexExpression();
	UniqueStatement parseWhileStatement();
	UniqueStatement parseForStatement();
	UniqueStatement parseIfStatement();
	UniqueComplexExpression parseComplexExpression();
	UniqueExpression parseExpression();
	UniqueAdditionComponent parseAdditionComponent();
	UniqueMultiplyComponent parseMulitplyComponent();
	UniqueElement parseElement();
	UniqueLambda parseLambda();
	bool expect(TokenTypes type) const;
	bool expect(TokenCategory category) const;
	bool expectVariableType() const;
	bool getReferenceOrComplexExpression(std::vector<std::variant<Reference, UniqueComplexExpression>>& argumentList);
	bool getReferenceOrIdentifier(std::vector<std::variant<Reference, std::string>>& argumentList);
	void checkForSemicolon();

	Lekser* _lekser;
	Token _token;
};

#endif