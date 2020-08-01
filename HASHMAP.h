#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10000

typedef enum type {
	null,
	integer,
	float_var,
	int_array,
	void_func,
	float_func,
	integer_func,
	float_array,
	zero

}Type;
typedef struct func_att {
	Type t;
	int size[100];
	char* name;
}ATTR;
typedef struct ht_item {
	char* name;
	char* role;
	Type type;
	int list_of_sizes[100];
	ATTR list_of_type[100];
	int used;
	int int_val;
}Ht_item;

char* typesString[];

typedef struct list {
	Ht_item* item;
	struct list* next;
}LinkedList;
typedef struct hashtable {
	Ht_item** items;
	LinkedList** overflow_buckets;
	struct hashtable* Father;
	int count;
}HashTable;

int hash_function(char* str);
Ht_item* create_item(char*, char*, Type);
LinkedList* allocate_list();

LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item);

LinkedList** create_overflow_buckets(HashTable* table);

void free_linkedlist(LinkedList* list);

void free_overflow_buckets(HashTable* table);
void free_item(Ht_item* item);
HashTable* create_table();
int free_table(HashTable* table, char*);

void handle_collision(HashTable* table, unsigned long index, Ht_item* item);

void ht_insert(HashTable* table, char* key, char* role, Type type);


Ht_item* ht_search(HashTable* table, char* key);

void insert_numbers_to_array(HashTable* table, char* key, int number, int index);

void insert_numbers_into_varaibles_func_predefs(HashTable* table, char* key, int number, int index, int i);
void insert_types_into_varaibles_func_predefs(HashTable* table, char* key, Type type, int index);
void update_type(HashTable* table, char* key, Type t);
int check_full_func_parameters(HashTable* table, char* key, HashTable* g);
void set_usesd_attribute(HashTable* ht, char* name);
void insert_names_into_varaibles_func_predefs(HashTable* table, char* key, char* name, int index);
void insert_parms_func_to_table(HashTable* table, char* key);
int check_args(HashTable* t, char* name, int index, Type le);

Ht_item* find(HashTable* table, char* name);
int check_type_func_full_deffs(HashTable* table, char* name, Type t, int index);


int check_if_param_is_array_and_same_size_func_full_deffs(HashTable* table, char* name, int number, int index, int i);

void set_used(Ht_item* t);