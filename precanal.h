/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Precedencni analyza (precanal.h)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#ifndef _PRECANAL_H
#define _PRECANAL_H

#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "precstack.h"

#define RT_TOKEN_PLUS 0
#define RT_TOKEN_MINUS 1
#define RT_TOKEN_ASTERISK 2
#define RT_TOKEN_SLASH 3
#define RT_TOKEN_LESS_THAN 4
#define RT_TOKEN_LESS_THAN_OR_EQUAL 5
#define RT_TOKEN_MORE_THAN 6
#define RT_TOKEN_MORE_THAN_OR_EQUAL 7
#define RT_TOKEN_TYPE_EQUAL 8
#define RT_TOKEN_TYPE_NOT_EQUAL 9
#define RT_TOKEN_ROUND_BRACKET_LEFT 10
#define RT_TOKEN_ROUND_BRACKET_RIGHT 11
#define RT_TOKEN_IDENTIFIER 12
#define RT_TOKEN_INT 13
#define RT_TOKEN_FLOAT 14
#define RT_TOKEN_STRING 15
#define RT_TOKEN_DOLLAR_SIGN 16
#define RT_EXPR 17

#define RULE_INT      20  // E -> int
#define RULE_FLOAT    21  // E -> float
#define RULE_ID       22  // E -> id
#define RULE_STR      23  // E -> str
#define RULE_PAR      24  // E -> (E)
#define RULE_ADD      25  // E -> E+E
#define RULE_SUB      26  // E -> E-E
#define RULE_MUL      27  // E -> E*E
#define RULE_DIV      28  // E -> E/E
#define RULE_LESS     29  // E -> E<E
#define RULE_GREAT    30  // E -> E>E
#define RULE_LEQ      31  // E -> E>=E
#define RULE_GEQ      32  // E -> E<=E
#define RULE_EQ       33  // E -> E===E
#define RULE_NEQ      34  // E -> E!==E
#define TYPE_BOOL     35  // bool type

int convert_token(int token_number);
int check_format(prec_stack_t *stack, int operator_type);
int check_semantic(prec_elem_t *op1_left, int operator, prec_elem_t *op2_right);
int shift(prec_stack_t *stack, ParserData *data);
prec_item_t *get_first_nonterminal(prec_stack_t *stack);
int reduce(prec_stack_t *stack, ParserData *data);
void reduce_brackets(prec_stack_t *stack);
int analyze(ParserData *data);
void pop3(prec_stack_t* stack);

#endif
