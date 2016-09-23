#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;
struct Token
{	 
	string token;
	int id;
	int lineNumber;
};
struct Keyword
{
	string key;
	int id;
};

class Parser
{
public:
	const char tblColumn[21] = { '_', '.', ',', ';','<','>','+','-','*','/','^',':',
		'{','}','[',']','(',')','!','=',' ' };//Used to check which column to set for state transition table
	const int accStates[15] = {-1, 2,5,6,11,12,14,15,16,18,20,35,36,37,38 };


	int stateTable[39][23];
	vector<Token> tokens;
	vector<Keyword> keyList;
	int lineNumber = 1;
	int tokenCount = 0;
		
		Parser()
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
					keyList[keyList.size() - 1].key=keyS;
					keyList[keyList.size() - 1].id = stoi(idS);

			}

		}
	//Adds a new token to the array
	void AddToken(string token, int id)
		{
			tokens.emplace_back(Token());		
			tokens[tokenCount].id = id;
			tokens[tokenCount].token = token;
			tokens[tokenCount].lineNumber = lineNumber;
			tokenCount++;
		}

	//prints out a list of all the tokens and info associated with them.
	void PrintTokens()
	{
		for (int i = 0; i < tokenCount; i++)
		{
			cout << "(Tok: ";// +;
			cout << tokens[i].id;
			cout << string(" line: ") + to_string(tokens[i].lineNumber) + " str= " + tokens[i].token << endl;
		}
	}


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
	}

	//Scans input file and grabs tokens
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
					current=KeywordCheck(textLine, current);
					start = current;
					continue;
				}
				else if (cc == '\"')
				{
					current=StringRead(textLine, current);
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
				AddToken(lexeme, state);
				start = current;
				//if (state < 7 ? current-- : current++);
				current--;
				if (state == -1)
					AddToken("\n", 1);
				(state == -1 ? state = -1 : state = 0);
				
			}
			current++;
			
			(state == 0 ? start = current : start = start);

		}
		
	}

	int StringRead(string line, int current)
	{
		int cur = current+1;
		string token = "";
		while (true)
		{
			//cout << endl << line<< endl;
			if (line[cur] != '\"')
				token = token + line[cur];
			else
				break;
			cur++;
		}
		AddToken(token, 5);
		return cur+1;
	}

	//Upon reading in an alpha, check the entire resulting string until a delimiter and compare against keyword list. 
	int KeywordCheck(string line,int current)
	{
		cout << "entered keywordCheck";
		int cur = current;
		string token="";
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
		for (int i = 0;i < keyList.size();i++)
		{
		   if (token==keyList[i].key)
			{
				
				AddToken(keyList[i].key,keyList[i].id);
				return cur;
			}
		}
		
		AddToken(token, 2);
		return cur;

	}
	
	//Populating table with data
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

	//Get new Table state
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

	//Find column index
	//Index is offset by two due to letters and digits taking first two columns in state table
	int findColumn(char c)
	{
		for (int i = 0; i < sizeof(tblColumn); i++)
		{
			if (tblColumn[i] == c)
				return i+2;
		}
	}
	
	//See if the state is an accepting state or not
	bool isAccepting(int state)
	{
		for (int i = 0; i < 15; i++)
		{
			if (accStates[i] == state)
				return true;
		}
		return false;
	}

};
