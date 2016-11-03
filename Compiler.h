#pragma once
#include "CParser.h"
#include "Lexer.h"

//Compiler class is where all the different components are called from, handles the menu, stores data used between multiple components, and does error handling.
class Compiler
{
	string fileName="No File Loaded"; //the filename the compiler is currently working with
	Lexer lexer; //the lexer
	Parser parser; //the parser
	string output; //recieves strings from the other components for output
	vector<Token> tokens; //the token list of the file most recently run through the parser. 
	
	//takes a string of a file to be opened and checks to see if the file is valid
	void LoadFile(istream& input)
	{
		string filename;
		input >> filename;
		ifstream file(filename);
		if (file)
			fileName = filename;
		else
			Error(1, 0, filename);
		file.close();
	}

	//main menu for the compiler. 
public: void menu()
	{
		int choice;
		while (true)
		{
			system("cls");
			cout << "************************A2 Compiler************************" << endl;
			cout << "	1. Load a file		" << endl;
			cout << "	2. Run Lexer		" << endl;
			cout << "	3. Run Parser		" << endl;
			cout << "	4. Settings			" << endl;
			cout << "	5. Exit				" << endl;
			cout << "	Currently Loaded File: " + fileName << endl;
			cout << "***********************************************************" << endl << endl;
			cout << "Make a selection: ";
			cin >> choice;
			system("cls");
			switch (choice)
			{
			case 1:
				cout << "Enter the name of the file you would like to open: ";
				LoadFile(cin);
				break;
			case 2:
				Lexer();
				break;
			case 3:
				Parser();
				break;
			case 5:
				return;
				break;
			default:
				break;
			}
			system("PAUSE");
		}


	}
	//run the currently loaded file through the lexer
	void Lexer()
	{
		
		if (!lexer.InitLexer())
			Error(lexer.errorcode, lexer.lineNumber, lexer.errorFile);
		else
			tokens=lexer.scanFile(fileName);

		tokens = lexer.tokens;
	}

	//run the currently loaded file through the parser
	void Parser()
	{
		
		cout << endl << tokens.size() << endl;
		if (!parser.ParserInit(tokens))
			Error(0, 0, 0);
		else
			parser.Machine();
	}

	void Error(int errorCode,int lineNo,string Filename="")
	{
		switch (errorCode)
		{
		case 1:
			cout << "ERROR 1: Cannot find file: "+Filename;
			break;
		default: //invalid error code sent
			cout << "ERROR: General error";
			break;
		}
	}

	//print the currently loaded output.
	void Print()
	{
		cout << output;
	}
};