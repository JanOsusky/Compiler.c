/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Zasobnik tabulek symbolu (symtablestack.c)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#include <stdlib.h>
#include <stdio.h>

#include "symtablestack.h"

// funkce pro inicializaci zasobniku
void stack_init(table_stack *stack)
{
    stack->top = NULL;
}

// funkce pro vkladani na zasobnik
bool stack_push(table_stack *stack, bst_node_t *tree)
{
    stack_item *new = (stack_item *)malloc(sizeof(stack_item));
    if (new == NULL) {
        return false;
    }

    new->symtable = tree;
    new->next = stack->top;
    stack->top = new;

    return true;
}

// funkce pro odstraneni prvku z vrcholu zasobniku
bool stack_pop(table_stack *stack)
{
    if (stack->top == NULL) {
        return false;
    }
    else {
        stack_item *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);

        return true;
    }
}

// funkce pro vyprazdneni zasobniku
void free_stack(table_stack *stack)
{
    while (stack_pop(stack));
}

// funkce, ktera vraci rootptr tabulky symbolu
stack_item *stack_top(table_stack *stack)
{
    return stack->top;
}
