/*
    Project #1: Lexer
	 CPSC 323
	 September 23, 2016
    Team CCJ- Chris Leonardi and Justin Shelley

	 A lexical analyzer for scanning and tokenizing source files. 
*/

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

