Team CCJ
Christopher Leonardi
Justin Shelley

Features:
	Reads in a text code file.
	Goes through the text file and pulls out tokens.
	Tokens are stored in a token struct
	Tokens are then printed out with information about them
Known Bugs:
	Token ID's are not properly recorded for operators and delimiters.
	Ints are recorded as strings	

Usage:
	In the main, drop the name of the text file you wish to run through lexer
	into x.scanFile(FILENAME_HERE).
	
	Run the program. 

Files Included:
	Parser.h
	main.cpp
	EX1.txt
	EX2.txt
	EX3.txt
	README.txt
	Keywords.txt
	FStble.txt

portions primarily authored by Chris:
Token and Keyword Structs
Parser class default consructor
Parser.AddToken()
Parser.PrintToken()
Parser.StringRead()
Parser.KeywordCheck()

Portions primarily authored by Justin:
Parser.ScanFile()
Parser.LoadTable()
Parser.ChangeTableState()
Parser.findColumn()
Parser.isAccepting()

Functions worked on by both:
Parser.next_token()

 