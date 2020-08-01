%option noyywrap
%{
/* including the token storage files  */
#include <stdio.h>
#include"Token.h"
#include"Parser.h"

/* defining variable Test_files to 2 */
#define TESTS 2

/* method to print a message when illegal token is inputted */
void commentHandeler(char*);
void lex_error(char*,int);

/* integer to find each token line  */
int tokenAtLine=-1;

%}
/* defining the base Tokens defintions so that we use them to idinfy ids,numbers...  */
DIGIT		[0-9]
NONZERO		[1-9]
ALPHA		[a-zA-Z]
ALPHANUM	[a-zA-Z0-9]
SMALL 		[a-z]
SIGN		("+"|"-")
/* ids should start with a letter after that int can have one(_) and then a number or a letter  */
ID				{SMALL}("_"?{ALPHANUM})*
NOT_ID				((([A-Z]|{DIGIT}|"_")({ALPHANUM}|"_")*)|(({ALPHANUM})*"_"("_")+("_"|{ALPHANUM})*))
/* integer is a number that is 0 or its a full number (with no .)  */
NUMBER_INT		({NONZERO}{DIGIT}*|0)

/* Real is a number that is 0 or its  not a full number (inculds .)  */
NUMBER_REAL	    (({NONZERO}{DIGIT}*|0)(".")?{DIGIT}*("e")?("+"|"-")?{DIGIT}+)

/* comments start with $$ and ends at the end of line,
 the . repsesnt evrey thing after the /* to the end */
COMMENT			\/\*([^*]|\*[^/])*\*\/
/* end of a line  */
ENDLN			[\n]
/* space between  tkones  */
WHITE			[ \t]

/* in this part we handle all the tokens that we find in the input find  */
%%
"int"	{return  create_and_store_token(TOKEN_KW_INT , yytext , tokenAtLine);} 
"float"	{return  create_and_store_token(TOKEN_KW_FLOAT , yytext , tokenAtLine);} 
"void"	{return  create_and_store_token(TOKEN_KW_VOID , yytext , tokenAtLine);}
"if"	{return  create_and_store_token(TOKEN_KW_IF , yytext , tokenAtLine);}
"return"	{return  create_and_store_token(TOKEN_KW_RETURN , yytext , tokenAtLine);}
";"	 	{return  create_and_store_token(TOKEN_SEP_SEMICOLON , yytext , tokenAtLine);}
","  	{return  create_and_store_token(TOKEN_SEP_COMMA , yytext , tokenAtLine);}
"("	 	{return  create_and_store_token(TOKEN_SEP_L_BRCKT , yytext , tokenAtLine);}
")"	 	{return  create_and_store_token(TOKEN_SEP_R_BRCKT , yytext , tokenAtLine);}
"["	 	{return  create_and_store_token(TOKEN_SEP_L_SQBRCKT , yytext , tokenAtLine);}
"]"		{return  create_and_store_token(TOKEN_SEP_R_SQBRCKT , yytext , tokenAtLine);}
"{"		{return  create_and_store_token(TOKEN_SEP_L_CURBRCKT , yytext , tokenAtLine);}
"}"		{return  create_and_store_token(TOKEN_SEP_R_CURBRCKT , yytext , tokenAtLine);}
":"		{return  create_and_store_token(TOKEN_SEP_COLON , yytext , tokenAtLine);}
"+"		{return  create_and_store_token(TOKEN_OP_ADD , yytext , tokenAtLine);}
"*"		{return  create_and_store_token(TOKEN_OP_MULT , yytext , tokenAtLine);}
"="		{return  create_and_store_token(TOKEN_OP_ASSIGN , yytext , tokenAtLine);}
"<"		{return  create_and_store_token(TOKEN_OP_SMALL , yytext , tokenAtLine);}
"<="	{return  create_and_store_token(TOKEN_OP_SC , yytext , tokenAtLine);}
">"		{return  create_and_store_token(TOKEN_OP_BIG , yytext , tokenAtLine);}
">="	{return  create_and_store_token(TOKEN_OP_BC , yytext , tokenAtLine);}
"=="	{return  create_and_store_token(TOKEN_OP_COM , yytext , tokenAtLine);}
"!="	{return  create_and_store_token(TOKEN_OP_NC , yytext , tokenAtLine);}
	
{ID}	{return  create_and_store_token(TOKEN_ID , yytext , tokenAtLine);}
{NUMBER_INT}	{return  create_and_store_token(TOKEN_INT_NUM , yytext , tokenAtLine);}
{NUMBER_REAL}	{return  create_and_store_token(TOKEN_FLOAT_NUM , yytext , tokenAtLine);}
{NOT_ID}		{lex_error(yytext,tokenAtLine);}

{ENDLN}			{tokenAtLine++;}
{COMMENT}       {commentHandeler(yytext);}
{WHITE}			{}

<<EOF>>			{return create_and_store_token(TOKEN_EOF, yytext , tokenAtLine);}

.       		{lex_error(yytext,tokenAtLine);}

%%

void lex_error(char *lexeme ,int lineNum)
{
	fprintf(yyout,"Character '%s' in line: {%d} does not begin any legal token in the language.\n",lexeme,lineNum);
}
void commentHandeler(char* text){
	int i=0;
	int len = strlen(text);
	if(text[0]!='/' && text[1]!='*')
	{
		return;
	}	
	
	for(i=0;i<len-1;i++){
		if(text[i] == '*' && text[i+1] == '/')
		{
			return;
		}
		if(text[i] == '\n'){
			tokenAtLine++;
		}
	}
}

int main(int argc, char *argv[])
{

	int test,test1;
	int flagin,flagout;

	char filename[100]={0};
	
	eTOKENS kind;


	for(test = 1 ; test <= TESTS ; test++)
	{
		
		flagin = flagout = 0;

		/*generating input file name*/
		sprintf_s(filename,sizeof(filename),"%s%d%s","C:\\temp\\test",test,".txt");
		
		/*open input file*/
		if((fopen_s(&yyin,filename,"r")))
		{ 
			printf("cannot open input file %s; please press any key to continue" ,filename);
			getchar();
			continue;
		}
		else
		{
			flagin = 1;
			tokenAtLine = 1;

			/*generating output file name*/
			sprintf_s(filename,sizeof(filename),"%s%d%s","C:\\temp\\test",test,"_318448255_208941682_lex.txt");

			/*open output file*/
			if((fopen_s(&yyout,filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue" ,filename);
				getchar();
				continue;
			}
			else
			{
				flagout = 1;
			}
		}

		/*both files opened*/
		if(flagin*flagout)
		{
			/*looping yylex till EOF*/
			do{
				kind = yylex();
				Handle_token(kind , yytext , tokenAtLine);
			} while(kind != TOKEN_EOF);
		}

		/*close files*/
		if(flagin)
			fclose(yyin);
		if(flagout)
			fclose(yyout);
	}
	

	
	for(test1 = 1 ; test1 <= TESTS ; test1++)
	{
		
		flagin = flagout = 0;

		/*generating input file name*/
		sprintf_s(filename,sizeof(filename),"%s%d%s","C:\\temp\\test",test1,".txt");
		
		/*open input file*/
		if((fopen_s(&yyin,filename,"r")))
		{ 
			printf("cannot open input file %s; please press any key to continue" ,filename);
			getchar();
			continue;
		}
		else
		{
			flagin = 1;
			tokenAtLine = 1;

			/*generating output file name*/
			sprintf_s(filename,sizeof(filename),"%s%d%s","C:\\temp\\test",test1,"_318448255_208941682__syntactic.txt");

			/*open output file*/
			if((fopen_s(&yyout,filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue" ,filename);
				getchar();
				continue;
			}
			else
			{
				flagout = 1;
			}
		}

		/*both files opened*/
		if(flagin*flagout)
		{
			main_parser();
		}

		/*close files*/
		if(flagin)
			fclose(yyin);
		if(flagout)
			fclose(yyout);
	}
	
}
