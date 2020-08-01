#include"Parser.h"
#include"Token.h"


extern FILE* yyout;
int flag = 1;
FILE* semantic;
Token* current_token;
int fu = 0;
HashTable* current_table;
HashTable* tmp; 
Type funcType;
int errorCount;
int legal_size = 0;
int return_flag;
char funcName[100];


void match(eTOKENS t)
{

	current_token = next_token();
	
	if (!current_token)
		return;

	if (current_token->kind != t)
	{
		current_token = back_token();
		fprintf(yyout, "Expected token: %s at line: %3d \n", eTokensString[t], current_token->lineNumber);
		fprintf(yyout, " Actual Token: %s found at line: %3d. lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		return;
	}
	else {
		fprintf(yyout, "%s at line: %3d \n", eTokensString[t], current_token->lineNumber);
	}

}
void main_parser() {
	errorCount = 0;
	current_table = create_table();
	static int i = 1;
	char filename[100];
	sprintf_s(filename, sizeof(filename), "%s%d%s", "C:\\temp\\test", i, ".txt");
	fopen_s(&semantic, filename, "w");
	i++;
	parser_prog();
	match(TOKEN_EOF);
	errorCount += free_table(current_table,"gloabal_Vars");
	fprintf(semantic, "Total Semantic Erorrs are : %d in this %s File\n",errorCount,filename);
	fclose(semantic);

}
void parser_prog() {
	int flag = 0;
	fprintf(yyout, "RULE{PROG --> GLOBAL_VARS FUNC_PREDEFFS FUNC_FULL_DEFFS} \n");
	while (1) {
		current_token = next_token();
		while (current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_EOF) {
			fprintf(yyout, "Expected token:TOKEN_KW_FLOAT, TOKEN_KW_INT at line: %3d \n", current_token->lineNumber);
			fprintf(yyout, " Actual Token: %s found at line: %3d. lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
			current_token = next_token();
		}
		if (current_token->kind == TOKEN_EOF) {
			current_token = back_token();
			return;
		}
		else {
			current_token = back_token();
			parser_global_vars();
			break;
		}
	}

	while (1) {
		current_token = next_token();
		while (current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_VOID && current_token->kind != TOKEN_EOF) {
			fprintf(yyout, "Expected token:TOKEN_KW_FLOAT, TOKEN_KW_INT at line: %3d \n", current_token->lineNumber);
			fprintf(yyout, " Actual Token: %s found at line: %3d. lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
			current_token = next_token();
		}
		if (current_token->kind == TOKEN_EOF) {
			current_token = back_token();
			return;
		}
		else {
			current_token = back_token();
			parser_func_predefs();
			break;
		}
	}
	current_token = back_token();
	parser_func_full_defs();
	current_token = next_token();

}

/* func_full_Deffs  */
void parser_func_full_defs() {
	flag = 2;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:
		tmp = create_table();
		tmp->Father = current_table;
		current_table = tmp;
		fprintf(yyout, "RULE{FUNC_FULL_DEFS --> FUNC_WITH_BODY U' } \n");
		current_token = back_token();
		parser_func_with_body();
		current_token = next_token();
		while (current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_VOID && current_token->kind != TOKEN_EOF) {
			fprintf(yyout, "Expected token:TOKEN_KW_FLOAT, TOKEN_KW_INT at line: %3d \n", current_token->lineNumber);
			fprintf(yyout, " Actual Token: %s found at line: %3d. lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
			current_token = next_token();
		}
		if (current_token->kind != TOKEN_EOF) {
			current_token = back_token();
			parser_u_tag();
		}
		tmp = current_table;
		current_table = tmp->Father;
		errorCount += free_table(tmp, funcName);
		return;
	default:

		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_VOID at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_VOID && current_token->kind != TOKEN_EOF) {
			current_token = next_token();
		}
		current_token = back_token();
		current_token = back_token();
		return;
	}
}

void parser_u_tag() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{U'--> FUNC_FULL_DEFS} \n");
		current_token = back_token();
		parser_func_full_defs();
	default:
		current_token = back_token();
		return;
	}
}

void parser_func_with_body() {
	return_flag = 0;
	funcType = null;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:
		tmp = create_table();
		tmp->Father = current_table;
		current_table = tmp;
		fprintf(yyout, "RULE{FUNC_WITH_BODY--> FUNC_PROTOTYPE COMP_STMT U' } \n");
		current_token = back_token();
		parser_func_prototype();
		parser_comp_stmt();
		if (return_flag == 0 && funcType != void_func) {
			fprintf(semantic, "%s must return a value of Type : %s.\n", funcName, typesString[funcType]);
			errorCount++;
		}
		tmp = current_table;
		current_table = tmp->Father;
		errorCount += free_table(tmp, funcName);
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_VOID at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_VOID && current_token->kind != TOKEN_EOF) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}
void parser_comp_stmt() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_L_CURBRCKT:
		tmp = create_table();
		tmp->Father = current_table;
		current_table = tmp;
		fprintf(yyout, "RULE{COMP_STMT--> {VAR_DEC_LIST STMT_LIST}} \n");
		fu++;
		parser_var_dec_list();
		parser_stmt_list();
		current_token = back_token();
		match(TOKEN_SEP_R_CURBRCKT);
		fu--;
		tmp = current_table;
		current_table = tmp->Father;
		errorCount += free_table(tmp, funcName);
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_SEP_L_CURBRCKT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_VOID && current_token->kind != TOKEN_EOF) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_var_dec_list() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{VAR_DEC_LIST--> V' } \n");
		current_token = back_token();
		parser_v_tag();
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_ID && current_token->kind != TOKEN_KW_IF && current_token->kind != TOKEN_KW_RETURN && current_token->kind != TOKEN_SEP_L_CURBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_v_tag() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{V'--> VAR_DEC V' } \n");
		current_token = back_token();
		parser_var_dec();
		parser_v_tag();
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_stmt_list() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_KW_IF:
	case TOKEN_KW_RETURN:
	case TOKEN_SEP_L_CURBRCKT:
		fprintf(yyout, "RULE{STMT_LIST--> STMT S' } \n");
		current_token = back_token();
		parser_stmt();
		parser_s_tag();
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_REURN,TOKEN_SEP_L_CURBRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_CURBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_s_tag() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_SEMICOLON:
		fprintf(yyout, "RULE{S'--> STMT S' } \n");
		parser_stmt();
		parser_s_tag();
		return;
	case TOKEN_ID:
	case TOKEN_KW_IF:
	case TOKEN_SEP_L_CURBRCKT:
	case TOKEN_KW_RETURN:
		current_token = back_token();
		parser_stmt_list();
		return;
	case TOKEN_SEP_R_CURBRCKT:
		current_token = next_token();
		if (current_token->kind == TOKEN_ID || current_token->kind == TOKEN_KW_IF || current_token->kind == TOKEN_SEP_L_CURBRCKT || current_token->kind == TOKEN_KW_RETURN && fu > 0)
		{
			current_token = back_token();
			fu--;
			current_token = back_token();
			match(TOKEN_SEP_R_CURBRCKT);
			parser_stmt_list();
			return;
		}
		else {
			current_token = back_token();
			return;
		}
	default:
		current_token = back_token();
		return;
	}
}

void parser_stmt() {
	current_token = next_token();
	Ht_item* t;
	switch (current_token->kind) {
	case TOKEN_ID:
		fprintf(yyout, "RULE{STMT--> ID R' } \n");
		t = find(current_table, current_token->lexeme);
		if (t == NULL) {
			fprintf(semantic ,"Undeclared Variable %s at line : %d \n", current_token->lexeme, current_token->lineNumber);
		}
		else {
			t->used = 1;
		}
		parser_m_tag(t);
		return;
	case TOKEN_KW_IF:
		fprintf(yyout, "RULE{STMT--> IF_STMT' } \n");
		current_token = back_token();
		parser_if_stmt();
		return;
	case TOKEN_KW_RETURN:
		fprintf(yyout, "RULE{STMT--> RETURN_STMT} \n");
		current_token = back_token();
		parser_return_stmt();
		return;
	case TOKEN_SEP_L_CURBRCKT:
		fprintf(yyout, "RULE{STMT--> COMP_STMT} \n");
		current_token = back_token();
		parser_comp_stmt();
		return;
	case TOKEN_SEP_R_CURBRCKT:
		current_token = back_token();
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_ID , TOKEN_KW_IF, TOKEN_KW_RETURN,TOKEN_SEP_L_CURBRCKT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_SEMICOLON) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_m_tag(Ht_item* t) {
	Type l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{M'--> (ARGS)} \n");
		set_used(t);
		parser_args(t);
		match(TOKEN_SEP_R_BRCKT);
		return;
	case TOKEN_SEP_L_SQBRCKT:
		fprintf(yyout, "RULE{M'--> id[EXP_LIST]=EXPR} \n");
		current_token = back_token();
		set_used(t);
		parser_i_tag(t);
		match(TOKEN_OP_ASSIGN);
		l = parser_expr();
		if (l != t->type && t != NULL) {
			if (l == integer && t->type == float_var) {

			}
			else {
				//error
				fprintf(semantic, "DIFFRENT TYPES: trying to assign {%s to %s} at line: %d.\n", typesString[t->type], typesString[l], current_token->lineNumber);
				errorCount++;
			}
		}
		return;
	case TOKEN_OP_ASSIGN:
		fprintf(yyout, "RULE{M'--> id = EXPR} \n");
		set_used(t);
		l = parser_expr();
		if ( t != NULL  && l!= t->type) {
			if (l == integer && t->type == float_var) {

			}
			else {
				//error
				fprintf(semantic, "DIFFRENT TYPES: trying to assign {%s to %s} at line: %d.\n", typesString[t->type], typesString[l], current_token->lineNumber);
				errorCount++;
			}
		}
		return;
	default:
		current_token = back_token();
		return;
	}
}
void parser_i_tag(Ht_item*t) {
	
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_L_SQBRCKT:
		fprintf(yyout, "RULE{I'--> [EXPR_LIST]} \n");
		parser_expr_list(t);
		match(TOKEN_SEP_R_SQBRCKT);
		return;

	default:
		current_token = back_token();
		return;
	}
}

void parser_condition() {
	Type l, u;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{CONDITION--> EXPR rel_op EXPR} \n");
		current_token = back_token();
		l = parser_expr();
		rel_op();
		u = parser_expr();
		if (u != l) {
			//error
			errorCount++;
			fprintf(semantic, "Diffrent TYPES: trying to Compare {%s to %s} different Types at line : %d.\n", typesString[u], typesString[l], current_token->lineNumber);
		}
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens:  TOKEN_ID, TOKEN_FLOAT_NUM,TOKEN_INT_NUM,TOKEN_SEP_L_BRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_BRCKT && current_token->kind != TOKEN_OP_SMALL && current_token->kind != TOKEN_OP_SC && current_token->kind != TOKEN_OP_COM
			&& current_token->kind != TOKEN_OP_BIG && current_token->kind != TOKEN_OP_BC && current_token->kind != TOKEN_OP_NC && current_token->kind != TOKEN_SEP_COMMA) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}
void rel_op() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_OP_SMALL: // <
	case TOKEN_OP_SC: // <=
	case TOKEN_OP_COM:// == 
	case TOKEN_OP_BIG: // > 
	case TOKEN_OP_BC: // >=
	case TOKEN_OP_NC:// !=
		fprintf(yyout, "RULE{rel_op --> == | <= | >= | != } \n");
		return;
	default:
		fprintf(yyout, "Expected tokens:  TOKEN_OP_SMALL, TOKEN_OP_SC,TOKEN_OP_COM,TOKEN_OP_BIG,TOKEN_OP_BC,TOKEN_OP_NC  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return;
	}
}


Type parser_expr() {
	Type u, l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{EXPR--> TERM E' } \n");
		current_token = back_token();
		l = parser_term();
		u = parser_e_tag();

		if ((l == integer && u == integer) || (l == integer && u == null)) {
			return integer;
		}
		else if (l == null) {
			errorCount++;
			fprintf(semantic, "Type is NULL at line: %d.\n", current_token->lineNumber);
			return null;
			

		}
		return float_var;

	case TOKEN_EOF:
		current_token = back_token();
		return null;
	default:
		fprintf(yyout, "Expected tokens:  TOKEN_ID, TOKEN_FLOAT_NUM,TOKEN_INT_NUM,TOKEN_SEP_L_BRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_BRCKT && current_token->kind != TOKEN_OP_SMALL && current_token->kind != TOKEN_OP_SC && current_token->kind != TOKEN_OP_COM
			&& current_token->kind != TOKEN_OP_BIG && current_token->kind != TOKEN_OP_BC && current_token->kind != TOKEN_OP_NC && current_token->kind != TOKEN_SEP_COMMA) {
			current_token = next_token();
		}
		current_token = back_token();
		return null;
	}
}

Type parser_e_tag() {
	Type u, l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_OP_ADD:
		fprintf(yyout, "RULE{E'--> +TERM E' } \n");
		l = parser_term();
		u = parser_e_tag();
		if (l == integer && u == integer || (l == integer && u == null)) {
			return integer;
		}
		else if (l == null) {
			errorCount++;
			fprintf(semantic, "Type is NULL at line: %d.\n", current_token->lineNumber);
			return null;
		
		}
		return float_var;

	default:
		current_token = back_token();
		return null;
	}
}

Type parser_term() {
	current_token = next_token();
	Type l, u;
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{TERM--> FACTOR T' } \n");
		current_token = back_token();
		l = parser_factor();
		u = parser_t_tag();
		if (l == integer && u == integer || (l == integer && u == null)) {
			return integer;
		}
		else if (l == null) {
			errorCount++;
			fprintf(semantic, "Type is NULL at line: %d.\n", current_token->lineNumber);
			return null;
		}
		return float_var;
	case TOKEN_EOF:
		current_token = back_token();
		return null;
	default:
		fprintf(yyout, "Expected tokens:  TOKEN_ID, TOKEN_FLOAT_NUM,TOKEN_INT_NUM,TOKEN_SEP_L_BRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_OP_ADD && current_token->kind!= TOKEN_EOF) {
			current_token = next_token();
		}
		current_token = back_token();
		return null;
	}
}

Type parser_t_tag() {
	Type l, u;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_OP_MULT:
		fprintf(yyout, "RULE{T'--> *FACTOR T' } \n");
		l = parser_factor();
		u = parser_t_tag();
		if (l == integer && u == integer || (l == integer && u == null)) {
			return integer;
		}
		else if (l == null) {
			errorCount++;
			fprintf(semantic, "Type is NULL at line: %d.\n", current_token->lineNumber);
			return null;
		}
		return float_var;
	default:
		current_token = back_token();
		return null;
	}
}

Type parser_factor() {
	Ht_item* t;
	char name[100];
	Type l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
		fprintf(yyout, "RULE{FACTOR--> ID R' } \n");
		strcpy_s(name, 100, current_token->lexeme);
		t = find(current_table, current_token->lexeme);
		if (t == NULL) {
			/*
			error
			*/
			errorCount++;
			fprintf(semantic, "%s is undefined at line : %d\n",name ,current_token->lineNumber);
			return null;
		}
		else {
			set_used(t);
		}
		parser_r_tag(t);
		if (t->type == integer_func || t->type == integer ) {
			return integer;
		}
		else if(t->type == float_func || t->type == float_var){
			return float_var;
		}
		else {
			return t->type;
		}
	case TOKEN_INT_NUM:
		fprintf(yyout, "RULE{FACOTR--> int_num} \n");
		return integer;
	case TOKEN_FLOAT_NUM:
		fprintf(yyout, "RULE{FACOTR--> float_num} \n");
		return float_var;
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{FACTOR--> (EXPR)} \n");
		l = parser_expr();
		match(TOKEN_SEP_R_BRCKT);
		return l;
	default:
		fprintf(yyout, "Expected tokens:  TOKEN_ID, TOKEN_FLOAT_NUM,TOKEN_INT_NUM,TOKEN_SEP_L_BRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return null;
	}
}

void parser_r_tag(Ht_item *t) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_L_SQBRCKT://[
		fprintf(yyout, "RULE{R'--> [EXPR_LIST]} \n");
		parser_expr_list(t);
		match(TOKEN_SEP_R_SQBRCKT);
		return;
	case TOKEN_SEP_L_BRCKT://(
		fprintf(yyout, "RULE{R'--> (ARGS)} \n");
		parser_args(t);
		match(TOKEN_SEP_R_BRCKT);
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_args(Ht_item* t) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{ARGS--> ARGS_LIST} \n");
		current_token = back_token();
		parser_args_list(t);
		return;
	default:
		current_token = back_token();
		return;
	}
}




void parser_expr_list(Ht_item* t) {
	int index = 0;
	Type l;
	int ifarray = 1;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{EXPR_LIST--> EXPR X' } \n");
		current_token = back_token();
		l = parser_expr();
		if (t->list_of_sizes[index] == 0) {
			fprintf(semantic, "Error trying to %s is not an array at line:%d\n", t->name, current_token->lineNumber);
			ifarray = 0;
		}
		if (l != integer) {
			/*
			error
			not integer in array buildings

			*/
			errorCount++;
			fprintf(semantic, "It should be integer at array index at line:%d\n",current_token->lineNumber);
		}

		index++;
		parser_x_tag(t, &index,ifarray);
		if (t->list_of_sizes[index] != 0) {
			/*
			error
			their need to be another fields in the array
			*/
			errorCount++;
			fprintf(semantic, "Error Missing array fields(indexes) in array:%s at line:%d\n", t->name, current_token->lineNumber);
		}
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		if (t->type == int_array || t->type == float_array) {
			/*
			error
			use id as a pointer
			not same levels
			*/
		}
		fprintf(yyout, "Expected tokens:  TOKEN_ID, TOKEN_FLOAT_NUM,TOKEN_INT_NUM,TOKEN_SEP_L_BRCKT  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_SQBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_x_tag(Ht_item* t, int* index, int flag) {
	current_token = next_token();
	Type l;
	switch (current_token->kind) {
	case TOKEN_SEP_COMMA:
		fprintf(yyout, "RULE{X'--> ,EXPR X' } \n");
		l = parser_expr();
		if (flag == 0) {

		}
		else if (t->list_of_sizes[*index] == 0) {
			/*
			error
			not integer in array buildings

			*/
			errorCount++;
			fprintf(semantic, "overrwritting in array in array:%s at line:%d\n", t->name, current_token->lineNumber);
		}
		else if (l != integer) {
			/*
			error
			not integer in array buildings

			*/
			errorCount++;
			fprintf(semantic, "you need to insert integers as index in array:%s at line:%d\n", t->name, current_token->lineNumber);
		}
		(*index) += 1;
		parser_x_tag(t, index,flag);
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_args_list(Ht_item* t) {
	current_token = next_token();
	Type l;
	int index = 0;
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_FLOAT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{ARGS_LIST--> EXPR A' } \n");
		current_token = back_token();
		l = parser_expr();
		if (t->list_of_type[index].t != l) {
			/*
			error

			*/
			errorCount++;
			fprintf(semantic, "Warning not same Types trying to put %s instead of %s at line: %d", typesString[l], typesString[t->list_of_type[index].t], current_token->lineNumber);
		}
		index++;
		parser_a_tag(t, &index);
		if (t->list_of_type[index].t != null) {
			/*
			error

			*/
			errorCount++;
			fprintf(semantic, "Missing more index n array %s  at line: %d", t->list_of_type[index].name, current_token->lineNumber);
		}
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_ID , TOKEN_INT_NUM, TOKEN_FLOAT_NUM,TOKEN_FLOAT_NUM at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return;
	}
}

void parser_a_tag(Ht_item* t, int* index) {
	current_token = next_token();
	Type l;
	switch (current_token->kind) {
	case TOKEN_SEP_COMMA:
		fprintf(yyout, "RULE{A'--> ,EXPR A' } \n");
		l = parser_expr();
		if (t->list_of_type[*index].t != l) {
			/*
			error

			*/
			errorCount++;
			fprintf(semantic, "Missing more index n array %s  at line: %d", t->list_of_type[*index].name, current_token->lineNumber);
		}
		*index += 1;
		parser_a_tag(t, index);
		return;
	default:
		current_token = back_token();
		return;
	}
}
 
void parser_if_stmt() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_IF:
		fprintf(yyout, "RULE{IF_STMT--> if(CONDITION)STMT} \n");
		match(TOKEN_SEP_L_BRCKT);
		parser_condition();
		match(TOKEN_SEP_R_BRCKT);
		parser_stmt();
		return;
	default:

		fprintf(yyout, "Expected tokens: TOKEN_KW_IF at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return;
	}
}
void parser_return_stmt() {
	Type l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_RETURN:
		fprintf(yyout, "RULE{RETURN_STMT--> return Q' } \n");
		l = parser_q_tag();
		if (l == integer) {
			l = integer_func;
		
		}
		else if (l == float_var) {
			l = float_func;
		}
		if (l != funcType || l == null) {
			/*
			error*/
			errorCount++;
			fprintf(semantic, "Error returning value need to  return :%s at line:%d\n", typesString[l], current_token->lineNumber);
		}
		return_flag = 1;
		return;
	default:

		fprintf(yyout, "Expected tokens: TOKEN_KW_RETURN at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return ;

	}
}

Type parser_q_tag() {
	Type l;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_FLOAT_NUM:
	case TOKEN_INT_NUM:
	case TOKEN_SEP_L_BRCKT:
		fprintf(yyout, "RULE{Q'--> EXPR} \n");
		current_token = back_token();
		l = parser_expr();
		return l;
	default:
		current_token = back_token();
		return null;

	}
}
/*func_predefs*/
void parser_func_predefs() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{FUNC_PREDEFFS --> FUNC_PROTOTYPE;F' } \n");
		current_token = back_token();
		parser_func_prototype();
		match(TOKEN_SEP_SEMICOLON);
		parser_f_tag();
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:

		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_VOID at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_VOID) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}
void parser_f_tag() {
	int i = 0;
	int flag = 0;
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:

		/*staert checking for lookahead*/
		while (current_token->kind != TOKEN_SEP_R_BRCKT && current_token->kind != TOKEN_EOF && current_token->kind != TOKEN_SEP_SEMICOLON)
		{
			i++;
			current_token = next_token();
		}

		if (current_token->kind == TOKEN_EOF) {
			flag = 0;
		}
		else {
			current_token = next_token();
			if (current_token->kind == TOKEN_SEP_L_CURBRCKT) {
				flag = 1;
			}
			current_token = back_token();
		}
		while (i > 0)
		{
			i--;
			current_token = back_token();
		}
		if (flag == 1) {
			return;
		}
		/*lookhead checking ends */
		fprintf(yyout, "RULE{F' --> FUNC_PROTOTYPE;F' } \n");
		current_token = back_token();
		parser_func_prototype();
		match(TOKEN_SEP_SEMICOLON);
		parser_f_tag();
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_func_prototype() {
	Ht_item* t;
	current_token = next_token();
	Type l;
	return_flag = 0;
	char s[100];
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_VOID:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{FUNC_PROTOTYPE--> RETURNED_TYPE id (PARAMS)} \n");
		current_token = back_token();
		l = parser_returned_type();
		funcType = l;
		if (l == null) {
			/*
			error
			*/
		}
		match(TOKEN_ID);
		strcpy_s(s, 100, current_token->lexeme);
		strcpy_s(funcName, 100, s);
		t = find(current_table, s);
		if (t == NULL && flag == 2) {
			ht_insert(current_table, s, "FUNCTION_FULL_DEFFS", l,current_token->lineNumber);
			
			set_used(t);	
		}else if(t == NULL || flag < 2){
			ht_insert(current_table, s, "FUNCTION_PREDEFS", l, current_token->lineNumber);
			
			}
		else{
			flag = 3;
			if (l != t->type) {
				/*
				error 
				*/
				fprintf(yyout, "function %s with type %s already exist and in onother type with %s at line: %3d \n",t->name,typesString[t->type], typesString[l],current_token->lineNumber);
				
			}
			else {
				set_used(t);
			}
		}
		match(TOKEN_SEP_L_BRCKT);
		parser_params(s);
		if (flag >= 2 ) {
			insert_parms_func_to_table(current_table, s,current_token->lineNumber);
		}
		match(TOKEN_SEP_R_BRCKT);
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_VOID at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_SEMICOLON && current_token->kind != TOKEN_SEP_L_CURBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}
Type parser_returned_type() {
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
		fprintf(yyout, "RULE{RETRURNED_TYPE --> TYPE | void} \n");
		return float_func;
	case TOKEN_KW_VOID:
		fprintf(yyout, "RULE{RETRURNED_TYPE --> TYPE | void} \n");
		return void_func;
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{RETRURNED_TYPE --> TYPE | void} \n");
		return integer_func;
	case TOKEN_EOF:
		current_token = back_token();
		return null;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT, TOKEN_KW_VOID at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_ID) {
			current_token = next_token();
		}
		current_token = back_token();
		return null;
	}
}

void parser_params(char* s) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{PARAMS--> PARAM_LIST} \n");
		current_token = back_token();
		parser_param_list(s);
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_param_list(char* s) {
	current_token = next_token();
	int index = 0;
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{PARAM_LIST --> PARAM L’} \n");
		current_token = back_token();
		parser_param(s, index);
		parser_l_tag(s, index + 1);
		return;
	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return;
	}
}

void parser_l_tag(char* s, int index) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_COMMA:
		fprintf(yyout, "RULE{ L’ --> ,PARAM L’} \n");
		parser_param(s, index);
		parser_l_tag(s, index + 1);
		return;
	default:
		current_token = back_token();
		return;
	}
}

void parser_param(char* s, int index) {
	Ht_item* t;
	t = find(current_table, s);
	current_token = next_token();
	char d[100];
	Type l;
	switch (current_token->kind) {
	case TOKEN_KW_FLOAT:
	case TOKEN_KW_INT:
		current_token = back_token();
		l = parser_type();
		fprintf(yyout, "RULE{ PARAM --> TYPE id P' } \n");
		match(TOKEN_ID);
		strcpy_s(d, 100, current_token->lexeme);

		if (t->list_of_type[index].t != l && t->list_of_type[index].t != zero && t != NULL && flag == 3	){
			fprintf(semantic,"%s is Not same Types in the declration function at line : %d\n", d, current_token->lineNumber);
		}
		insert_names_into_varaibles_func_predefs(current_table, s, d, index);
		insert_types_into_varaibles_func_predefs(current_table, s, l, index);
		parser_p_tag(s, index);
		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_COMMA) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_p_tag(char* s, int index) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_L_SQBRCKT:
		fprintf(yyout, "RULE{ P' --> [DIM_SIZES]} \n");
		parser_dim_sizes_2(s, index, 0);
		match(TOKEN_SEP_R_SQBRCKT);
		return;
	default:
		current_token = back_token();
		return;
	}
}
void parser_dim_sizes_2(char* s, int index, int i) {
	current_token = next_token();
	int number;
	switch (current_token->kind) {
	case TOKEN_INT_NUM:
		fprintf(yyout, "RULE{ DIM_SIZES --> int_num D' } \n");
		number = atoi(current_token->lexeme);
		if (flag == 2) {
			int answer = check_if_param_is_array_and_same_size_func_full_deffs(current_table, s, number, index, i);
			if (answer != 0) {
				/*
				error
				*/
			}
		}
		else {
			insert_numbers_into_varaibles_func_predefs(current_table, s, number, index, i);
		}
		parser_d_tag_2(s, index, i + 1);
		current_token = next_token();
		if (current_token->kind == TOKEN_SEP_COMMA) {
			current_token = back_token();
			parser_d_tag_2(s, index, i + 1);
		}
		else {
			current_token = back_token();
		}
		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_INT_NUM  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_CURBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}


void parser_d_tag_2(char* s, int index, int i) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_COMMA:
		fprintf(yyout, "RULE{ D' --> ,DIM_SIZES} \n");
		parser_dim_sizes_2(s, index, i);
		return;
	default:
		current_token = back_token();
		return;
	}
}
/*  END OF FUNC_PREDEFS SECSSION*/

/*Gloabal vars secssion*/


void parser_global_vars() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_INT:
	case TOKEN_KW_FLOAT:
		fprintf(yyout, "RULE{ GLOBAL_VARS--> VAR_DEC G'\n");
		current_token = back_token();
		parser_var_dec();
		current_token = next_token();
		if (current_token->kind == TOKEN_KW_FLOAT || current_token->kind == TOKEN_KW_INT) {
			current_token = back_token();
			parser_g_tag();
		}
		else {
			current_token = back_token();
			return;
		}

		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;

	default:

		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT && current_token->kind != TOKEN_KW_VOID) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}
void parser_g_tag() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_INT:
	case TOKEN_KW_FLOAT:
		current_token = next_token();
		current_token = next_token();
		if (current_token->kind == TOKEN_SEP_L_BRCKT) {
			current_token = back_token();
			current_token = back_token();
			current_token = back_token();
			return;
		}
		fprintf(yyout, "RULE{G'--> VAR_DEC G' } \n");
		current_token = back_token();
		current_token = back_token();
		current_token = back_token();
		parser_var_dec();
		parser_g_tag();
		return;
	default:
		current_token = back_token();
		return;
	}
}
void parser_var_dec() {
	Type t;
	char s[100];
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_INT:
	case TOKEN_KW_FLOAT:
		fprintf(yyout, "RULE{VAR_DEC--> TYPR id C'\n");
		current_token = back_token();
		t = parser_type();
		match(TOKEN_ID);
		strcpy_s(s, 100, current_token->lexeme);
		if (t == null) {
			/*
			error
			*/
		}
		else {
			ht_insert(current_table, current_token->lexeme, "Variable", t, current_token->lineNumber);
		}

		parser_c_tag(s);
		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_KW_FLOAT && current_token->kind != TOKEN_KW_INT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_c_tag(char* s) {
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_SEP_SEMICOLON:
		fprintf(yyout, "RULE{ C' --> ;} \n");
		return;
	case TOKEN_SEP_L_SQBRCKT:
		fprintf(yyout, "RULE{ C' --> [DIM_SIZES];} \n");
		parser_dim_sizes(s, 0);

		match(TOKEN_SEP_R_SQBRCKT);
		match(TOKEN_SEP_SEMICOLON);
		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_SEP_SEMICOLON , TOKEN_SEP_L_SQBRCKT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		current_token = back_token();
		return;
	}
}

Type parser_type() {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_KW_INT:
		fprintf(yyout, "RULE{ TYPE --> int} \n");
		return integer;
	case TOKEN_KW_FLOAT:
		fprintf(yyout, "RULE{ TYPE --> float} \n");
		return float_var;

	case TOKEN_EOF:
		current_token = back_token();
		return null;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_KW_INT , TOKEN_KW_FLOAT at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_ID) {
			current_token = next_token();
		}
		current_token = back_token();
		return null;
	}
}

void parser_dim_sizes(char* s, int index) {
	current_token = next_token();
	int number;
	switch (current_token->kind) {
	case TOKEN_INT_NUM:
		fprintf(yyout, "RULE{ DIM_SIZES --> int_num D' } \n");
		number = atoi(current_token->lexeme);

		insert_numbers_to_array(current_table, s, number, index);
		parser_d_tag(s, index + 1);
		current_token = next_token();
		if (current_token->kind == TOKEN_SEP_COMMA) {
			current_token = back_token();
			parser_d_tag(s, index + 1);
		}
		else {
			current_token = back_token();
		}
		return;

	case TOKEN_EOF:
		current_token = back_token();
		return;
	default:
		fprintf(yyout, "Expected tokens: TOKEN_INT_NUM  at line: %3d \n", current_token->lineNumber);
		fprintf(yyout, "Actual Token: %s  found at line: %3d.  lexeme: %s\n", eTokensString[current_token->kind], current_token->lineNumber, current_token->lexeme);
		while (current_token->kind != TOKEN_SEP_R_CURBRCKT) {
			current_token = next_token();
		}
		current_token = back_token();
		return;
	}
}

void parser_d_tag(char* s, int index) {
	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_SEP_COMMA:
		fprintf(yyout, "RULE{ D' --> ,DIM_SIZES} \n");
		parser_dim_sizes(s, index);
		return;
	default:
		current_token = back_token();
		return;
	}
}
/* END OF gloabal Vars secsssion*/