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
	A1_Symbol m_sym;  //RHS rule symbol
	int ruleNumber = -1;
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
	void P2AST(PSTNode * root);  //Convert tree to AST
	void P2AST_Convert(PSTNode * root);
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
		cout << "Node: rule #" << root->ruleNumber << ": " << root->m_sym.name << " = ";
		printKids(root);
		cout << endl;
		for (int i = 0; i < root->kidsCount; i++)
		{
			printTree(root->pKids[i]);
		}
	}
	else
		cout << "Node: " << root->m_sym.name << "   Rule number:" << root->m_sym.ruleID << endl;
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

void ParseTree::P2AST(PSTNode * root)
{
	cout << "Converting node: " << root->m_sym.name << " Position: " << root->position << " Rule number: "
		<< root->ruleNumber << endl;
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
	cout << "Finished converting node " << root->m_sym.name << root->ruleNumber << endl;
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
	cout << "I got here before FUBAR: " << ruleID << endl;
	switch (ruleID)
	{
	case 1:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		$1->pKids.push_back($2);  //Link kwdpg to Slist
		delete $$;
		$$ = $1; //Make kwdprg the root
		break;
	case 2:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = root->pKids[0];
		$2 = root->pKids[1];
		gma->pKids.clear(); gma->pKids.push_back($1); gma->pKids.push_back($2); //Connect kwdprg to Stmt and Slist
		$$->pKids.clear(); delete $$;
		break;
	case 3:
		cout << "I got here before FUBAR: " << ruleID << endl;
		break;
	case 4:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = root->pKids[0];
		$2 = root->pKids[1];
		$3 = root->pKids[2];
		$2->pKids.push_back($1); $2->pKids.push_back($3);
		gma->pKids[$$->position] = $2;
		$$->pKids.clear(); delete $$;
		break;
	case 5:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 6:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 7:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 8:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0]; //kwdprint
		$2 = $$->pKids[2];
		$3 = $$->pKids[4];
		$$->pKids.clear();
		if ($3->ruleNumber == -1)
		{
			$1->pKids.push_back($2);
			gma->pKids[$$->position] = $1;
		}
		else
		{
			$1->pKids.push_back($2);
			$1->pKids.push_back($3);
			gma->pKids[$$->position] = $1;
		}

		$$->pKids.clear();  delete $$;
		break;
	case 9:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		gma->pKids[$$->position] = $1;
		gma->pKids.push_back($2);
		$$->pKids.clear(); delete $$;
		break;
	case 10:
		cout << "I got here before FUBAR: " << ruleID << endl;
		break;
	case 11:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$2 = $$->pKids[1];
		gma->pKids[1] = $2;
		$$->pKids.clear(); delete $$;
		break;
	case 12:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		if ($$->pKids.size() > 1)
		{
			$2 = $$->pKids[1];
			if ($2->ruleNumber == -1)
			{
				gma->pKids[$$->position] = $1;
				
			}
			else
			{
				$2->pKids.insert($2->pKids.begin(), $1);
				gma->pKids[$$->position] = $2;
				

			}
			$$->pKids.clear(); delete $$;
		}
		else
		{
			gma->pKids[$$->position] = $1;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 13:
		cout << "I got here before FUBAR: " << ruleID << endl;
		break;
	case 14:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		$3 = $$->pKids[2];
		if ($3->m_sym.name == "eps")
		{
			$1->pKids.push_back($2);
			gma->pKids[$$->position] = $1;
			$$->pKids.clear(); delete $$;
		}
		else
		{
			gma->pKids[$$->position] = $1;
			$1->pKids.push_back($2); $1->pKids.push_back($3);
			$$->pKids.clear(); delete $$;
		}
		break;
	case 15:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		if ($$->pKids.size() > 1)
		{
			$2 = $$->pKids[1];
			if ($2->ruleNumber == -1)
			{
				gma->pKids[$$->position] = $1;
				$$->pKids.clear(); delete $$;
			}
			else
			{
				$2->pKids.insert($2->pKids.begin(), $1);
				gma->pKids[$$->position] = $2;
				$$->pKids.clear(); delete $$;
			}
		}
		else
		{
			gma->pKids[$$->position] = $1;
			$$->pKids.clear(); delete $$;
		}
		cout << "Did you reach me?" << endl;
		
		break;
	case 16:
		cout << "I got here before FUBAR: " << ruleID << endl;
		break;
	case 17:
		cout << "I got here before FUBAR: " << ruleID << endl;
		$1 = $$->pKids[0];
		$2 = $$->pKids[1];
		if ($$->pKids.size() > 2)
		{
			$3 = $$->pKids[2];
			if ($3->m_sym.name == "eps")
			{
				$1->pKids.push_back($2);
				gma->pKids[$$->position] = $1;
				$$->pKids.clear(); delete $$;
			}

			else
			{
				gma->pKids[$$->position] = $1;
				$1->pKids.push_back($2); $1->pKids.push_back($3);
				$$->pKids.clear(); delete $$;
			}
		}
		else
		{
			$1->pKids.push_back($2);
			gma->pKids[$$->position] = $1;
			$$->pKids.clear(); delete $$;
		}
		break;
	case 18:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		cout << "I am converting teh fatom" << endl;
		break;
	case 19:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[1];
		$$->pKids.clear(); delete $$;
		break;
	case 20:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 21:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 22:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		cout << "I converted teh float" << endl;
		break;
	case 23:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 24:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 25:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 26:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 27:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	case 28:
		cout << "I got here before FUBAR: " << ruleID << endl;
		gma->pKids[$$->position] = $$->pKids[0];
		$$->pKids.clear(); delete $$;
		break;
	default:
		break;
	}
}

/*
void ParseTree::P2A_Helper(PSTNode * root)
{

    So if this node's kid has kids in the second slot?
	we gonna do this thang:

	if (isNonTerminal(root->pKids[somenumber]))
	{
	// $2.Lkid = $1
	//
	root->pKids[1] = root->pKids[0];
	PSTNode * gma = root->pMom->pMom;
	int gmaSlot = root->gmaSlot;

	//$$ = $2
	gma->pKids[gmaSlot] = root->pKids[1]
}
*/