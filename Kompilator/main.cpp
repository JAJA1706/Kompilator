#include <iostream>
#include <variant>
#include <memory>
#include <sstream>
#include <vector>
#include "Lekser\Token.h"
#include "Lekser\Lekser.h"
#include "Parser\Parser.h"
#include "Program\Program.h"

using namespace std;

int main( int argc, char** argv)
{

	UniqueFileSource source = make_unique<FileSource>("F:\\source\\TKOM\\Lekser\\Lekser\\przykladowyKod.txt");
	unique_ptr<Lekser> lekser(make_unique <Lekser>(move(source)));
	Parser parser(lekser.get());
	Program program = parser.parseProgram();
	program.execute();
	Stack stack = program.getStack();

    return 0;
}
