#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin, * yyout;
char* eTokensString[];


#define TOKEN_ARRAY_SIZE 1000
typedef enum eTOKENS
{
	/* NUMBERS*/
	TOKEN_MPTY,
	TOKEN_INT_NUM,
	TOKEN_FLOAT_NUM,


	/* OPERATIONS AR_OP*/
	TOKEN_OP_ADD,
	TOKEN_OP_MULT,

	/*REL_OP*/
	TOKEN_OP_SMALL, // <
	TOKEN_OP_SC, // <=
	TOKEN_OP_COM, // == 
	TOKEN_OP_BIG, // > 
	TOKEN_OP_BC, // >=
	TOKEN_OP_NC, // !=
	 /* ASSIGNMENT OPERATION */
	 TOKEN_OP_ASSIGN, // = 

	 /*TOKEN_ID*/
	 TOKEN_ID,

	 /*TOKEN  KEYWORDS*/

	 TOKEN_KW_INT,
	 TOKEN_KW_FLOAT,
	 TOKEN_KW_VOID,
	 TOKEN_KW_IF,
	 TOKEN_KW_RETURN,


	 /*SEPRATION SIGNS*/
	 TOKEN_SEP_SEMICOLON, // ;
	 TOKEN_SEP_COMMA, // , 
	 TOKEN_SEP_COLON, // :
	 TOKEN_SEP_L_BRCKT, // (
	 TOKEN_SEP_R_BRCKT, // )
	 TOKEN_SEP_L_SQBRCKT, // [
	 TOKEN_SEP_R_SQBRCKT, // ]
	 TOKEN_SEP_L_CURBRCKT, // {
	 TOKEN_SEP_R_CURBRCKT, // }


	 /*EOF TOKEN*/
	 TOKEN_EOF,

}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char* lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token* tokensArray;
	struct Node* prev;
	struct Node* next;
} Node;
void Handle_token(eTOKENS kind, char* lexeme, int lineNum);
eTOKENS create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token* next_token();
Token* back_token();


#endif