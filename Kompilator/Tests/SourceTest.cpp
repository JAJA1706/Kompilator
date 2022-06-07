#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include "..\Lekser\Lekser.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(FileSourceClass)
	BOOST_AUTO_TEST_CASE(OpeningFile)
	{
		BOOST_CHECK_THROW( FileSource testFile("Tests/niematakiegopliku.txt"), std::invalid_argument );
		BOOST_CHECK_NO_THROW(FileSource testFile("Tests/jesttakiplik.txt"));
	}

	BOOST_AUTO_TEST_CASE(ReadingFile) 
	{
		UniqueFileSource file(new FileSource("Tests/readTest.txt"));

		BOOST_CHECK_EQUAL(file->getNextChar(), 't');
		BOOST_CHECK_EQUAL(file->peekNextChar(), 'a');
		BOOST_CHECK_EQUAL(file->getNextChar(), 'a');
		BOOST_CHECK_EQUAL(file->getNextChar(), 'k');
		BOOST_CHECK_EQUAL(file->peekNextChar(), EOF);
		BOOST_CHECK_EQUAL(file->getNextChar(), EOF);
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(StreamSourceClass)
	BOOST_AUTO_TEST_CASE(ReadingStringStream) 
	{
		string sentence = "nie";
		istringstream stringStream(sentence);
		UniqueStreamSource stream(new StreamSource(stringStream));

		BOOST_CHECK_EQUAL(stream->getNextChar(), 'n');
		BOOST_CHECK_EQUAL(stream->peekNextChar(), 'i');
		BOOST_CHECK_EQUAL(stream->getNextChar(), 'i');
		BOOST_CHECK_EQUAL(stream->getNextChar(), 'e');
		BOOST_CHECK_EQUAL(stream->peekNextChar(), EOF);
		BOOST_CHECK_EQUAL(stream->getNextChar(), EOF);
	}

	BOOST_AUTO_TEST_CASE(ReadingFileStream)
	{
		fstream file;
		file.open("Tests/readTest.txt", fstream::in);
		UniqueStreamSource stream(new StreamSource(file));

		BOOST_CHECK_EQUAL(stream->getNextChar(), 't');
		BOOST_CHECK_EQUAL(stream->peekNextChar(), 'a');
		BOOST_CHECK_EQUAL(stream->getNextChar(), 'a');
		BOOST_CHECK_EQUAL(stream->getNextChar(), 'k');
		BOOST_CHECK_EQUAL(stream->peekNextChar(), EOF);
		BOOST_CHECK_EQUAL(stream->getNextChar(), EOF);
		file.close();
	}
BOOST_AUTO_TEST_SUITE_END()