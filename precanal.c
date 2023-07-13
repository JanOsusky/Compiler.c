/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Precedencni analyza (precanal.c)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Tomas Najman (xnajma03)
 */

#include <stdlib.h>

#include "precanal.h"
#include "error.h"

int open_brackets = 0;
int err;

int table [17][17] = {
/*           ,+,  ,-,  ,*,  ,/,  ,<,  ,<=, ,>,  ,>=, ,===, ,!==, ,(,  ,),   ID  INT FLOAT STR   $  */
/* + */    { '>', '>', '<', '<', '>', '>', '>', '>', '>',  '>',  '<', '>', '<', '<', '<', '<', '>' },
/* - */    { '>', '>', '<', '<', '>', '>', '>', '>', '>',  '>',  '<', '>', '<', '<', '<', ' ', '>' },
/* * */    { '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  '<', '>', '<', '<', '<', ' ', '>' },
/* / */    { '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  '<', '>', '<', '<', '<', ' ', '>' },
/* < */    { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* <= */   { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* > */    { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* >= */   { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* === */  { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* !== */  { '<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ',  ' ',  '<', '>', '<', '<', '<', '<', '>' },
/* ( */    { '<', '<', '<', '<', '<', '<', '<', '<', '<',  '<',  '<', '=', '<', '<', '<', '<', ' ' },
/* ) */    { '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  ' ', '>', ' ', ' ', ' ', ' ', '>' },
/* ID */   { '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  ' ', '>', ' ', ' ', ' ', ' ', '>' },
/* INT */  { '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  ' ', '>', ' ', ' ', ' ', ' ', '>' },
/* FLOAT */{ '>', '>', '>', '>', '>', '>', '>', '>', '>',  '>',  ' ', '>', ' ', ' ', ' ', ' ', '>' },
/* STR */  { '>', ' ', ' ', ' ', '>', '>', '>', '>', '>',  '>',  ' ', '>', ' ', ' ', ' ', ' ', '>' },
/* $ */    { '<', '<', '<', '<', '<', '<', '<', '<', '<',  '<',  '<', ' ', '<', '<', '<', '<', '#' }
};

int convert_token(int token_number) {
    switch(token_number){
        case TOKEN_PLUS: return RT_TOKEN_PLUS;
        case TOKEN_MINUS: return RT_TOKEN_MINUS;
        case TOKEN_ASTERISK: return RT_TOKEN_ASTERISK;
        case TOKEN_SLASH: return RT_TOKEN_SLASH;
        case TOKEN_LESS_THAN: return RT_TOKEN_LESS_THAN;
        case TOKEN_LESS_THAN_OR_EQUAL: return RT_TOKEN_LESS_THAN_OR_EQUAL;
        case TOKEN_MORE_THAN: return RT_TOKEN_MORE_THAN;
        case TOKEN_MORE_THAN_OR_EQUAL: return RT_TOKEN_MORE_THAN_OR_EQUAL;
        case TOKEN_TYPE_EQUAL: return RT_TOKEN_TYPE_EQUAL;
        case TOKEN_TYPE_NOT_EQUAL: return RT_TOKEN_TYPE_NOT_EQUAL;
        case TOKEN_ROUND_BRACKET_LEFT: return RT_TOKEN_ROUND_BRACKET_LEFT;
        case TOKEN_ROUND_BRACKET_RIGHT: return RT_TOKEN_ROUND_BRACKET_RIGHT;
        case TOKEN_IDENTIFIER: return RT_TOKEN_IDENTIFIER;
        case TOKEN_INT: return RT_TOKEN_INT;
        case TOKEN_FLOAT: return RT_TOKEN_FLOAT;
        case TOKEN_STRING: return RT_TOKEN_STRING;
        //end of expr
        case TOKEN_COMMA:
        case TOKEN_SEMICOLON:
        case TOKEN_EOF:
        case TOKEN_CURLY_BRACKET_LEFT:
            return RT_TOKEN_DOLLAR_SIGN;

        default:   
            return ERROR_INTERNAL;
    }
    return -1;
}

int check_format(prec_stack_t *stack, int operator_type) {
    int op1, op2, operator;
    op1 = stack->top->element.type;
    op2 = stack->top->next->next->element.type;
    operator = stack->top->next->element.type;

    if(operator_type != operator || op1 != RT_EXPR || op2 != RT_EXPR)
        return ERROR_EXPRESSION_UNEVALUABLE;

    if(check_semantic(&(stack->top->element), stack->top->next->element.type, &(stack->top->next->next->element)) != ERROR_NO_ERROR)
        return ERROR_EXPRESSION_UNEVALUABLE;

    prec_stack_pop(stack);
    prec_stack_pop(stack);
    return ERROR_NO_ERROR;
}

int check_semantic(prec_elem_t *op1_left, int operator, prec_elem_t *op2_right) {
    if(operator == RT_TOKEN_PLUS){
        if ((op1_left->data_type == type_string && op2_right->data_type != type_string) ||
            (op1_left->data_type != type_string && op2_right->data_type == type_string)){
                return ERROR_EXPRESSION_UNEVALUABLE;
            } else if (op1_left->data_type == type_string || op2_right->data_type == type_string){
                if(op1_left->data_type == type_int && op2_right->data_type == type_float){
                    op1_left->data_type = type_float;
                }
                if(op2_right->data_type == type_int && op1_left->data_type == type_float){
                    op2_right->data_type = type_float;
                }
            }
    }

    if(operator == RT_TOKEN_MINUS){
        if(op1_left->data_type == type_string || op2_right->data_type == type_string){
            return ERROR_EXPRESSION_UNEVALUABLE;
        } else if (op1_left->data_type == type_string || op2_right->data_type == type_string){
                if(op1_left->data_type == type_int && op2_right->data_type == type_float){
                    op1_left->data_type = type_float;
                }
                if(op2_right->data_type == type_int && op1_left->data_type == type_float){
                    op2_right->data_type = type_float;
                }
            }
    }

    if(operator == RT_TOKEN_ASTERISK){
        if(op1_left->data_type == type_string || op2_right->data_type == type_string){
            return ERROR_EXPRESSION_UNEVALUABLE;
        } else if (op1_left->data_type == type_string || op2_right->data_type == type_string){
                if(op1_left->data_type == type_int && op2_right->data_type == type_float){
                    op1_left->data_type = type_float;
                }
                if(op2_right->data_type == type_int && op1_left->data_type == type_float){
                    op2_right->data_type = type_float;
                }
            }
    }

    if(operator == RT_TOKEN_SLASH){
        if(op1_left->data_type == type_string || op2_right->data_type == type_string){
            return ERROR_EXPRESSION_UNEVALUABLE;
        } else if (op1_left->data_type == type_string || op2_right->data_type == type_string){
                if(op1_left->data_type == type_int && op2_right->data_type == type_float){
                    op1_left->data_type = type_float;
                }
                if(op2_right->data_type == type_int && op1_left->data_type == type_float){
                    op2_right->data_type = type_float;
                }
            }
    }

    return ERROR_NO_ERROR;
}    

int shift(prec_stack_t *stack, ParserData *data) {
    bst_node_t *node;
    int type = convert_token(data->token.type);
    if(data->token.type == TOKEN_IDENTIFIER) {
        if((node = symtable_search(data->local_table, data->token.lexeme->str)) != NULL) {
            if(node->symbol.defined != true){
                return ERROR_VARIABLE_UNDEFINED;
            }
            else {
                if (prec_stack_push(stack, type, true, node->symbol.data_type, data->token.lexeme->str) != ERROR_NO_ERROR)
                    return ERROR_INTERNAL;
                return ERROR_NO_ERROR;
            }

            if (prec_stack_push(stack, type, true, node->symbol.data_type, data->token.lexeme->str) != ERROR_NO_ERROR)
                    return ERROR_INTERNAL;
                return ERROR_NO_ERROR;
        }
    }
    switch (type) {
        case RT_TOKEN_INT:
            if (prec_stack_push(stack, type, true, type_int, data->token.lexeme->str) != ERROR_NO_ERROR)
                return ERROR_INTERNAL;
            
            return ERROR_NO_ERROR;

        case RT_TOKEN_FLOAT:
            if (prec_stack_push(stack, type, true, type_float, data->token.lexeme->str) != ERROR_NO_ERROR)
                return ERROR_INTERNAL;
            return ERROR_NO_ERROR;

        case RT_TOKEN_STRING:
            if (prec_stack_push(stack, type, true, type_string, data->token.lexeme->str) != ERROR_NO_ERROR)
                return ERROR_INTERNAL;
            return ERROR_NO_ERROR;

        default:
            if (prec_stack_push(stack, type, true, type_undefined, data->token.lexeme->str) != ERROR_NO_ERROR)
                return ERROR_INTERNAL;
            return ERROR_NO_ERROR;
    }

    return ERROR_NO_ERROR;
}

prec_item_t *get_first_nonterminal(prec_stack_t *stack) {
    prec_item_t *tmp = stack->top;
    while(tmp != NULL) {
        if(tmp->element.terminal == true)
            return tmp;

        tmp = tmp->next;
    }
    return NULL;
}

int reduce(prec_stack_t *stack, ParserData *data) {
    prec_item_t *first_nonterm = get_first_nonterminal(stack);
    bst_node_t *tempNode = NULL;
    bool stop = false;
    stack_item *tempItem = stack_top(&(data->symstack));
    if(first_nonterm == NULL)
        return ERROR_INTERNAL;

    switch (first_nonterm->element.type) {
    case RT_TOKEN_PLUS:
        if((err = check_format(stack, RT_TOKEN_PLUS)) != ERROR_NO_ERROR){
            return err;
        }
        break;

    case RT_TOKEN_MINUS:
        if((err = check_format(stack, RT_TOKEN_MINUS)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_ASTERISK:
        if((err = check_format(stack, RT_TOKEN_ASTERISK)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_SLASH:
        if((err = check_format(stack, RT_TOKEN_SLASH)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_LESS_THAN:
        if((err = check_format(stack, RT_TOKEN_LESS_THAN)) != ERROR_NO_ERROR)
            return err;
        break;
    
    case RT_TOKEN_LESS_THAN_OR_EQUAL:
        if((err = check_format(stack, RT_TOKEN_LESS_THAN_OR_EQUAL)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_MORE_THAN:
        if((err = check_format(stack, RT_TOKEN_MORE_THAN)) != ERROR_NO_ERROR)
            return err;
        break;
    
    case RT_TOKEN_MORE_THAN_OR_EQUAL:
        if((err = check_format(stack, RT_TOKEN_MORE_THAN_OR_EQUAL)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_TYPE_EQUAL:
        if((err = check_format(stack, RT_TOKEN_TYPE_EQUAL)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_TYPE_NOT_EQUAL:
        if((err = check_format(stack, RT_TOKEN_TYPE_NOT_EQUAL)) != ERROR_NO_ERROR)
            return err;
        break;

    case RT_TOKEN_FLOAT:
    case RT_TOKEN_INT:
    case RT_TOKEN_STRING:
        first_nonterm->element.type = RT_EXPR;
        first_nonterm->element.terminal = false;
        break;

    case RT_TOKEN_IDENTIFIER:
            while ((tempItem != NULL) && (!stop)) {
                if (tempItem->in_func == true) {
                    stop = true;
                }
                if ((tempNode = symtable_search(tempItem->symtable, get_first_nonterminal(stack)->element.name)) != NULL) {
                    break;
                }
                else {
                    tempItem = tempItem->next;
                }
            }

            if ((tempNode == NULL) && (!stop)) {

                tempNode = symtable_search(data->local_table, data->token.lexeme->str);
            }
            if (tempNode == NULL) {
                return ERROR_VARIABLE_UNDEFINED;
            }
        first_nonterm->element.type = RT_EXPR;
        first_nonterm->element.terminal = false;
        break;
    }

    return ERROR_NO_ERROR;
}

void reduce_brackets(prec_stack_t *stack) {
    prec_item_t * tmp = stack->top->next;
    pop3(stack);
    prec_stack_push(stack, tmp->element.type, tmp->element.terminal, tmp->element.data_type, tmp->element.name);
}

int analyze(ParserData *data) {
    prec_stack_t stack;
    int sign;

    prec_stack_init(&stack);

    if((err = get_token(&(data->token))) != ERROR_NO_ERROR)
            return err;

    while(true){
        if(data->token.type == TOKEN_DOLLAR_SIGN){
            if((err = get_token(&(data->token))) != ERROR_NO_ERROR)
                return err;
            continue;
        }
        if (convert_token(data->token.type) == RT_TOKEN_ROUND_BRACKET_LEFT) {
            open_brackets++;
        }

        if (convert_token(data->token.type) == RT_TOKEN_ROUND_BRACKET_RIGHT && open_brackets == 0 && 
            stack.top->element.type == RT_EXPR && get_first_nonterminal(&stack)->element.type == RT_TOKEN_DOLLAR_SIGN) {
            ungetc(')', stdin);
            return ERROR_NO_ERROR;
        }

        if (convert_token(data->token.type) == ERROR_INTERNAL )
            return ERROR_INTERNAL;

        sign = table[get_first_nonterminal(&stack)->element.type][convert_token(data->token.type)];    

        switch (sign) {
            case '<':
                if((err = shift(&stack, data)) != ERROR_NO_ERROR)
                    return err;
                if((err = get_token(&(data->token))) != ERROR_NO_ERROR)
                    return err;
                break;

            case '>':
                if((err = reduce(&stack, data)) != ERROR_NO_ERROR)
                    return err;
                break;

            case '=':
                shift(&stack, data);
                reduce_brackets(&stack);
                if((err = get_token(&(data->token))) != ERROR_NO_ERROR)
                    return err;
                break;

            case '#':
                if(!(stack.top->element.terminal))
                    data->type = stack.top->element.data_type;

                if(data->token.type == TOKEN_COMMA)
                    ungetc(',', stdin);

                else if(data->token.type == TOKEN_SEMICOLON)
                    ungetc(';', stdin);

                else if (data->token.type == EOF)
                    ungetc(EOF, stdin);

                return ERROR_NO_ERROR;
                break;

            case ' ':
                return ERROR_SEMANTIC_OTHER;
                break;
        }


    }
    prec_free_stack(&stack);
}

void pop3(prec_stack_t* stack) {
    prec_stack_pop(stack);
    prec_stack_pop(stack);
    prec_stack_pop(stack);
}
