/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Tabulka symbolu (symtable.c)
 *
 *   @author Vojtech Adamek (xadame44)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"

void symtable_init(bst_node_t **tree) {
  if(tree != NULL)
    (*tree) = NULL;
}

void symtable_func_init(bst_func_t **tree) {
  if(tree != NULL)
    (*tree) = NULL;
}

void symtable_insert_symbol(bst_node_t **tree, char *key) {
  if(tree != NULL){
    if((*tree) == NULL) {
      bst_node_t *new = (bst_node_t *) malloc(sizeof(bst_node_t));
      if(new == NULL) {
        return;
      }

      // inicializace prvku stromu
      new->key = malloc((strlen(key)+1) * sizeof(char));
      strcpy(new->key, key);
      new->symbol.name = new->key;
      new->symbol.defined = false;
      new->symbol.data_type = type_undefined;
      new->symbol.null_capable = false;
      new->left = NULL;
      new->right = NULL;

      (*tree) = new;
    }

    else {
      if (strcmp(key, (*tree)->key) < 0) // key < (*tree)->key
        symtable_insert_symbol(&(*tree)->left, key);

      else if (strcmp(key, (*tree)->key) > 0)
        symtable_insert_symbol(&(*tree)->right, key);
      
      else if (!strcmp(key, (*tree)->key)) {}
    }
  }
}

void symtable_insert_func(bst_func_t **tree, char *key) {
  if(tree != NULL){
    if((*tree) == NULL) {
      bst_func_t *new = (bst_func_t *) malloc(sizeof(bst_func_t));
      if(new == NULL) {
        return;
      }
      // inicializace prvku stromu
      new->key = malloc((strlen(key)+1) * sizeof(char));
      strcpy(new->key, key);
      new->func.name = new->key;
      new->func.param_count = 0;
      new->func.return_type = type_undefined;
      new->func.defined = false;

      new->func.param_list = (param_list_t *)malloc(sizeof(param_list_t *));
      param_list_init(new->func.param_list);

      new->left = NULL;
      new->right = NULL;

      (*tree) = new;
    }

    else {
      if (strcmp(key, (*tree)->key) < 0) // key < (*tree)->key
        symtable_insert_func(&(*tree)->left, key);

      else if (strcmp(key, (*tree)->key) > 0) // key > (*tree)->key
        symtable_insert_func(&(*tree)->right, key);
      
      else if (!strcmp(key, (*tree)->key)) {/*TODO*/}
    }
  }
}

void param_list_init(param_list_t *param_list) {
  param_list->first = NULL;
}

void add_param(bst_func_t **tree, data_type type) {
  param_list_elem_t * new = (param_list_elem_t *)malloc(sizeof(param_list_elem_t));
  if(new == NULL){
    return;
  }
  new->data_type = type;
  new->next = (*tree)->func.param_list->first;

  if((*tree)->func.param_list->first == NULL) {
    (*tree)->func.param_list->first = new;
  }
  else {
    param_list_elem_t ** tmp = &((*tree)->func.param_list->first);
    while((*tmp) != NULL) {
      tmp = &(*tmp)->next;
    }
    (*tmp) = new;
  }
  
}

void symtable_dispose_symbol(bst_node_t **tree) {
  if(tree != NULL) {
    if ((*tree) != NULL) {
      symtable_dispose_symbol(&(*tree)->right);
      symtable_dispose_symbol(&(*tree)->left);
      free((*tree)->key);
      free(*tree);
      (*tree) = NULL;
    }
  }
}

void symtable_dispose_func(bst_func_t **tree) {
  if(tree != NULL) {
    if ((*tree) != NULL) {
      symtable_dispose_func(&(*tree)->right);
      symtable_dispose_func(&(*tree)->left);
      param_dispose((*tree)->func.param_list);
      free((*tree)->key);
      free(*tree);
      (*tree) = NULL;
    }
  }
}

void param_dispose(param_list_t *param_list) {
    param_list_elem_t *toDelete;

    while (param_list->first != NULL) {
        toDelete = param_list->first;
        param_list->first = param_list->first->next;
        free(toDelete);
    }
}

bst_node_t * symtable_search(bst_node_t *tree, char *key) {
  if (tree != NULL) {
    if (!strcmp(tree->key, key)) {     // nasli jsme hledany uzel
      return tree;
    }
    else if (strcmp(tree->key, key) > 0) {
      return (symtable_search(tree->left, key));
    }
    else 
      return (symtable_search(tree->right, key));
  }
  return NULL;
}

bst_func_t * symtable_search_func(bst_func_t *tree, char *key) {
  if (tree != NULL) {
    if (!strcmp(tree->key, key)) {     // nasli jsme hledany uzel
      return tree;
    }
    else if (strcmp(tree->key, key) > 0) {
      return (symtable_search_func(tree->left, key));
    }
    else 
      return (symtable_search_func(tree->right, key));
  }
  return NULL;
}

void symtable_change_rv(bst_func_t **tree, return_type rt) {
  if ((*tree) != NULL) {
    (*tree)->func.return_type = rt;
  }
}

void symtable_change_pc(bst_func_t **tree, int count) {
  if ((*tree) != NULL) {
    (*tree)->func.param_count = count;
  }
}

void symtable_func_def(bst_func_t **tree) {
  if ((*tree) != NULL) {
    (*tree)->func.defined = true;
  }
}

void symtable_symbol_def(bst_node_t **tree) {
  if ((*tree) != NULL) {
    (*tree)->symbol.defined = true;
  }
}

void symtable_change_dt(bst_node_t **tree, data_type type) {
  if ((*tree) != NULL) {
    (*tree)->symbol.data_type = type;
  }
}

void symtable_nullcapable(bst_node_t **tree) {
  if ((*tree) != NULL) {
    (*tree)->symbol.null_capable = true;
  }
}
