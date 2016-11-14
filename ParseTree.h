/*
	Project #3: PST + AST + Symtab
	CPSC 323
	November 13, 2016
	Team CCJ- Chris Leonardi and Justin Shelley

	
	A class for generating parse trees and converting them to an abstract syntax tree.
	Author: Justin Shelley
*/

#pragma once

#include "Lexer.h"
#include <stack>
#include <string>


//Symbols that go into the nodes
struct A1_Symbol
{
	int index;  //Index in rule 
	string name; //Symbol's name
	int ruleID; //Rule ID the toke is part of
};

//Nodes that make up the parse tree
struct PSTNode
{
	Token nodeToken; //Token in the node
	int creationIndex; //Order node was created
	A1_Symbol m_sym;  //RHS rule symbol
	int ruleNumber = 0; //Node's ruler number
	PSTNode * pMom;   //Mom node
	vector<PSTNode *> pKids;  //Kid nodes
	int kidsCount = 0;    //How many kids
	int position = 0;     //Position in RHS rule. Should be identical to index in mom's kid list.
	PSTNode(A1_Symbol symbol_val, PSTNode * mom_val, int pos_val) : m_sym(symbol_val), pMom(mom_val), 
		position(pos_val) {};
	
};

/*
	Class ParseTree
	Responsible for generating parse tree, converting to AST, and deleting the tree.

	@author Justin Shelley
*/
class ParseTree
{
public: 
	int indexCount = 0; //Node index
	PSTNode * root; //Pointer to root of three
	ParseTree() : root(NULL) {};  //Default constructor, set root to null.
	bool is_empty() const; //Check for empty tree
	void insertNodes(PSTNode *, A1_Symbol); //Making children for the mom node
	void printAST(PSTNode * root); //Prints AST
	void printTree(PSTNode * root);  //Print parse tree
	void printKids(PSTNode * root);  //Prints a node's kids
	void deleteTree(PSTNode * root); //Deletes the tree
	bool isNonTerminal(PSTNode * root); //Checks if a node has kids (nonterminal)
	void P2AST(PSTNode * root);  //Convert tree to AST
	void P2AST_Convert(PSTNode * root); //Helper function for P2AST
	bool isTerminal(PSTNode * root); //Checks if a node is a token identifier
};

//Check if tree is empty idk why I implemented it
bool ParseTree::is_empty() const
{
	return (root == NULL);
}

/*insertNodes
@param PSTNode * root - The mom node where new kid nodes will be created
@param A1_Symbol symbol - The symbol making up the node

@author Justin Shelley
*/
void ParseTree::insertNodes(PSTNode * root, A1_Symbol symbol)
{
		//temp.m_Inonterm = static_cast<kterm_T>(rules[i].rule);
		PSTNode* tempNode = new PSTNode(symbol, root, symbol.index);
		tempNode->creationIndex = indexCount;
		indexCount++;
		root->pKids.push_back(tempNode);
		root->kidsCount += 1;
		cout << "Inserting node..." << endl;
		cout << "Node: " << tempNode->m_sym.name << endl;
}

/*
	printAST - Prints the abstract syntax tree in preorder starting with the root node
	@param root - Root of the tree
	@author Justin Shelley
*/
void ParseTree::printAST(PSTNode * root)
{
	if (root == NULL) return;
	if (isNonTerminal(root))
	{
		cout << "(N " << root->creationIndex << ": " << root->m_sym.name << " mom= " << root->pMom->creationIndex << " kids= " << root->pKids.size() 
			<< ")"<< endl;
		cout << endl;
		for (int i = 0; i < root->pKids.size(); i++)
		{
			printAST(root->pKids[i]);
		}
	}
	else if (isTerminal(root))
	{
		cout << "(N: " << root->creationIndex << ": " << root->m_sym.name << "  "
			<< "mom= " << root->pMom->creationIndex << " kids= 0"
			<< "(T: \"" << root->nodeToken.token << "\"" << endl;
	}
	else
	{
		cout << "(N: " << root->creationIndex << ": " << root->m_sym.name << "  "
			<< "mom= " << root->pMom->creationIndex << " kids= " << root->pKids.size() << endl;
	}
}

/*
	printTree - Prints the parse tree in pre-order
	@param root - Root of the tree
	@author Justin Shelley
*/
void ParseTree::printTree(PSTNode * root)
{
	if (root == NULL) return;
	if (isNonTerminal(root))
	{
		cout << "(N:  rule = #" << root->ruleNumber << ": " << root->m_sym.name << " = ";
		printKids(root);
		for (int i = 0; i < root->pKids.size(); i++)
		{
			printTree(root->pKids[i]);
		}
	}
}

/*
	printKids - helper function for printTree
	@param root - Root of tree
	@author Justin Shelley
*/
void ParseTree::printKids(PSTNode * root)
{
	for (int i = 0; i < root->pKids.size(); i++)
	{
		cout << root->pKids[i]->m_sym.name << " ";
	}
	cout << ")" << endl;
}

/*
	deleteTree - Free up memory and set pointers to NULL
	@param root - Root of tree
	@author Justin Shelley
*/
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

/*
	isNonTerminal - Checks the node to see if it has kids (aka nonterminal)
	@return bool - Yes if node is nonterminal, no otherwise
	@param root - Node to check
	@author Justin Shelley
*/
bool ParseTree::isNonTerminal(PSTNode * root)
{
	return (root->pKids.size() > 0);
}

/*
	P2AST - Parse 2 Abstract Syntax Tree. Goes through each node to apply conversion function
	@param root - Root of tree
	@author Justin Shelley
*/
void ParseTree::P2AST(PSTNode * root)
{
	if (root == NULL) return;
	else
	{
		for (int i = 0; i < root->pKids.size(); i++)
		{
			P2AST(root->pKids[i]);
		}
		if (isNonTerminal(root))
			P2AST_Convert(root);
	}
}

/*
	P2AST_Convert - Helper function for P2AST. Rule id designated which block modification to apply
	@param root - Node to be converted
	@author Justin Shelley
*/
void ParseTree::P2AST_Convert(PSTNode * root)
{
	PSTNode * gma = root->pMom;
	PSTNode * $$ = root;
	PSTNode * $1;
	PSTNode * $2;
	PSTNode * $3;
	PSTNode * $4;
	int ruleID = root->ruleNumber;
	switch (ruleID)
	{
	case 1:
		$1 = $$->pKids[0];
		for (int i = 1; i < $$->pKids.size(); i++)
		{
			if (isNonTerminal($$->pKids[i]))
			{
				$1->pKids.push_back($$->pKids[i]);
				$$->pKids[i]->pMom = $1;
			}
		}//Link kwdpg to Slist
		delete $$;
		this->root = $1; //Make kwdprg the root
		this->root->pMom->creationIndex = -1;
		break;
	case 2:
		if ($$->pKids.size() >= 4)
		{
			$1 = $$->pKids[0];
			$2 = $$->pKids[2];
			$3 = $$->pKids[3];
			gma->pKids[$$->position] = $1;
			$1->pMom = gma;
			for (int i = 1; i < $$->pKids.size(); i++)
			{
				if (isNonTerminal($$->pKids[i]))
					gma->pKids.push_back($$->pKids[i]);
				$$->pKids[i]->pMom = gma;
			}
		}
		else
		{
			$1 = root->pKids[0];
			$2 = root->pKids[2];
			gma->pKids[$$->position] = $1; gma->pKids.push_back($2); //Connect kwdprg to Stmt and Slist
			$1->pMom = gma; $2->pMom = gma;
		}
		$$->pKids.clear(); delete $$;
		break;
	case 3:
		break;
	case 4:
		$1 = root->pKids[0];
		$2 = root->pKids[1];
		$3 = root->pKids[2];
		$2->pKids.push_back($1); $2->pKids.push_back($3);
		$1->pMom = $2; $3->pMom = $2;
		gma->pKids[$$->position] = $2;
		$2->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 5:
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids[0]->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 6:
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids[0]->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 7:
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids[0]->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 8:
		$1 = $$->pKids[0]; //kwdprint
		$2 = $$->pKids[2];
		$3 = $$->pKids[4];
		$$->pKids.clear();
		if ($3->ruleNumber == 0)
		{
			$1->pKids.push_back($2);
			$2->pMom = $1;
			gma->pKids[$$->position] = $1;
			$1->pMom = gma;
		}
		else
		{
			$1->pKids.push_back($2); $2->pMom = $1;
			$1->pKids.push_back($3); $3->pMom = $1;
			gma->pKids[$$->position] = $1; $1->pMom = gma;
		}

		$$->pKids.clear();  delete $$;
		break;
	case 9:
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		gma->pKids.push_back($2); $2->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 10:
		break;
	case 11:
		$2 = $$->pKids[1];
		gma->pKids[1] = $2; $2->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 12:
		$1 = $$->pKids[0];
		if ($$->pKids.size() > 1)
		{
			$2 = $$->pKids[1]; 
			if ($2->ruleNumber == 0)
			{
				gma->pKids[$$->position] = $1; $1->pMom = gma;
				
			}
			else
			{
				$2->pKids.insert($2->pKids.begin(), $1); $1->pMom = $2;
				gma->pKids[$$->position] = $2; $2->pMom = gma;
				

			}
			$$->pKids.clear(); delete $$;
		}
		else
		{
			gma->pKids[$$->position] = $1; $1->pMom = gma;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 13:
		break;
	case 14:
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		$3 = $$->pKids[2];
		if ($3->m_sym.name == "eps")
		{
			$1->pKids.push_back($2); $2->pMom = $1;
			gma->pKids[$$->position] = $1; $1->pMom = gma;
			$$->pKids.clear(); delete $$;
		}
		else
		{
			gma->pKids[$$->position] = $1; $1->pMom = gma;
			$1->pKids.push_back($2); $1->pKids.push_back($3); $2->pMom = $1; $3->pMom = $1;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 15:
		$1 = $$->pKids[0];
		if ($$->pKids.size() > 1)
		{
			$2 = $$->pKids[1];
			if ($2->ruleNumber == 0)
			{
				gma->pKids[$$->position] = $1; $1->pMom = gma;
				$$->pKids.clear(); delete $$;
			}
			else
			{
				$2->pKids.insert($2->pKids.begin(), $1); $1->pMom = $2;
				gma->pKids[$$->position] = $2; $2->pMom = gma;
				$$->pKids.clear(); delete $$;
			}
		}
		else
		{
			gma->pKids[$$->position] = $1; $1->pMom = gma;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 16:
		break;
	case 17:
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		if ($$->pKids.size() > 2)
		{
			$3 = $$->pKids[2];
			if ($3->m_sym.name == "eps")
			{
				$1->pKids.push_back($2); $2->pMom = $1;
				gma->pKids[$$->position] = $1; $1->pMom = gma;
				$$->pKids.clear(); delete $$;
			}

			else
			{
				gma->pKids[$$->position] = $1; $1->pMom = gma;
				$1->pKids.push_back($2); $1->pKids.push_back($3); $2->pMom = $1; $3->pMom = $1;
				$$->pKids.clear(); delete $$;
			}
		}
		else
		{
			$1->pKids.push_back($2); $2->pMom = $1;
			gma->pKids[$$->position] = $1; $1->pMom = gma;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 18:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 19:
		$1 = $$->pKids[1];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 20:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 21:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 22:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 23:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 24:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 25:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 26:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 27:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	case 28:
		$1 = $$->pKids[0];
		gma->pKids[$$->position] = $1; $1->pMom = gma;
		$$->pKids.clear(); delete $$;
		break;
	default:
		break;
	}
}

/*
	isTerminal - Checks if node is a token identifier
	@param root - Node to check
	@author Justin Shelley
*/
bool ParseTree::isTerminal(PSTNode * root)
{
	if (root->m_sym.name == "id" || root->m_sym.name == "int" || root->m_sym.name == "float" || root->m_sym.name == "string")
		return true;
	else return false;
}