/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Implementace parseru (parser.h)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Jan Osusky (xosusk00)
 *   @author David Prochazka (xproch0q)
 */

#ifndef _PARSER_H
#define _PARSER_H

#include "scanner.h"
#include "symtable.h"
#include "symtablestack.h"

#define GENERATE_CODE(_callback, ...) \
    if (!_callback(__VA_ARGS__))      \
    return ERROR_INTERNAL

typedef struct
{
    bst_func_t *global_table; // tabulka funkci
    bst_node_t *local_table;  // tabulka promennych hlavniho tela programu

    table_stack symstack; // zasobnik tabulek symbolu
    data_type type;

    lexToken token;
} ParserData;

int prog(ParserData *parserData);
int st_list(ParserData *parserData);
int stat(ParserData *parserData);
int init_rhs(ParserData *parserData);
int rhs(ParserData *parserData, bst_node_t *tempNode);
int in_param_init(ParserData *parserData);
int in_param_list(ParserData *parserData);
int in_param(ParserData *parserData);
int expr(ParserData *parserData);
int type(ParserData *parserData, bst_func_t *tempFunc);
int out_param_init(ParserData *parserData);
int out_param_list(ParserData *parserData);
void init_var(ParserData *parserData);
void insert_buildin(bst_func_t **func_table);
int parser();

#endif
