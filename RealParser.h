#pragma once

#include "Parser.h"
#include <stack>
#include<type_traits>

struct ParserItem: Token
{
	bool isToken;
	string rule;
	int row = 0;
};

 


class RealParser
{
public:
	vector<Token> tokenList;
	int posCounter = 0; //keeps track of the position in the vector
	stack<ParserItem> machineStack;
	//int matrix[5][6];
	char PLACEHOLDER;//need to add a char rule holder to token class
	ParserItem item;
	int passCounter = 0;

	int matrix[15][18]=
			{// kwprg  b1   b2 sem id kwin kwp p1  p2 com int flo str plu min ast sla  car
	/*pgm*/		{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Slist*/	{ 0,	0,	3,	0,	2,	0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Stmt*/	{ 0,	0,	0,	0,	4,	0,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Y1*/		{ 0,	0,	0,	0,	6,	7,	0,	6,	0,	0,	6,	6,	6,	0,	0,	0,	0,	0 },
	/*S_Out*/	{ 0,	0,	0,	0,	0,	0,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*Elist*/	{ 0,	0,	0,	0,	9,	0,	0,	9, 10,	9,	9,	9,	9,	0,	0,	0,	0,	0 },
	/*Elist2*/	{ 0,	0,	0,	0,	0,	0,	0,	0, 13, 11,	0,	0,	0,	0,	0,	0,	0,	0 },
	/*E*/		{ 0,	0,	0,	0, 12,	0,	0, 12,	0,	0, 12, 12, 12,	0,	0,	0,	0,	0 },
	/*X1*/		{ 0,	0,	0, 13,	0,	0,	0,	0, 13, 13,	0,	0,	0, 14, 14,	0,	0,	0 },
	/*T*/		{ 0,	0,	0,	0, 15,	0,	0,	15,	0,	0, 15, 15, 15,	0,	0,	0,	0,	0 },
	/*X2*/		{ 0,	0,	0,	13,	0,	0,	0,	0, 16, 16,	0,	0,	0, 16, 16, 17, 17, 17 },
	/*F*/		{ 0,	0,	0,	0, 18,	0,	0, 19,	0,	0, 18, 18, 18,	0,	0,	0,	0,	0 },
	/*Fatom*/	{ 0,	0,	0,	0, 20,	0,	0,	0,	0,	0, 21, 22, 23,	0,	0,	0,	0,	0 },
	/*Opadd*/	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 24, 25,	0,	0,	0 },
	/*Opmul*/	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 26, 27, 28 },
		
	};

	


	RealParser(vector<Token> tokens)
	{
		ParserItem eof, pgm;
		eof.rule = "eof";eof.isToken = true;
		pgm.rule = "pgm";pgm.isToken = false;pgm.row = 1;
		machineStack.push(eof);
		machineStack.push(pgm);

		//machineStack.push("$");
		tokenList = tokens;
	}

	//Returns the next token in the list and increments the positional cursor
	Token GetNextToken()
	{
		return tokenList[posCounter];		
	}

	//reads in the next token and analyzes it against the stack
	void Machine()
	{
		while (posCounter != tokenList.size())
		{
			passCounter++;
			PrintStatus();
			int x, y;
			Token curToken = GetNextToken();
			ParserItem curPItem = machineStack.top();

			//compare curToken to the top of the stack and perform appropriate action
			if (curPItem.isToken == true)
			{ //check if the top of the stack is a token. if it is perform the correct action if it matches the next token
			  //else throw an error
				//cout<<endl << "*******" <<curPItem.rule<<"******"<< curToken.id << endl;
				if (curPItem.token == curToken.token)
				{
					machineStack.pop();
					posCounter++;
				}
				else if (curPItem.token == "string"&&curToken.id == 5)//else if string
				{
					machineStack.pop();
					posCounter++;
				}
				else if (curPItem.token == "float"&&curToken.id == 4)//else if float
				{
					machineStack.pop();
					posCounter++;
				}
				else if (curPItem.token == "int"&&curToken.id == 3)//else if int
				{
					machineStack.pop();
					posCounter++;
				}
				else if (curPItem.rule == "id"&&curToken.id == 2)//else if id
				{
					machineStack.pop();
					posCounter++;
				}
				else
				{
					Error(2);
					return;
				}
			}

			else if (matrix[curPItem.row-1][GetCol(tokenList[posCounter])-1] == 0)//Empty Matrix
			{
				Error(3);
				return;
			}
			else
			{
				vector<ParserItem> rules = ReturnRules(matrix[curPItem.row - 1][GetCol(tokenList[posCounter]) - 1]);
				machineStack.pop();
				//cout << endl <<"size of incoming rule list:"<<rules.size()<<"content of top rule"<<rules[0].token<< endl;
				if (rules.size()==1&&rules[0].rule =="eps")//epsilon rule
				{
					//do nothing;
				}
				else
				{
					//cout << endl << "*****PUSHING RULE: " << matrix[curPItem.row - 1][GetCol(tokenList[posCounter]) - 1] << endl;
					for (int i = 0;i < rules.size();i++)//push the rules/tokens to the stack.
					{
						
						machineStack.push(rules[i]);
					}
				}

			}
		}

	}
	//Prints the next item in the stack as well as the next item in the token stream.
	void PrintStatus()
	{
		ParserItem top = machineStack.top();
		cout << "***********Pass #"<<passCounter<<"****************************" << endl;
		cout << "*Top stack item               Top Token" << endl;
		cout << "*" <<top.rule<< "                        " << tokenList[posCounter].token << endl;
		cout << "***************************************";

	}
	//takes a token in and returns the corresponding column in the matrix
	int GetCol(Token token)
	{

		switch (token.id)
		{
		case 2:
			return 5;
			break;
		case 3:
			return 11;
			break;
		case 4:
			return 12;
			break;
		case 5:
			return 13;
			break;
		case 6:
			return 10;
			break;
		case 7:
			return 4;
			break;
		case 10:
			return 1;
			break;
		case 22:
			return 6;
			break;
		case 23:
			return 7;
			break;
		case 33:
			return 2;
			break;
		case 34:
			return 3;
			break;
		case 37:
			return 8;
			break;
		case 38:
			return 9;
			break;
		case 41:
			return 16;
			break;
		case 42:
			return 18;
			break;
		case 45:
			return 0; //need to fix = case
			break;
		case 46:
			return 15;
			break;
		case 47: 
			return 14;
			break;
		case 48:
			return 17;
			break;
		default:
			break;
		}

	}

	//receives a rule number from the matrix and returns as a vector of Parser items, the corresponding rule set.  
	vector<ParserItem> ReturnRules(int ruleNumber)
	{
		vector<ParserItem>returnRules;
		switch (ruleNumber)
		{
		case 1: 
		{
			ParserItem r4; r4.isToken = true;r4.rule = "brace2";r4.token = "}";
			ParserItem r3; r3.isToken = false;r3.rule = "slist";r3.row = 2;
			ParserItem r2; r2.isToken = true;r2.rule = "brace1";r2.token = "{";
			ParserItem r1; r1.isToken = true;r1.rule = "kwdprg";r1.token = "prog";
			returnRules.push_back(r4);returnRules.push_back(r3);returnRules.push_back(r2);returnRules.push_back(r1);
		}
		break;
		case 2:
		{
			ParserItem r3; r3.isToken = false;r3.rule = "slist";r3.row = 2;
			ParserItem r2; r2.isToken = true;r2.rule = "semi";r2.token = ";";
			ParserItem r1; r1.isToken = false;r1.rule = "stmt";r1.row = 3;
			returnRules.push_back(r3);returnRules.push_back(r2);returnRules.push_back(r1);
		}
		break;
		case 3:
		{
			ParserItem r1; r1.isToken = false;r1.rule = "eps";
			returnRules.push_back(r1);
		}
		break;
		case 4:
		{
			
			ParserItem r3; r3.isToken = true;r3.rule = "id";
			ParserItem r2; r2.isToken = true;r2.rule = "equal";r2.token = "=";
			ParserItem r1; r1.isToken = false;r1.rule = "y1";r1.row = 4;
			returnRules.push_back(r1);returnRules.push_back(r2);returnRules.push_back(r3);//inverted to fix push order
		}
		break;
		case 5:
		{
			ParserItem r1; r1.isToken = false;r1.rule = "s_out";r1.row = 5;
			returnRules.push_back(r1);
		}
		break;
		case 6:
		{
			ParserItem r1; r1.isToken = false;r1.rule = "e";r1.row = 8;
			returnRules.push_back(r1);
		}
		break;
		case 7:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "kwdinput";r1.token = "input";
			returnRules.push_back(r1);
		}
		break;
		case 8:
		{
			ParserItem r4;r4.isToken = true;r4.rule = "kwdprint";r4.token = "print";
			ParserItem r3; r3.isToken = true;r3.rule = "paren1"; r3.token = "(";
			ParserItem r2; r2.isToken = false;r2.rule = "elist";r2.row = 6;
			ParserItem r1; r1.isToken = true;r1.rule = "paren2"; r1.token = ")";
			returnRules.push_back(r1); returnRules.push_back(r2);returnRules.push_back(r3);returnRules.push_back(r4);//inverted to fix push order
		}
		break;
		case 9:
		{

			
			ParserItem r2; r2.isToken = false;r2.rule = "e";r2.row = 8;
			ParserItem r1; r1.isToken = false;r1.rule = "elist2";r1.row = 7;
			returnRules.push_back(r1);returnRules.push_back(r2); //inverted to fix push order
		}
		break;
		case 10:
		{

			ParserItem r1; r1.isToken = false;r1.rule = "eps";
			returnRules.push_back(r1);
		}
		break;
		case 11:
		{

			
			ParserItem r2; r2.isToken = true;r2.rule = "comma"; r2.token = ",";
			ParserItem r1; r1.isToken = false;r1.rule = "elist";r1.row = 6;
			returnRules.push_back(r1);returnRules.push_back(r2); //inverted to fix push order
		}
		break;
		case 12:
		{

			
			ParserItem r2; r2.isToken = false;r2.rule = "t"; r2.row = 10;
			ParserItem r1; r1.isToken = false;r1.rule = "x1";r1.row = 9;
			returnRules.push_back(r1);returnRules.push_back(r2);//inverted to fix push order
		}
		break;
		case 13:
		{

			
			ParserItem r1; r1.isToken = false;r1.rule = "eps";
			returnRules.push_back(r1);
		}
		break;
		case 14:
		{

			ParserItem r3; r3.isToken = false;r3.rule = "opadd"; r3.row = 14;
			ParserItem r2; r2.isToken = false;r2.rule = "t";r2.row = 10;
			ParserItem r1; r1.isToken = false;r1.rule = "x1";r1.row = 9;
			returnRules.push_back(r1);returnRules.push_back(r2);returnRules.push_back(r3); //inverted to fix push order
		}
		break;
		case 15:
		{

			
			ParserItem r2; r2.isToken = false;r2.rule = "f"; r2.row = 12;
			ParserItem r1; r1.isToken = false;r1.rule = "x2";r1.row = 11;
			returnRules.push_back(r1);returnRules.push_back(r2); //inverted to fix push order
		}
		break;
		case 16:
		{

			ParserItem r1; r1.isToken = false;r1.rule = "eps";
			returnRules.push_back(r1);
		}
		break;
		case 17:
		{

			ParserItem r3; r3.isToken = false;r3.rule = "opmul"; r3.row = 15;
			ParserItem r2; r2.isToken = false;r2.rule = "f";r2.row = 12;
			ParserItem r1; r1.isToken = false;r1.rule = "x2"; r1.row = 11;
			returnRules.push_back(r1);returnRules.push_back(r2);returnRules.push_back(r3);//inverted to fix push order
		}
		break;
		case 18:
		{

			ParserItem r1; r1.isToken = false;r1.rule = "fatom"; r1.row = 13;
			returnRules.push_back(r1);
		}
		break;
		case 19:
		{

			ParserItem r3; r3.isToken = true;r3.rule = "paren1"; r3.token = "(";
			ParserItem r2; r2.isToken = false;r2.rule = "e";r2.row = 8;
			ParserItem r1; r1.isToken = true;r1.rule = "paren2"; r1.token = ")";
			returnRules.push_back(r1);returnRules.push_back(r2);returnRules.push_back(r3); //inverted to fix push order
		}
		break;
		case 20:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "id";r1.token = "id";
			returnRules.push_back(r1);
		}
		break;
		case 21:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "int";r1.token = "int";
			returnRules.push_back(r1);
		}
		break;
		case 22:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "float";r1.token = "float";
			returnRules.push_back(r1);
		}
		break;
		case 23:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "string";r1.token = "string";
			returnRules.push_back(r1);
		}
		break;
		case 24:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "plus";r1.token = "+";
			returnRules.push_back(r1);
		}
		break;
		case 25:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "minus";r1.token = "-";
			returnRules.push_back(r1);
		}
		break;
		case 26:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "aster";r1.token = "*";
			returnRules.push_back(r1);
		}
		break;
		case 27:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "slash";r1.token = "/";
			returnRules.push_back(r1);
		}
		break;
		case 28:
		{
			ParserItem r1; r1.isToken = true;r1.rule = "caret";r1.token = "^";
			returnRules.push_back(r1);
		}
		break;
		
		default:
			break;
		}

		return returnRules;
	}

	void Error(int code)
	{
		switch (code)
		{
		case 2:cout <<endl << tokenList[posCounter].lineNumber << ": " << "M2 ERROR: Top is Token" << endl;
			break;
		case 3:cout << endl <<tokenList[posCounter].lineNumber<<": "<< "M3 ERROR: Empty Matrix" << endl;
			break;
		default:cout << endl << tokenList[posCounter].lineNumber << ": " << "GENERAL ERROR" << endl;
			break;

			}

		cout << "remaning items on Stack:" << endl;
		while (machineStack.size() != 0)
		{
			ParserItem p = machineStack.top();
			machineStack.pop();
			cout << p.rule << endl;

		}

	}

};