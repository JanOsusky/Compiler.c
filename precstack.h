/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Zasobnik pro zpracovani vyrazu (precstack.h)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#ifndef _PRECSTACK_H
#define _PRECSTACK_H

#include <stdbool.h>

#include "symtable.h"

typedef struct {
    int type;
    bool terminal;
    data_type data_type;
    char name[1000];
} prec_elem_t;

typedef struct prec_item {
    struct prec_item *next;
    prec_elem_t element;
} prec_item_t;

typedef struct {
    prec_item_t *top;
} prec_stack_t;

int prec_stack_init(prec_stack_t* stack);
int prec_stack_push(prec_stack_t *stack, int type, bool terminal, data_type data_type, char* name);
void prec_stack_pop(prec_stack_t* stack);
void prec_free_stack(prec_stack_t* stack);

#endif
