#pragma once
#include"Token.h"
#include<stdio.h>
#include"HASHMAP.h"

void parser_prog();
void parser_global_vars();
void parser_g_tag();
void parser_var_dec();
void parser_c_tag(char* s);
Type parser_type();
void parser_dim_sizes(char* s, int);
void parser_d_tag(char* s, int);
void parser_func_predefs();
void parser_f_tag();
void parser_func_prototype();
void parser_func_full_defs();
void parser_u_tag();
void parser_func_with_body();
Type parser_returned_type();
void parser_params(char*);
void parser_param_list(char*);
void parser_l_tag(char*, int);
void parser_param(char*, int);
void parser_p_tag(char*, int);
void parser_comp_stmt();
void parser_var_dec_list();
void parser_v_tag();
void parser_stmt_list();
void parser_s_tag();
void parser_stmt();
void parser_m_tag(Ht_item*);
void parser_if_stmt();

void parser_args(Ht_item*);
void parser_args_list(Ht_item*);
void parser_a_tag(Ht_item*, int*);
void parser_return_stmt();
Type parser_q_tag();

void parser_i_tag(Ht_item* t);
void parser_expr_list(Ht_item* t);
void parser_x_tag(Ht_item*, int*, int);
void parser_condition();
Type parser_expr();
Type parser_e_tag();
Type parser_term();
Type parser_t_tag();
Type parser_factor();
void parser_r_tag(Ht_item*);
void match(eTOKENS);
void rel_op();



void main_parser();
void parser_d_tag_2(char* s, int index, int i);
void parser_dim_sizes_2(char* s, int index, int i);