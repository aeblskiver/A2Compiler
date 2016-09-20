#include "Parser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include "main.h"

void next_token(ifstream);

void main()
{
					//The state transition table
				//File containing state transition table data

	Parser x;

	x.loadTable();

	x.AddToken("add", 56);
	x.AddToken("sub", 57);
	x.AddToken("mult", 54);
	x.AddToken("divide", 55);

	x.PrintTokens();

	assert(x.findColumn('.') == 3);
	assert(x.findColumn('=') == 21);
	assert(x.findColumn('*') == 10);

	assert(x.changeTableState(0, 0) == 1);
	assert(x.changeTableState(38, 21) == 0);
	assert(x.changeTableState(4, 2) == 6);

	x.next_token("float hello = 34.5; int x = (6*8);");


	system("PAUSE");
}

