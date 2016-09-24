#include "Parser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include "main.h"

void next_token(ifstream);

void main()
{
	//Create a parser object to work on files.
	Parser x;
	//Load the state table
	x.loadTable();
	
	/*
	    Example of reading file and printing out the tokens.
		 You can change the filename here to scan a file of your choosing.
	*/ 
	x.scanFile("EX1.txt");
	x.PrintTokens();

	system("PAUSE");
}

