#include <boost/test/unit_test.hpp>
#include <vector>
#include "..\Parser\Parser.h"
#include "..\Lekser\Lekser.h"
#include "..\Program\Program.h"
#include "..\Parser\ParserException.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(ParserClass)

	BOOST_AUTO_TEST_CASE(AssignStatementCase) 
	{
		istringstream stringStream("START a = 3; b = fun(10); c = (1+3); STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		AssignStatement* statement = dynamic_cast<AssignStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(CallStatementCase)
	{
		istringstream stringStream("START fun(abc, 10, \"yes\", 34.12); fun2(); fun3(fun2(((1>0)))); STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		CallStatement* statement = dynamic_cast<CallStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(DefinitionStatementCase)
	{
		istringstream stringStream("START int x; float y = 3; string z = x; STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		VariableDefinition* statement = dynamic_cast<VariableDefinition*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(IfStatementCase)
	{
		istringstream stringStream("START IF( !(x > 5) ) return; END STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		IfStatement* statement = dynamic_cast<IfStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(ReturnStatementCase)
	{
		istringstream stringStream("START return x; return 10; return \"lol\"; return; return fun(x); STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		ReturnStatement* statement = dynamic_cast<ReturnStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(ForStatementCase)
	{
		istringstream stringStream("START FOR x IN RANGE (1==1 / x - megaFun(y) ) fun(); END STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		ForStatement* statement = dynamic_cast<ForStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(WhileStatementCase)
	{
		istringstream stringStream("START while(1+2*(fun(x)-10) > 0) x = x+5; END STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		WhileStatement* statement = dynamic_cast<WhileStatement*>(program.getStatement(0));
		BOOST_CHECK(statement != nullptr);
	}

	BOOST_AUTO_TEST_CASE(lambdaCase)
	{
		istringstream stringStream(	"START  "
									"	function l = lambda[]() return 5; END;	"
									"   int x = l();							" 

									"   x = lambda[](int a)						"
									"			a = a * 2;						"
									"		END(10);							"	
									"STOP										");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());

		BOOST_CHECK_NO_THROW(parser.parseProgram());
	}

	BOOST_AUTO_TEST_CASE(ProgramThrowCase)
	{
		istringstream stringStream( ""
									"	if(abc >= 3)"
									"		"
									"	END"
									"STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>( move(stream) ));
		Parser parser(lekser.get());

		BOOST_CHECK_THROW(parser.parseProgram(), ParserException);
	}

	BOOST_AUTO_TEST_CASE(ProgramThrowCase2)
	{
		istringstream stringStream( "START  "
									"	WHILE(xd)"
									"		x = x+3" //brak srednika
									"   END    "
									"STOP   ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());

		BOOST_CHECK_THROW(parser.parseProgram(), ParserException);
	}

	BOOST_AUTO_TEST_CASE(ProgramCase)
	{
	istringstream stringStream( "START\n"
								"	if(abc >= 3)\n"
								"		fun1(10);\n"
								"	ELSE\n"
								"		abc = 10;\n"
								"	END\n"
								"STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>( move(stream) ));
		Parser parser( lekser.get() );

		BOOST_CHECK_NO_THROW(parser.parseProgram());
	}

	BOOST_AUTO_TEST_CASE(ProgramCase2)
	{
		istringstream stringStream( "START  "
									"	IF(xd <= lool)"
									"		abc(tak,nie,10);"
									"   ELSE"
									"       cbd(ref x);"
									"	END "
									"STOP   ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>( move(stream) ));
		Parser parser(lekser.get());

		BOOST_CHECK_NO_THROW(parser.parseProgram());
	}

	BOOST_AUTO_TEST_CASE(ProgramCase3)
	{
		istringstream stringStream( "START  "
									"	FOR x IN RANGE 10"
									"		abc(tak,nie,10);"
									"       IF(x == 5) "
									"			return;"
									"       END        "
									"	END "
									"STOP   ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>( move(stream) ));
		Parser parser(lekser.get());

		BOOST_CHECK_NO_THROW(parser.parseProgram());
	}

	BOOST_AUTO_TEST_CASE(MegaProgramCase)
	{
		istringstream stringStream(	"DEF fun(int x, function l)		"
									"	x = l(10);					"
									"	return x;					"
									"END							"
									"Def fun2()						"
									"	float jazdunia = 03;		"
									"	lambda[ref jazdunia]()		"
									"		jazdunia = jazdunia*2;	"
									"	END();						"
									"	print(jazdunia);			"
									"END							"
									"int tak = 0;					"
									"START							"
									"	int x;						"
									"	FOR x IN RANGE (((3+1)*2))	"
									"		fun2();					"
									"       x = x / 0.5;			"
									"		print(fun(x, lambda[](int num) return num+1; END));"
									"	END							"
									"STOP							");
		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();

		FunctionDefinition* function = dynamic_cast<FunctionDefinition*>(program.getFunction(0));
		FunctionDefinition* function2 = dynamic_cast<FunctionDefinition*>(program.getFunction(1));
		VariableDefinition* statement = dynamic_cast<VariableDefinition*>(program.getStatement(0));
		ForStatement* statement2 = dynamic_cast<ForStatement*>(program.getStatement(1));

		BOOST_CHECK(function != nullptr);
		BOOST_CHECK(function2 != nullptr);
		BOOST_CHECK(statement != nullptr);
		BOOST_CHECK(statement2 != nullptr);

	}

BOOST_AUTO_TEST_SUITE_END()