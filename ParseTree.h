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
	int ruleID;
};

//Nodes that make up the parse tree
struct PSTNode
{
	Token nodeToken;
	int creationIndex;
	A1_Symbol m_sym;  //RHS rule symbol
	int ruleNumber = 0;
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
	int indexCount = 0;
	PSTNode * root;
	ParseTree() : root(NULL) {};  //Default constructor, set root to null.
	bool is_empty() const; //Check for empty tree
	void insertNodes(PSTNode *, A1_Symbol); //Making children for the mom node
	void printAST(PSTNode * root);
	void printTree(PSTNode * root);  //Prints in post order but I don't think it's correct
	void printKids(PSTNode * root);
	void deleteTree(PSTNode * root); //In theory it deletes the tree
	bool isNonTerminal(PSTNode * root);
	void P2AST(PSTNode * root);  //Convert tree to AST
	void P2AST_Convert(PSTNode * root);
	bool isTerminal(PSTNode * root);
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
		tempNode->creationIndex = indexCount;
		indexCount++;
		root->pKids.push_back(tempNode);
		root->kidsCount += 1;
		cout << "Inserting node..." << endl;
		cout << "Node: " << tempNode->m_sym.name << endl;
}

//printTree - Prints the tree in preorder starting with the root node
//I don't know if it's supposed to print every node or just Nonterminal symbols
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

void ParseTree::printKids(PSTNode * root)
{
	for (int i = 0; i < root->pKids.size(); i++)
	{
		cout << root->pKids[i]->m_sym.name << " ";
	}
	cout << ")" << endl;
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

bool ParseTree::isTerminal(PSTNode * root)
{
	if (root->m_sym.name == "id" || root->m_sym.name == "int" || root->m_sym.name == "float" || root->m_sym.name == "string")
		return true;
	else return false;
}