#include <boost/test/unit_test.hpp>
#include <vector>
#include "..\Parser\Parser.h"
#include "..\Lekser\Lekser.h"
#include "..\Program\Program.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(ProgramClass)
	BOOST_AUTO_TEST_CASE(VariableDefinitionCase)
	{
		istringstream stringStream("Start int x = 3; float y = 5; float z = 0.5; string a = \"xddd\"; int b;  STOP");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK(get<int>(stack.getVariableValue("x")) == 3);
		BOOST_CHECK(get<float>(stack.getVariableValue("y")) == 5);
		BOOST_CHECK(get<float>(stack.getVariableValue("z")) == 0.5);
		BOOST_CHECK(get<string>(stack.getVariableValue("a")) == "xddd");
		BOOST_CHECK(get<int>(stack.getVariableValue("b")) == 0);
	}

	BOOST_AUTO_TEST_CASE(VariableAssignCase)
	{
		istringstream stringStream(	"Start int x; float y; string z; "  
									"x = 2+0.5*(4-1);				 "
									"y = 2+0.5*(4-1);				"
									"z = \"tak \" + \"i nie\";	STOP");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 3);
		BOOST_CHECK_EQUAL(get<float>(stack.getVariableValue("y")), 3.5);
		BOOST_CHECK_EQUAL(get<string>(stack.getVariableValue("z")), "tak i nie");
	}

	BOOST_AUTO_TEST_CASE(WhileLoopCase)
	{
		istringstream stringStream("Start int x = 3; while(x <= 10) x = x + 1; END STOP ");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 11);
	}

	BOOST_AUTO_TEST_CASE(ForLoopCase)
	{
		istringstream stringStream("START int x; int y = 10; FOR x IN RANGE 10 y = y + 1; END  STOP");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 10);
		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("y")), 20);
	}

	BOOST_AUTO_TEST_CASE(IfStatementCase)
	{
		istringstream stringStream("START int x = 4; float y = 4.1; IF(x < y) x = 0; ELSE y = 0; END  STOP");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 0);
		BOOST_CHECK_CLOSE(get<float>(stack.getVariableValue("y")), 4.1, 0.01);
	}

	BOOST_AUTO_TEST_CASE(CallStatementCase)
	{
		istringstream stringStream( "DEF fun(int x, int ref y)		"
									"	x = 10;						"
									"	y = 9;						"
									"	return x;					"
									"END							"	
									"START							"
									"	int x; int y; int z;		"
									"	z = fun(x, ref y);			"
									"STOP							");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 0);
		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("y")), 9);
		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("z")), 10);
	}

	BOOST_AUTO_TEST_CASE(RecursiveCase)
	{
		istringstream stringStream( " DEF fun(int x)				"
									"	IF(x < 5)					"
									"		return x;				"
									"	ELSE						"
									"		x = x - 1;				"
									"		return fun(x);			"
									"	END							"
									"END							"
									"START							"
									"	int x = 10;					"
									"	x = fun(x);					"
									"STOP							");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 4);
	}

	BOOST_AUTO_TEST_CASE(LambdaCase)
	{
		istringstream stringStream(	" DEF fun(function lam)												"
									"	return lam(2);													"
									"END																"
									"START																"
									"	int x = lambda[](int y) return y*2; END(10);					"
									"	function lamFun = lambda[](int x) x = x + 1; return x==3; END;	"
									"	int y = fun(lamFun);											"
									"	int z = lambda[ref y]() y = y - 10; return 5; END();			"
									"STOP																");

		UniqueStreamSource stream(new StreamSource(stringStream));
		unique_ptr<Lekser> lekser(make_unique <Lekser>(move(stream)));
		Parser parser(lekser.get());
		Program program = parser.parseProgram();
		program.execute();
		Stack stack = program.getStack();

		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("x")), 20);
		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("y")), -9);
		BOOST_CHECK_EQUAL(get<int>(stack.getVariableValue("z")), 5);
	}


BOOST_AUTO_TEST_SUITE_END()