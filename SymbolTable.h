/*
Project #3: PST + AST + Symtab
CPSC 323
November 13, 2016
Team CCJ- Chris Leonardi and Justin Shelley


A class for creating the symbol table
Author: Chris Leonardi
*/

#pragma once
#include <iostream>
#include <vector>
#include "Lexer.h"
using namespace std;

//struct for Symbole Table items. Holds the name of the ID type as well as each token. 
struct SymbolTableItem
{
	string ID;
	vector<Token> appearences;
};

//symbol table class that builds and manages the symbol table. 
class SymbolTable
{
	vector<SymbolTableItem> symbolTable;

	//takes a list of tokens and builds a symbol table from it.
public:void BuildSymbolTable(vector<Token>tokens)
	{
		//testing token list for valid data
		
		for (int i = 0;i < tokens.size();i++)
		{
			if (tokens[i].id == 2)
			{
				//cout << tokens[i].lineNumber;
				//cout << "Found valid token:" + tokens[i].token << "at line "<<to_string(tokens[i].lineNumber) << "and position "<<to_string(tokens[i].loction) << endl;
				AddToTable(tokens[i]);
			}
		}

	}

	   //Add an item to the table. First it checks to see if ID is already present, if not then it creates an entry. 

	void AddToTable(Token idToken)
	{
		//first check to makes sure that particular ID doesn't already exist on the table. if it does, add this token to that vector
		for (int i = 0;i < symbolTable.size();i++)
		{
			if (symbolTable[i].ID == idToken.token)
			{
				symbolTable[i].appearences.push_back(idToken);
				return;
			}
		}

		//The id does not exist on the talbe. Make a new entry for it and add it to the table
		SymbolTableItem sym;
		sym.ID = idToken.token;
		sym.appearences.push_back(idToken);
		symbolTable.push_back(sym);
	}


	//Go through the symbole table and print all ID's
	void PrintTable()
	{
		for (int i = 0;i < symbolTable.size();i++)
		{
			cout <<endl<< "( " + symbolTable[i].ID + ":";
			for (int j = 0;j < symbolTable[i].appearences.size();j++)
			{
				cout << "(Line: "<<to_string(symbolTable[i].appearences[j].lineNumber)<< " Col: " << to_string(symbolTable[i].appearences[j].column) << " )";
			}
		}
	}


};
