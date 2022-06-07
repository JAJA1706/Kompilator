#ifndef SOURCE_H
#define SOURCE_H
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>

class Source
{
public:
	virtual ~Source() {}
	virtual char getNextChar() = 0;
	virtual char peekNextChar() = 0;
	virtual int getPosition() = 0;
};


class FileSource : public Source 
{
private:
	std::string _fileName;
	FILE* _file;
	int _position;
public:

	int getPosition();
	FileSource(std::string fileName);
	~FileSource() { fclose(_file); }
	FileSource(const FileSource& fileSource);
	FileSource& operator=(const FileSource& fileSource);
	char getNextChar();
	char peekNextChar();
};



class StreamSource : public Source 
{
private:
	std::string _buffer;
	std::istream& _stream;
	int _position;
public:

	int getPosition();
	StreamSource(std::istream& stream);
	char getNextChar();
	char peekNextChar();
};


typedef std::unique_ptr<Source> UniqueSource;
typedef std::unique_ptr<FileSource> UniqueFileSource;
typedef std::unique_ptr<StreamSource> UniqueStreamSource;
#endif