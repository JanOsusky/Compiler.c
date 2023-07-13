/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Zasobnik pro zpracovani vyrazu (precstack.c)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#include <stdlib.h>

#include "precstack.h"
#include "error.h"
#include "precanal.h"

// funkce pro inicializaci zasobniku
int prec_stack_init(prec_stack_t* stack) {
    prec_item_t *new = malloc(sizeof(prec_item_t));
    if (new == NULL) return ERROR_INTERNAL;

    new->element.type = RT_TOKEN_DOLLAR_SIGN;
    new->element.terminal = true;
    new->element.data_type = type_undefined;

    new->next = NULL;
    stack->top = new;

    return ERROR_NO_ERROR;
}

// funkce pro vkladani na zasobnik
int prec_stack_push(prec_stack_t *stack, int type, bool terminal, data_type data_type, char* name) {
    prec_item_t *new = malloc(sizeof(prec_item_t));
    if (new == NULL) return ERROR_INTERNAL;

    new->element.type = type;
    new->element.terminal = terminal;
    new->element.data_type = data_type;
    strcpy(new->element.name, name);

    new->next = stack->top;
    stack->top = new;

    return ERROR_NO_ERROR;
}

// funkce pro odstraneni prvku z vrcholu zasobniku
void prec_stack_pop(prec_stack_t* stack) {
    if (stack->top != NULL) {
        prec_item_t *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

// funkce pro vyprazdneni zasobniku
void prec_free_stack(prec_stack_t* stack) {
    while (stack->top != NULL)
        prec_stack_pop(stack);
}
