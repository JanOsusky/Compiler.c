/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Tabulka symbolu (symtable.h)
 *
 *   @author Vojtech Adamek (xadame44)
 */

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdbool.h>

// vycet pro ulozeni datoveho typu promenne
typedef enum {
  type_int,
  type_float,
  type_string,
  type_null,
  type_undefined
} data_type;

// vycet pro specifikaci navratoveho typu
typedef enum {
  return_int,
  return_float,
  return_string,
  return_void,
  return_undefined
} return_type;

typedef struct { 
  char *name;
  data_type data_type;
  bool null_capable;
  bool defined;
} symbol_t;

typedef struct bst_node { 
  char *key;
  symbol_t symbol;
  struct bst_node *left;
  struct bst_node *right;
} bst_node_t;

typedef struct param_list_elem {
  data_type data_type;
  struct param_list_elem *next;
} param_list_elem_t;

typedef struct param_list {
  param_list_elem_t *first;
} param_list_t;

typedef struct {
  char *name;
  int param_count;
  param_list_t *param_list;
  return_type return_type;
  bool null_capable;
  bool defined;
} func_t;

typedef struct bst_func {
  char *key;
  func_t func;
  struct bst_func *left;
  struct bst_func *right;
} bst_func_t;


// FUNCTION DECLARATIONS

/* VAR SYMTABLE */
void symtable_init(bst_node_t **tree);

void symtable_insert_symbol(bst_node_t **tree, char *key);

void symtable_insert_func(bst_func_t **tree, char *key);

bst_node_t *symtable_search(bst_node_t *tree, char *key);

void symtable_dispose_symbol(bst_node_t **tree);

// sets the value "defined" of struct symbol_t to true
void symtable_symbol_def(bst_node_t **tree);

// sets the value "data_type" of struct symbol_t to "type"
void symtable_change_dt(bst_node_t **tree, data_type type);

// sets the value "null_capable" of struct symbol_t to true
void symtable_nullcapable(bst_node_t **tree);

/* FUNC SYMTABLE */
void symtable_func_init(bst_func_t **tree);

void symtable_insert_func(bst_func_t **tree, char *key);

void symtable_dispose_func(bst_func_t **tree);

void param_list_init(param_list_t *param_list);

void add_param(bst_func_t **tree, data_type type);

void param_dispose(param_list_t *param_list);

// sets the value "defined" of struct func_t to true
void symtable_func_def(bst_func_t **tree); 

// sets the value "param_count" of struct func_t to "count"
void symtable_change_pc(bst_func_t **tree, int count);

// sets the value "return_type" of struct func_t to "rt"
void symtable_change_rv(bst_func_t **tree, return_type rt);

bst_func_t *symtable_search_func(bst_func_t *tree, char *key);

#endif
