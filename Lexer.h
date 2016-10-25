#pragma once

/*
	CPSC 323 Project #1: Lexer
	The parser class is responsible for scanning a text file and tokenizing the input.

	@author Chris Leonardi, Justin Shelley
	@version 1.0 9/23/2016
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stack>
#include <unordered_map>

using namespace std;


//Struct for token information
struct Token
{
	string token;
	int id;
	int lineNumber;
	
};


//Struct for recognizing keywords
struct Keyword
{
	string key;
	int id;
};


//Parser class for tokenizing input
class Lexer
{
public:
	const char tblColumn[21] = { '_', '.', ',', ';','<','>','+','-','*','/','^',':',
		'{','}','[',']','(',')','!','=',' ' };//Used to check which column to set for state transition table
	const int accStates[15] = {  -1,2,5,6,11,12,14,15,16,18,20,35,36,37,38 }; //List of accepting states
	const std::unordered_map<string, int> opList = { { ",",6 },{ ";",7 },{ "<",31 },{ ">",32 },{ "{",33 },{ "}",34 },
	{ "[",35 },{ "]",36 },{ "(",37 },{ ")",38 },{ "*",41 },{ "^",42 },{ ":",43 },{ ".",44 },{ "=",45 },{ "-",46 },
	{ "+",47 },{ "/",48 },{ "->",51 },{ "==",52 },{ "!=",53 },{ "<=",54 },{ ">=",55 },{ "<<",56 },{ ">>",57 }}; //For token IDs

	int stateTable[39][23];
	vector<Token> tokens;
	vector<Keyword> keyList;
	int lineNumber = 1;
	int tokenCount = 0;

	/*
	    Default constructor
		 Initializes key word list by reading in from Keywords.txt

		 @author Chris Leonardi
	*/
	Lexer()
	{
		ifstream keys;
		keys.open("Keywords.txt");
		if (!keys)
			cout << "ERROR OPENING FILE!!!";
		while (!keys.eof())
		{
			string keyS, idS;
			keyList.emplace_back(Keyword());
			keys >> keyS; //keyList[keyList.size - 1].key;
			keys >> idS; //keyList[keyList.size - 1].id				
			keyList[keyList.size() - 1].key = keyS;
			keyList[keyList.size() - 1].id = stoi(idS);
		}
	}
	
	/*
	    AddToken accepts a lexeme and id and adds it to the token list

		 @param token The lexeme that has been tokenized
		 @param id The ID for the relevant token

		 @author Chris Leonardi
	*/
	void AddToken(string token, int id)
	{
		tokens.emplace_back(Token());
		tokens[tokenCount].id = id;
		tokens[tokenCount].token = token;
		tokens[tokenCount].lineNumber = lineNumber;
		tokenCount++;
	}

	/*
		Prints out a list of all the tokens and info associated with them.
		@author Chris Leonardi
	*/
	void PrintTokens()
	{
		for (int i = 0; i < tokenCount; i++)
		{
			cout << "(Tok: ";// +;
			cout << tokens[i].id;
			cout << string(" line: ") + to_string(tokens[i].lineNumber) + " str= " + tokens[i].token << endl;
		}
	}

	/*
	    Load a text file and scan line by line for tokens
		 @param fileName The name of the file to be read
		 @author Justin Shelley
	*/
	void scanFile(string fileName)
	{
		string textLine;

		ifstream srcFile;
		srcFile.open(fileName);

		while (getline(srcFile, textLine))
		{			
			next_token(textLine);
			lineNumber++;
		}
		if (srcFile.eof())
		{
			--lineNumber;
			AddToken("", 0);
		}
	}

	/*
	    Scans input file and grabs tokens
		 @param textLine The line that has been grabbed from the file 
		 @author Justin Shelley and Chris Leonardi
	*/
	void next_token(string textLine)
	{
		char cc;		//Current character
		int state = 0;	//State table row
		int col;		//State table column
		int start = 0;
		int current = 0;
		string lexeme;

		cc = textLine[current];
		while (state != -1)
		{
			cc = textLine[current];				//Current character
			if (cc == '\0' || cc == '\n')		//Check for end of file or end of string
			{
				state = -1;
			}
			
			else
			{
				if (isalpha(cc) || cc == '_')
				{
					current = KeywordCheck(textLine, current);
					start = current;
					continue;
				}
				else if (cc == '/' && textLine[current + 1] == '/')
				{
					break;
				}
				else if (cc == '\"')
				{
					current = StringRead(textLine, current);
					start = current;
					continue;
				}
				else if (isdigit(cc))		  col = 1;
				else col = findColumn(cc);
				//cout << "state= " << state << " " << col;
				state = changeTableState(state, col);	//Change state
			}

			if (isAccepting(state))
			{
				
				lexeme = textLine.substr(start, current - start);
				//cout << "Lexeme: " << lexeme << endl;
				if (state < 7 && state > 0) AddToken(lexeme, getID(state));	//If it's an identifier, int or float
				else AddToken(lexeme, getOpID(lexeme));					//For delimiters, operators, and other punctuation
				start = current;
				current--;
				(state == -1 ? state = -1 : state = 0); //Check for end of line and reset start

			}
			current++;

			(state == 0 ? start = current : start = start);

		}

	}

	/*
	    Reads a lexeme that has been identified as being part of a string
		 @param line The string lexeme
		 @param current The current position in the character string
		 @return The new position in character string
		 @author Chris Leonardi
	*/
	int StringRead(string line, int current)
	{
		int cur = current + 1;
		string token = "";
		while (true)
		{
			if (line[cur] != '\"')
				token = token + line[cur];
			else
				break;
			cur++;
		}		
		AddToken(token, 5);
		return cur + 1;
	}

	/*
	    Checks if identifier is a keyword
		 @param line The identifier lexeme being checked
		 @param current The current position in character string
		 @return The new position in character string
		 @author Chris Leonardi
	*/
	int KeywordCheck(string line, int current)
	{
		//cout << "entered keywordCheck";
		int cur = current;
		string token = "";
		while (true)
		{
			if (isalnum(line[cur]))
				token = token + line[cur];
			else
				break;
			cur++;
		}

		//check token against keyword list. If match found, add to token list as that keyword, else add as an identifier
		//then return with new cursor position. 
		for (size_t i = 0;i < keyList.size();i++)
		{
			if (token == keyList[i].key)
			{

				AddToken(keyList[i].key, keyList[i].id);
				return cur;
			}
		}

		AddToken(token, 2);
		return cur;

	}

	/*
	    Initializes the state table with data from text file
		 @author Justin Shelley
	*/
	void loadTable()
	{
		ifstream tableFile;
		tableFile.open("FStbl.txt");
		if (tableFile) std::cout << "Successfully loaded state transition table" << endl;
		for (int i = 0; i < 39; i++)
		{
			for (int j = 0; j < 23; j++)
			{
				tableFile >> stateTable[i][j];
			}
		}
		tableFile.close();
	}

	/*
	    Changes the state of the table while scanning characters
		 @param row Row of state table
		 @param col Column of state table
		 @author Justin Shelley
	*/
	int changeTableState(int row, int col)
	{
		try {
			return stateTable[row][col];
		}
		catch (std::out_of_range& value)
		{
			std::cerr << "Out of range";
			std::cerr << value.what() << endl;
			std::abort();
		}
	}

	/*
	    Find column index. Index is offset by two due to letters 
		 and digits taking first two columns in state table
		 @param c The character currently being examined
		 @return The new state of the table
		 @author Justin Shelley
	*/
	int findColumn(char c)
	{
		for (int i = 0; i < sizeof(tblColumn); i++)
		{
			if (tblColumn[i] == c)
				return i + 2;
		}
		return 0;
	}

	/*
	    Check if the state is an accepting state or not
		 @param state The current state of the table
		 @return True if accepting, False if not
		 @author Justin Shelley
	*/
	bool isAccepting(int state)
	{
		for (int i = 0; i < 15; i++)
		{
			if (accStates[i] == state)
				return true;
		}
		return false;
	}

	/*
	    Gets proper IDs for floats and ints. If I had set up state table properly this could have been avoided, oh well!
		 @param state The current state of the table.
		 @return the Correct ID for the token
		 @author Justin Shelley
	*/
	int getID(int state)
	{
		switch (state) {
		case 5:
			return 3;
		case 6:
			return 4;
		default:
			return state;
		}
	}
	
	/*
	    Checks the lexeme against the list of operators and returns the token ID
		 @param op The non-keyword token
		 @return Token ID
		 @author Justin Shelley
	*/
	int getOpID(string op)
	{
		if (opList.count(op) == 0) return 99; //ERROR, unknown token
		else return opList.find(op)->second;
	}
};