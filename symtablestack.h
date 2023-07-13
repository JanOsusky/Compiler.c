/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Zasobnik tabulek symbolu (symtablestack.h)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#ifndef _SYMTABLESTACK_H
#define _SYMTABLESTACK_H

#include <stdbool.h>
#include <string.h>

#include "symtable.h"

typedef struct stack_item
{
    struct stack_item *next; // pointer to next item
    bst_node_t *symtable;
    bool in_func;
} stack_item;

typedef struct
{
    stack_item *top;
} table_stack;

void stack_init(table_stack *stack);
bool stack_push(table_stack *stack, bst_node_t *symtable);
bool stack_pop(table_stack *stack);
void free_stack(table_stack *stack);
stack_item *stack_top(table_stack *stack);

#endif
