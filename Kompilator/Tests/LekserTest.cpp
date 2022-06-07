#include <boost/test/unit_test.hpp>
#include <vector>
#include "..\Lekser\Lekser.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(LekserClass)
UniqueFileSource file(new FileSource("Tests/LekserTest.txt"));
Lekser fileLekser(move(file));


	BOOST_AUTO_TEST_CASE(CommentCase) 
	{
		istringstream stringStream("  \n  //to jest komentarz \n");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::COMMENT);
		BOOST_CHECK_EQUAL(std::get<string>( token.getValue() ), "to jest komentarz ");
	}

	BOOST_AUTO_TEST_CASE(EOFCase)
	{
		istringstream stringStream("   \t \n \t   \n");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::END_OF_FILE);
	}

	BOOST_AUTO_TEST_CASE(NumberCase)
	{
		istringstream stringStream("2333413956");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::CONST_INT);
		BOOST_CHECK_EQUAL(std::get<int>(token.getValue()), 2333413956);
		BOOST_CHECK(std::get<int>(token.getValue()) != 0);
	}

	BOOST_AUTO_TEST_CASE(FloatCase)
	{
		istringstream stringStream("   34.5521 ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		float testFloat = 34.5521;
		double testDouble = 34.5521;

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::CONST_FLOAT);
		BOOST_CHECK_EQUAL(std::get<float>(token.getValue()), testFloat);
		BOOST_CHECK_CLOSE(std::get<float>(token.getValue()), testDouble, 0.01);
	}

	BOOST_AUTO_TEST_CASE(FloatCase2)
	{
		istringstream stringStream("0.35521");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		float testFloat = 0.35521;
		double testDouble = 0.35521;

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::CONST_FLOAT);
		BOOST_CHECK_EQUAL(std::get<float>(token.getValue()), testFloat);
		BOOST_CHECK_CLOSE(std::get<float>(token.getValue()), testDouble, 0.01);
	}

	BOOST_AUTO_TEST_CASE(FloatCase3)
	{
		istringstream stringStream("0001.045");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		float testFloat = 1.045;
		double testDouble = 1.045;

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::CONST_FLOAT);
		BOOST_CHECK_EQUAL(std::get<float>(token.getValue()), testFloat);
		BOOST_CHECK_CLOSE(std::get<float>(token.getValue()), testDouble, 0.01);
	}

	BOOST_AUTO_TEST_CASE(StringCase)
	{
		istringstream stringStream(" \"To sie zgadza\"");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::CONST_STRING);
		BOOST_CHECK_EQUAL(std::get<string>(token.getValue()), "To sie zgadza");
		BOOST_CHECK(std::get<string>(token.getValue()) != "To sie nie zgadza");
	}

	BOOST_AUTO_TEST_CASE(UnterminatedString)
	{
		istringstream stringStream("\"abc");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		Token token = lekser.getNextToken();
		BOOST_CHECK(token.getType() == TokenTypes::UNKNOWN);
	}

	BOOST_AUTO_TEST_CASE(KeyWordCase)
	{
		istringstream stringStream("int");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::INTEGER);
		BOOST_CHECK_EQUAL(std::get<string>(token.getValue()), "int");
		BOOST_CHECK(std::get<string>(token.getValue()) != "float");
	}

	BOOST_AUTO_TEST_CASE(SeparatorCase)
	{
		istringstream stringStream("  , ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::COMMA);
		BOOST_CHECK_EQUAL(std::get<string>(token.getValue()), ",");
	}

	BOOST_AUTO_TEST_CASE(OperatorCase)
	{
		istringstream stringStream("!= ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));

		Token token = lekser.getNextToken();

		BOOST_CHECK(token.getType() == TokenTypes::UNEQUAL);
		BOOST_CHECK_EQUAL(std::get<string>(token.getValue()), "!=");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MultipleTests)

	BOOST_AUTO_TEST_CASE(MultipleTest1)
	{
		istringstream stringStream("DEF fun1(string ref tak)\n\ttak = \"nie\";\nEND");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		vector<Token> vec;
		Token token;

		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);


		BOOST_CHECK(vec[0].getType() == TokenTypes::DEF);
		BOOST_CHECK(vec[1].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[2].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[3].getType() == TokenTypes::STRING);
		BOOST_CHECK(vec[4].getType() == TokenTypes::REFERENCE);
		BOOST_CHECK(vec[5].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[6].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[7].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[8].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[9].getType() == TokenTypes::CONST_STRING);
		BOOST_CHECK(vec[10].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[11].getType() == TokenTypes::END);
		BOOST_CHECK(vec[12].getType() == TokenTypes::END_OF_FILE);
		BOOST_CHECK_EQUAL(std::get<string>(vec[0].getValue()), "DEF");
		BOOST_CHECK_EQUAL(std::get<string>(vec[1].getValue()), "fun1");
		BOOST_CHECK_EQUAL(std::get<string>(vec[2].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[3].getValue()), "string");
		BOOST_CHECK_EQUAL(std::get<string>(vec[4].getValue()), "ref");
		BOOST_CHECK_EQUAL(std::get<string>(vec[5].getValue()), "tak");
		BOOST_CHECK_EQUAL(std::get<string>(vec[6].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[7].getValue()), "tak");
		BOOST_CHECK_EQUAL(std::get<string>(vec[8].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[9].getValue()), "nie");
		BOOST_CHECK_EQUAL(std::get<string>(vec[10].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[11].getValue()), "END");
		BOOST_CHECK_EQUAL(std::get<string>(vec[12].getValue()), "");
	}

	BOOST_AUTO_TEST_CASE(MultipleTest2)
	{
		istringstream stringStream("start\nint x = 2;\nx = x*x+3*61.75;\nfunction L = lambda[x](int a)\na = a + x;//jazduniaaa\nend(3)"
									" ");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		vector<Token> vec;
		Token token;

		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);

		BOOST_CHECK(vec[0].getType() == TokenTypes::START);
		BOOST_CHECK(vec[1].getType() == TokenTypes::INTEGER);
		BOOST_CHECK(vec[2].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[3].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[4].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[5].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[6].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[7].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[8].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[9].getType() == TokenTypes::MULTIPLICATION);
		BOOST_CHECK(vec[10].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[11].getType() == TokenTypes::PLUS);
		BOOST_CHECK(vec[12].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[13].getType() == TokenTypes::MULTIPLICATION);
		BOOST_CHECK(vec[14].getType() == TokenTypes::CONST_FLOAT);
		BOOST_CHECK(vec[15].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[16].getType() == TokenTypes::FUNCTION);
		BOOST_CHECK(vec[17].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[18].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[19].getType() == TokenTypes::LAMBDA);
		BOOST_CHECK(vec[20].getType() == TokenTypes::OPEN_SQUARE_BRACKET);
		BOOST_CHECK(vec[21].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[22].getType() == TokenTypes::CLOSED_SQUARE_BRACKET);
		BOOST_CHECK(vec[23].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[24].getType() == TokenTypes::INTEGER);
		BOOST_CHECK(vec[25].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[26].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[27].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[28].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[29].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[30].getType() == TokenTypes::PLUS);
		BOOST_CHECK(vec[31].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[32].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[33].getType() == TokenTypes::COMMENT);
		BOOST_CHECK(vec[34].getType() == TokenTypes::END);
		BOOST_CHECK(vec[35].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[36].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[37].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[38].getType() == TokenTypes::END_OF_FILE);
		BOOST_CHECK_EQUAL(std::get<string>(vec[0].getValue()), "START");
		BOOST_CHECK_EQUAL(std::get<string>(vec[1].getValue()), "int");
		BOOST_CHECK_EQUAL(std::get<string>(vec[2].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[3].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<int>(vec[4].getValue()), 2);
		BOOST_CHECK_EQUAL(std::get<string>(vec[5].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[6].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[7].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[8].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[9].getValue()), "*");
		BOOST_CHECK_EQUAL(std::get<string>(vec[10].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[11].getValue()), "+");
		BOOST_CHECK_EQUAL(std::get<int>(vec[12].getValue()), 3);
		BOOST_CHECK_EQUAL(std::get<string>(vec[13].getValue()), "*");
		BOOST_CHECK_EQUAL(std::get<float>(vec[14].getValue()), 61.75);
		BOOST_CHECK_EQUAL(std::get<string>(vec[15].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[16].getValue()), "function");
		BOOST_CHECK_EQUAL(std::get<string>(vec[17].getValue()), "L");
		BOOST_CHECK_EQUAL(std::get<string>(vec[18].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[19].getValue()), "lambda");
		BOOST_CHECK_EQUAL(std::get<string>(vec[20].getValue()), "[");
		BOOST_CHECK_EQUAL(std::get<string>(vec[21].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[22].getValue()), "]");
		BOOST_CHECK_EQUAL(std::get<string>(vec[23].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[24].getValue()), "int");
		BOOST_CHECK_EQUAL(std::get<string>(vec[25].getValue()), "a");
		BOOST_CHECK_EQUAL(std::get<string>(vec[26].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[27].getValue()), "a");
		BOOST_CHECK_EQUAL(std::get<string>(vec[28].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[29].getValue()), "a");
		BOOST_CHECK_EQUAL(std::get<string>(vec[30].getValue()), "+");
		BOOST_CHECK_EQUAL(std::get<string>(vec[31].getValue()), "x");
		BOOST_CHECK_EQUAL(std::get<string>(vec[32].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[33].getValue()), "jazduniaaa");
		BOOST_CHECK_EQUAL(std::get<string>(vec[34].getValue()), "END");
		BOOST_CHECK_EQUAL(std::get<string>(vec[35].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<int>(vec[36].getValue()), 3);
		BOOST_CHECK_EQUAL(std::get<string>(vec[37].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[38].getValue()), "");
	}

	BOOST_AUTO_TEST_CASE(MultipleTest3)
	{
		istringstream stringStream("41.abc(*!==superkod_pozdrawiam>=<=10\n\n\t");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		vector<Token> vec;
		Token token;

		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);

		BOOST_CHECK(vec[0].getType() == TokenTypes::CONST_FLOAT);
		BOOST_CHECK(vec[1].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[2].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[3].getType() == TokenTypes::MULTIPLICATION);
		BOOST_CHECK(vec[4].getType() == TokenTypes::UNEQUAL);
		BOOST_CHECK(vec[5].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[6].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[7].getType() == TokenTypes::NOT_SMALLER);
		BOOST_CHECK(vec[8].getType() == TokenTypes::NOT_GREATER);
		BOOST_CHECK(vec[9].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[10].getType() == TokenTypes::END_OF_FILE);
		BOOST_CHECK_EQUAL(std::get<float>(vec[0].getValue()), 41);
		BOOST_CHECK_EQUAL(std::get<string>(vec[1].getValue()), "abc");
		BOOST_CHECK_EQUAL(std::get<string>(vec[2].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[3].getValue()), "*");
		BOOST_CHECK_EQUAL(std::get<string>(vec[4].getValue()), "!=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[5].getValue()), "=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[6].getValue()), "superkod_pozdrawiam");
		BOOST_CHECK_EQUAL(std::get<string>(vec[7].getValue()), ">=");
		BOOST_CHECK_EQUAL(std::get<string>(vec[8].getValue()), "<=");
		BOOST_CHECK_EQUAL(std::get<int>(vec[9].getValue()), 10);
		BOOST_CHECK_EQUAL(std::get<string>(vec[10].getValue()), "");
	}

	BOOST_AUTO_TEST_CASE(MultipleTest4)
	{
		istringstream stringStream("FORiINRANGE;FOR abc In RaNgE 100output(\"tak\")");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		vector<Token> vec;
		Token token;

		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);

		BOOST_CHECK(vec[0].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[1].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[2].getType() == TokenTypes::FOR);
		BOOST_CHECK(vec[3].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[4].getType() == TokenTypes::IN);
		BOOST_CHECK(vec[5].getType() == TokenTypes::RANGE);
		BOOST_CHECK(vec[6].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[7].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[8].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[9].getType() == TokenTypes::CONST_STRING);
		BOOST_CHECK(vec[10].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK_EQUAL(std::get<string>(vec[0].getValue()), "FORiINRANGE");
		BOOST_CHECK_EQUAL(std::get<string>(vec[1].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[2].getValue()), "FOR");
		BOOST_CHECK_EQUAL(std::get<string>(vec[3].getValue()), "abc");
		BOOST_CHECK_EQUAL(std::get<string>(vec[4].getValue()), "IN");
		BOOST_CHECK_EQUAL(std::get<string>(vec[5].getValue()), "RANGE");
		BOOST_CHECK_EQUAL(std::get<int>(vec[6].getValue()), 100);
		BOOST_CHECK_EQUAL(std::get<string>(vec[7].getValue()), "output");
		BOOST_CHECK_EQUAL(std::get<string>(vec[8].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[9].getValue()), "tak");
		BOOST_CHECK_EQUAL(std::get<string>(vec[10].getValue()), ")");
	}

	BOOST_AUTO_TEST_CASE(MultipleTest5)
	{
		istringstream stringStream("if(abc >= 3)\nfun1(\"super\");ElSe\n abc\t/abc//tak*nie\\lol\n;output();");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser(move(stream));
		vector<Token> vec;
		Token token;

		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);

		BOOST_CHECK(vec[0].getType() == TokenTypes::IF);
		BOOST_CHECK(vec[1].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[2].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[3].getType() == TokenTypes::NOT_SMALLER);
		BOOST_CHECK(vec[4].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[5].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[6].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[7].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[8].getType() == TokenTypes::CONST_STRING);
		BOOST_CHECK(vec[9].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[10].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[11].getType() == TokenTypes::ELSE);
		BOOST_CHECK(vec[12].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[13].getType() == TokenTypes::DIVISION);
		BOOST_CHECK(vec[14].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[15].getType() == TokenTypes::COMMENT);
		BOOST_CHECK(vec[16].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[17].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[18].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[19].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[20].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[21].getType() == TokenTypes::END_OF_FILE);
		BOOST_CHECK_EQUAL(std::get<string>(vec[0].getValue()), "IF");
		BOOST_CHECK_EQUAL(std::get<string>(vec[1].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[2].getValue()), "abc");
		BOOST_CHECK_EQUAL(std::get<string>(vec[3].getValue()), ">=");
		BOOST_CHECK_EQUAL(std::get<int>(vec[4].getValue()), 3);
		BOOST_CHECK_EQUAL(std::get<string>(vec[5].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[6].getValue()), "fun1");
		BOOST_CHECK_EQUAL(std::get<string>(vec[7].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[8].getValue()), "super");
		BOOST_CHECK_EQUAL(std::get<string>(vec[9].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[10].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[11].getValue()), "ELSE");
		BOOST_CHECK_EQUAL(std::get<string>(vec[12].getValue()), "abc");
		BOOST_CHECK_EQUAL(std::get<string>(vec[13].getValue()), "/");
		BOOST_CHECK_EQUAL(std::get<string>(vec[14].getValue()), "abc");
		BOOST_CHECK_EQUAL(std::get<string>(vec[15].getValue()), "tak*nie\\lol");
		BOOST_CHECK_EQUAL(std::get<string>(vec[16].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[17].getValue()), "output");
		BOOST_CHECK_EQUAL(std::get<string>(vec[18].getValue()), "(");
		BOOST_CHECK_EQUAL(std::get<string>(vec[19].getValue()), ")");
		BOOST_CHECK_EQUAL(std::get<string>(vec[20].getValue()), ";");
		BOOST_CHECK_EQUAL(std::get<string>(vec[21].getValue()), "");
	}

	BOOST_AUTO_TEST_CASE(MultipleTest6)
	{
		istringstream stringStream( "START"
									"	if(abc >= 3)"
									"		fun1(10);"
									"	ELSE"
									"		abc = -10;"
									"STOP");
		UniqueStreamSource stream(new StreamSource(stringStream));
		Lekser lekser( move(stream) );
		vector<Token> vec;
		Token token;
		do {
			token = lekser.getNextToken();
			vec.push_back(token);
		} while (token.getType() != TokenTypes::END_OF_FILE);

		BOOST_CHECK(vec[0].getType() == TokenTypes::START);
		BOOST_CHECK(vec[1].getType() == TokenTypes::IF);
		BOOST_CHECK(vec[2].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[3].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[4].getType() == TokenTypes::NOT_SMALLER);
		BOOST_CHECK(vec[5].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[6].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[7].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[8].getType() == TokenTypes::OPEN_ROUND_BRACKET);
		BOOST_CHECK(vec[9].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[10].getType() == TokenTypes::CLOSED_ROUND_BRACKET);
		BOOST_CHECK(vec[11].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[12].getType() == TokenTypes::ELSE);
		BOOST_CHECK(vec[13].getType() == TokenTypes::IDENTIFIER);
		BOOST_CHECK(vec[14].getType() == TokenTypes::ASSIGN);
		BOOST_CHECK(vec[15].getType() == TokenTypes::MINUS);
		BOOST_CHECK(vec[16].getType() == TokenTypes::CONST_INT);
		BOOST_CHECK(vec[17].getType() == TokenTypes::SEMICOLON);
		BOOST_CHECK(vec[18].getType() == TokenTypes::STOP);
	}

BOOST_AUTO_TEST_SUITE_END()