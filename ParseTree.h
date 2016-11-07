#pragma once

#include "Lexer.h"
#include <stack>
#include <string>

enum kterm_T { Pgm, kwdprg, brace1, Slist, brace2, Stmt, semi, id, equal, Y1, S_out, E, kwdinput,
kwdprint, paren1, Elist, paren2, Elist2, comma, T, X1, Opadd, F, X2, Opmul, Fatom, 
paren1, paren2, id, _int, _float, _string, plus, minus, aster, slash}; //add all the nonterms

//Symbols that go into the nodes
struct A1_Symbol
{
	int index;  //I don't know what this is for
	string name; //Symbol's name
	kterm_T m_Inonterm;
};

//Nodes that make up the parse tree
struct PSTNode
{
	A1_Symbol m_sym;  //RHS rule symbol
	PSTNode * pMom;   //Mom node
	vector<PSTNode *> pKids;  //Kid nodes
	int kidsCount;    //How many kids
	int position;     //Position in RHS rule. Should be identical to index in mom's kid list.
	PSTNode(A1_Symbol symbol_val, PSTNode * mom_val, int pos_val) : m_sym(symbol_val), pMom(mom_val), 
		position(pos_val) {};
};

//Parse tree
class ParseTree
{
public: 
	PSTNode * root;
	ParseTree() : root(NULL) {};  //Default constructor, set root to null.
	bool is_empty() const; //Check for empty tree
	bool insertNodes(PSTNode *, vector<ParserItem>);
};

//Check if tree is empty
bool ParseTree::is_empty() const
{
	return (root == NULL);
}

bool ParseTree::insertNodes(PSTNode * root, vector<ParserItem> rules)
{
	for (int i = 0; i < rules.size; i++)
	{
		A1_Symbol temp;
		temp.index = i;
		temp.name = rules[i].rule;
		//temp.m_Inonterm = static_cast<kterm_T>(rules[i].rule);
		PSTNode* tempNode = new PSTNode(temp, root, i);
		root->pKids.push_back(tempNode);
	}
}

