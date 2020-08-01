#include "Token.h"
/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int currentIndex = 0;
Node* currentNode = NULL;

char* eTokensString[] = {
	/* NUMBERS*/
"empty",
"TOKEN_INT_NUM",
"TOKEN_FLOAT_NUM",


/* OPERATIONS AR_OP*/
"TOKEN_OP_ADD",
"TOKEN_OP_MULT",

/*REL_OP*/
"TOKEN_OP_SMALL", // <
"TOKEN_OP_SC", // <=
"TOKEN_OP_COM", // == 
"TOKEN_OP_BIG", // > 
"TOKEN_OP_BC", // >=
"TOKEN_OP_NC", // !=
 /* ASSIGNMENT OPERATION */
" TOKEN_OP_ASSIGN", // = 

 /*TOKEN_ID*/
 "TOKEN_ID",

 /*TOKEN  KEYWORDS*/

 "TOKEN_KW_INT",
 "TOKEN_KW_FLOAT",
 "TOKEN_KW_VOID",
 "TOKEN_KW_IF",
 "TOKEN_KW_RETURN",


 /*SEPRATION SIGNS*/
 "TOKEN_SEP_SEMICOLON", // ;
 "TOKEN_SEP_COMMA", // , 
 "TOKEN_SEP_COLON", // :
 "TOKEN_SEP_L_BRCKT", // (
 "TOKEN_SEP_R_BRCKT", // )
 "TOKEN_SEP_L_SQBRCKT", // [
 "TOKEN_SEP_R_SQBRCKT", // ]
 "TOKEN_SEP_L_CURBRCKT", // {
 "TOKEN_SEP_R_CURBRCKT", // }


 /*EOF TOKEN*/
"TOKEN_EOF"

};



/*
* This function creates a token and stores it in the storage.
*/
eTOKENS create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray =
			(Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray =
				(Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif
	return kind;
}

void Handle_token(eTOKENS kind, char* lexeme, int lineNum)
{

	fprintf(yyout, "Token of Type: %s\t lexeme: %s  \t found at line: %3d.\n ", eTokensString[kind], lexeme, lineNum);

}


Token* back_token()
{
	if (currentNode == NULL)
		return NULL;
	else if (currentIndex == 0)
	{
		if (currentNode->prev != NULL) {
			currentIndex = TOKEN_ARRAY_SIZE - 1;
			currentNode = currentNode->prev;
			return &(currentNode->tokensArray[currentIndex]);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		currentIndex--;
		return &(currentNode->tokensArray[currentIndex]);
	}
}


/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token()
{
	if (currentNode == NULL) {
		yylex();
		return &(currentNode->tokensArray[currentIndex]);
	}
	

	else if (currentIndex == TOKEN_ARRAY_SIZE - 1)
	{
		if (currentNode->next != NULL)
		{
			currentNode = currentNode->next;
			currentIndex = 0;
			return &(currentNode->tokensArray[currentIndex]);
		}
		else {
			yylex();
			return &(currentNode->tokensArray[currentIndex]);
		}
	}
	else
	{
		if (currentNode->tokensArray[currentIndex + 1].kind != 0)
		{
			currentIndex++;
			return &(currentNode->tokensArray[currentIndex]);
		}
		else
		{
			yylex();
			if (currentNode->tokensArray[currentIndex].kind == TOKEN_EOF)
			{
				return &(currentNode->tokensArray[currentIndex].kind);
			}
			return &(currentNode->tokensArray[currentIndex]);
		}

	}

}
