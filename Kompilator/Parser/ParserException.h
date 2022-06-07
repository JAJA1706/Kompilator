#pragma once
#include <exception>

class ParserException : public std::exception {
    const int _position;

public:
    ParserException(const char* msg, const int position = 0) : std::exception(msg), _position(position) {}

    int getPosition() { return _position; }

};