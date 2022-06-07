#include <iostream>
#include "Source.h"

using namespace std;

int FileSource::getPosition()
{
	return _position;
}

FileSource::FileSource(std::string fileName) : _fileName(fileName) {
	if (fopen_s(&_file, _fileName.c_str(), "r"))
		throw std::invalid_argument("Nie odnaleziono pliku");
}

FileSource::FileSource(const FileSource& fileSource)
{
	_fileName = fileSource._fileName;
	fopen_s(&_file, _fileName.c_str(), "r");
}

FileSource& FileSource::operator=(const FileSource& fileSource)
{
	_fileName = fileSource._fileName;
	fopen_s(&_file, _fileName.c_str(), "r");
	return *this;
}

char FileSource::getNextChar() {
	++_position;
	return fgetc(_file);
}

char FileSource::peekNextChar() {
	fpos_t position;
	char peek;

	fgetpos(_file, &position);
	peek = fgetc(_file);
	fsetpos(_file, &position);

	return peek;
}


int StreamSource::getPosition()
{
	return _position;
}

StreamSource::StreamSource(istream& stream) : _stream(stream) {};

char StreamSource::getNextChar() {
	if (_buffer.length() == 0) {
		char tempBuffer[64];

		//cout << "Wprowadz kod: \n";
		_stream.get(tempBuffer, 64, EOF);
		_buffer.assign(tempBuffer);

		if (_buffer.length() == 0)
			return EOF;
	}

	char firstChar = _buffer[0];
	_buffer.erase(0,1);
	++_position;
	return firstChar;
}

char StreamSource::peekNextChar() {
	if (_buffer.length() == 0) {
		char tempBuffer[64];

		//cout << "Wprowadz kod: \n";
		_stream.get(tempBuffer, 64, EOF);
		_buffer.assign(tempBuffer);

		if (_buffer.length() == 0)
			return EOF;
	}

	return _buffer[0];
}