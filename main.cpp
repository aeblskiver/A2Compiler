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
	
	x.scanFile("EX1.txt");
	x.PrintTokens();


	


	system("PAUSE");
}

