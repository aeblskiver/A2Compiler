#pragma once

#include "Lexer.h"
#include <stack>
#include <string>

enum kterm_T { Pgm, kwdprg, brace1, Slist, brace2, Stmt, semi, id, equal, Y1, S_out, E, kwdinput,
kwdprint, paren1, Elist, paren2, Elist2, comma, T, X1, Opadd, F, X2, Opmul, Fatom, _int, _float, _string, plus, minus, aster, slash}; //add all the nonterms

//Symbols that go into the nodes
struct A1_Symbol
{
	int index;  //I don't know what this is for
	string name; //Symbol's name
};

//Nodes that make up the parse tree
struct PSTNode
{
	A1_Symbol m_sym;  //RHS rule symbol
	PSTNode * pMom;   //Mom node
	vector<PSTNode *> pKids;  //Kid nodes
	int kidsCount = 0;    //How many kids
	int position = 0;     //Position in RHS rule. Should be identical to index in mom's kid list.
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
	void insertNodes(PSTNode *, A1_Symbol); //Making children for the mom node
	void printTree(PSTNode * root);  //Prints in post order but I don't think it's correct
	void printKids(PSTNode * root);
	void deleteTree(PSTNode * root); //In theory it deletes the tree
	bool isNonTerminal(PSTNode * root);
};

//Check if tree is empty idk why I implemented it
bool ParseTree::is_empty() const
{
	return (root == NULL);
}

/*insertNodes
@param PSTNode * root - The mom node where new kid nodes will be created
@param A1_Symbol symbol - The symbol making up the node idk
*/
void ParseTree::insertNodes(PSTNode * root, A1_Symbol symbol)
{
		//temp.m_Inonterm = static_cast<kterm_T>(rules[i].rule);
		PSTNode* tempNode = new PSTNode(symbol, root, symbol.index);
		root->pKids.push_back(tempNode);
		root->kidsCount += 1;
		cout << "Inserting node..." << endl;
		cout << "Node: " << tempNode->m_sym.name << endl;
}

//printTree - Prints the tree in preorder starting with the root node
//I don't know if it's supposed to print every node or just Nonterminal symbols
void ParseTree::printTree(PSTNode * root)
{
	if (root == NULL) return;
	if (isNonTerminal(root))
	{
		cout << "Node: rule #X: " << root->m_sym.name << " = ";
		printKids(root);
		cout << endl;
		for (int i = 0; i < root->kidsCount; i++)
		{
			printTree(root->pKids[i]);
		}
	}
	else
		cout << "Node: " << root->m_sym.name << endl;
}

void ParseTree::printKids(PSTNode * root)
{
	for (int i = 0; i < root->kidsCount; i++)
	{
		cout << root->pKids[i]->m_sym.name << " ";
	}
}

//In theory, deletes every node from tree
void ParseTree::deleteTree(PSTNode * root)
{
	if (root == NULL) return;
	else
	{
		for (int i = 0; i < root->pKids.size(); i++)
		{
			deleteTree(root->pKids[i]);
		}
		delete root;
		root = NULL;
	}
}

bool ParseTree::isNonTerminal(PSTNode * root)
{
	return (root->pKids.size() > 0);
}