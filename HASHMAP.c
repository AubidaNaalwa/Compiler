#include"HASHMAP.h"
extern FILE* semantic;

int hash_function(char* str) {
    int i = 0;
    for (int j = 0; str[j]; j++) {
        i += str[j];
    }
    return (i % SIZE);
}

Ht_item* create_item(char* key, char* role, Type type) {
    // Creates a pointer to a new hash table item
    int i, j;

    Ht_item* item;

    (item) = (Ht_item*)malloc(sizeof(Ht_item));

    (item)->name = (char*)malloc(strlen(key) + 1);
    (item)->role = (char*)malloc(strlen(role) + 1);

    strcpy_s((item)->name, strlen(key) + 1, key);
    strcpy_s((item)->role, strlen(role) + 1, role);
    for (i = 0; i < 100; i++) {
        (item)->list_of_type[i].t = null;
        (item)->list_of_type[i].name = NULL;
        item->type = null;
        item->list_of_sizes[i] = 0;
        for (j = 0; j < 100; j++) {
            (item)->list_of_type[i].size[j] = 0;
        }
    }

    (item)->type = type;
    (item)->used = 0;
    return item;
}
LinkedList* allocate_list() {
    // Allocates memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    return list;
}

LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    // Inserts the item onto the Linked List
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }

    else if (list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }

    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

LinkedList** create_overflow_buckets(HashTable* table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList** buckets = (LinkedList**)calloc(SIZE, sizeof(LinkedList*));
    for (int i = 0; i < SIZE; i++) {
        buckets[i] = NULL;
    }
    return buckets;
}

void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->name);
        free(temp->item->role);
        free(temp->item);
        free(temp);
    }
}

void free_overflow_buckets(HashTable* table) {
    // Free all the overflow bucket lists
    LinkedList** buckets = table->overflow_buckets;
    for (int i = 0; i < SIZE; i++) {
        free_linkedlist(buckets[i]);
    }
    free(buckets);
}
void free_item(Ht_item* item) {
    // Frees an item
    free(item->name);
    free(item->role);
    free(item);
}

HashTable* create_table() {
    // Creates a new HashTable
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->count = 0;
    table->items = (Ht_item**)malloc(SIZE * sizeof(Ht_item*));
    for (int i = 0; i < SIZE; i++) {
        table->items[i] = NULL;
    }
    table->overflow_buckets = create_overflow_buckets(table);
    table->Father = NULL;
    return table;
}

int free_table(HashTable* table, char* function) {
    // Free the table
    int j = 0;
    if (!table) {
        return 0;
    }
    for (int i = 0; i < SIZE; i++) {
        Ht_item* item = table->items[i];

        if (item != NULL)
        {
            if (item->used == 0 && item->type != zero && item->name != NULL) {
                j++;
                fprintf(semantic, "%s Has not been used at BLOCK: %s. \n", item->name, function);
            }
            free_item(item);
        }
    }

    // Free the overflow bucket linked linkedlist and it's items
    free_overflow_buckets(table);
    free(table->items);
    free(table);
    return j;
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {

    if (!table) {
        return;
    }
    LinkedList* head = table->overflow_buckets[index % SIZE];
    if (head == NULL) {
        // We need to create the list
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index % SIZE] = head;
        return;
    }
    else {
        // Insert to the list
        table->overflow_buckets[index % SIZE] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable* table, char* key, char* role, Type type) {
    // Create the item
    Ht_item* item = create_item(key, role, type);
    if (!table) {
        return;
    }
    // Compute the index
    unsigned long index = hash_function(key);

    Ht_item* current_item = table->items[index % SIZE];

    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == SIZE) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }

        // Insert directly
        table->items[index % SIZE] = item;
        table->count++;
    }

    else {
        // Scenario 1: We only need to update value
        if (strcmp(current_item->name, key) == 0) {
            fprintf(semantic, "%s is already exist in the same BLOCK\n", current_item->name);
            return;
        }

        else {
            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}


Ht_item* ht_search(HashTable* g, char* key) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    HashTable* table = g;
    if (!table) {
        return NULL;
    }
    int index = hash_function(key);
    Ht_item* item = table->items[index % SIZE];
    LinkedList* head = table->overflow_buckets[index % SIZE];

    // Ensure that we move to items which are not NULL
    while (item != NULL) {
        if (strcmp(item->name, key) == 0)
            return item;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void insert_numbers_to_array(HashTable* table, char* key, int number, int index) {
    Ht_item* tmp = ht_search(table, key);
    if (tmp == NULL)
    {
        return;
    }
    tmp->list_of_sizes[index] = number;
}

void insert_numbers_into_varaibles_func_predefs(HashTable* table, char* key, int number, int index, int i) {
    Ht_item* tmp = ht_search(table, key);
    if (index == 100 || tmp == NULL) {
        return;
    }
    if (tmp->list_of_type[index].t == integer)
        tmp->list_of_type[index].t = int_array;
    else if (tmp->list_of_type[index].t == float_var)
        tmp->list_of_type[index].t = float_array;

    tmp->list_of_type[index].size[i] = number;
}

void insert_types_into_varaibles_func_predefs(HashTable* table, char* key, Type type, int index) {
    Ht_item* tmp = ht_search(table, key);
    if (index == 100 || tmp == NULL) {
        return;
    }
    tmp->list_of_type[index].t = type;

}
void insert_names_into_varaibles_func_predefs(HashTable* table, char* key, char* name, int index) {
    Ht_item* tmp = ht_search(table, key);
    if (index >= 100 || tmp == NULL) {
        return;
    }
    for (int i = 0; i < index; i++) {
        if (strcmp(name, tmp->list_of_type[i].name) == 0) {
            /*
            error
            */
            return;
        }
    }
    tmp->list_of_type[index].name = (char*)malloc(sizeof(name) + 1);
    strcpy_s(tmp->list_of_type[index].name, sizeof(name) + 1, name);
}

int check_type_func_full_deffs(HashTable* table, char* name, Type t, int index) {
    Ht_item* l = find(table, name);
    if (l == NULL || index >= 100) {
        return -1;
    }
    if (l->list_of_type[index].t != t) {
        return 1;
    }
    return 0;
}

int check_if_param_is_array_and_same_size_func_full_deffs(HashTable* table, char* name, int number, int index, int i) {
    Ht_item* l = find(table, name);
    if (l == NULL || index >= 100) {
        return -1;
    }
    if (l->list_of_type[index].size[i] != number) {
        return 1;
    }
    return 0;
}


void update_type(HashTable* table, char* key, Type t) {
    Ht_item* tmp = ht_search(table, key);
    if (tmp == NULL) {
        return;
    }
    tmp->type = t;
}

int check_full_func_parameters(HashTable* table, char* key, HashTable* g) {
    Ht_item* tmp = ht_search(table, key);
    Ht_item* tmp2 = ht_search(g, key);
    int i = 0;
    int j;
    if (tmp == NULL || tmp2 == NULL) {
        /*
        error handling
        */
        return 0;
    }
    for (i = 0; i < 100; i++) {
        if (tmp->list_of_type[i].t == null || tmp2->list_of_type[i].t == null) {
            break;
        }
        if (tmp->list_of_type[i].t != tmp2->list_of_type[i].t) {
            /*
            error handling
            */
            return 0;
        }
        for (j = 0; j < 100; j++) {
            if (tmp->list_of_type[i].t == 0 || tmp2->list_of_type[i].t == 0) {
                break;
            }
            if (tmp->list_of_type[i].size[j] != tmp2->list_of_type[i].size[j]) {
                /*
                error handling
                */
                break;
            }
        }
        if (tmp->list_of_type[i].size[j] != tmp2->list_of_type[i].size[j] && j < 100) {
            /*
            errorHANDLING
            */
        }
    }
    if (tmp->list_of_type[i].t != tmp2->list_of_type[i].t && i < 100) {

        /*
        errorHANDLING
        */
        return 0;
    }
    return 1;
}

void set_usesd_attribute(HashTable* ht, char* name) {
    Ht_item* tmp = ht_search(ht, name);
    if (tmp == NULL)
    {
        return;
    }
    tmp->used = 1;
}

void insert_parms_func_to_table(HashTable* table, char* key) {
    Ht_item* tmp = find(table, key);
    if (tmp == NULL) {
        return;
    }
    Ht_item* item;
    int i, j;
    for (i = 0; i < 100; i++) {
        if (tmp->list_of_type[i].name == NULL) {
            break;
        }
        ht_insert(table, tmp->list_of_type[i].name, "VARIABLE", tmp->list_of_type[i].t);
        item = ht_search(table, tmp->list_of_type[i].name);
        for (j = 0; j < 100; j++) {
            item->list_of_sizes[j] = tmp->list_of_type[i].size[j];
        }

    }
}

int check_args(HashTable* t, char* name, int index, Type le) {

    Ht_item* tmp = ht_search(t, name);
    if (tmp == NULL) {
        return 1;
    }
    if (tmp->list_of_type[index].t == le) {
        return 1;
    }
    return 0;
}

Ht_item* find(HashTable* table, char* name) {
    HashTable* tmp = table;
    Ht_item* n;

    while (tmp != NULL) {
        n = ht_search(tmp, name);
        if (n != NULL) {
            return n;
        }
        tmp = tmp->Father;
    }
    return NULL;
}

void set_used(Ht_item* t) {
    if (t != NULL) {
        t->used = 1;
    }
}


char* typesString[] = {
    "null",
    "integer",
    "float_var",
    "int_array",
    "void_func",
    "float_func",
    "integer_func",
    "float_array",
    "zero"
};
