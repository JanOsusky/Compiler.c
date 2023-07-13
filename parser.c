/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Implementace parseru (parser.c)
 *
 *   @author Vojtech Adamek (xadame44)
 *   @author Jan Osusky (xosusk00)
 *   @author David Prochazka (xproch0q)
 */

#include <stdio.h>

#include "parser.h"
#include "precanal.h"
#include "error.h"
#include "codegen.h"

#define GET_NEXT_TOKEN                                                  \
    if ((err_code = get_token(&(parserData->token))) != ERROR_NO_ERROR) \
    {                                                                   \
        return err_code;                                                \
    }

// rules starting from non-terminal <prog>
int prog(ParserData *parserData)
{
    int err_code;
    bst_node_t *tempNode;
    bst_func_t *tempFunc;

    // rule <prog> -> ?>
    if (parserData->token.type == TOKEN_EOF)
    {
        return ERROR_NO_ERROR;
    }

    // rule <prog> -> IDENTIFIER
    else if (parserData->token.type == TOKEN_IDENTIFIER)
    {
        // rule <prog> -> function id (<out_param_init>) : <type> {<st_list>} <prog>
        if (!strcmp(parserData->token.lexeme->str, "function"))
        {
            GET_NEXT_TOKEN;
            if (parserData->token.type == TOKEN_IDENTIFIER)
            {
                // inserting the label of the function to the global symbol table
                if ((tempFunc = symtable_search_func(parserData->global_table, parserData->token.lexeme->str)) != NULL)
                {
                    return ERROR_FUNCTION_UNDEFINED;
                }
                else
                {
                    symtable_insert_func(&(parserData->global_table), parserData->token.lexeme->str);
                    symtable_init(&tempNode);
                    stack_push(&(parserData->symstack), tempNode);
                    stack_top(&(parserData->symstack))->in_func = true;
                }
                GENERATE_CODE(generate_function_start, parserData->token.lexeme->str);
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_ROUND_BRACKET_LEFT)
                {
                    return ERROR_BAD_SYNTAX;
                }
                else
                {
                    GET_NEXT_TOKEN;
                    err_code = out_param_init(parserData);
                    if (err_code != ERROR_NO_ERROR)
                    {
                        return err_code;
                    }
                    else
                    {
                        GET_NEXT_TOKEN;
                        if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
                        {
                            return ERROR_BAD_SYNTAX;
                        }
                        else
                        {

                            GET_NEXT_TOKEN;
                            if (parserData->token.type != TOKEN_COLON)
                            {
                                return ERROR_BAD_SYNTAX;
                            }
                            else
                            {
                                GET_NEXT_TOKEN;
                                err_code = type(parserData, tempFunc);
                                if (err_code != ERROR_NO_ERROR)
                                {
                                    return err_code;
                                }
                                else
                                {
                                    GET_NEXT_TOKEN;
                                    if (parserData->token.type != TOKEN_CURLY_BRACKET_LEFT)
                                    {
                                        return ERROR_BAD_SYNTAX;
                                    }
                                    else
                                    {
                                        err_code = st_list(parserData);
                                        if (err_code != ERROR_NO_ERROR)
                                        {
                                            return err_code;
                                        }
                                        else
                                        {
                                            GENERATE_CODE(generate_function_end, parserData.token.lexeme->str)
                                            if (parserData->token.type != TOKEN_CURLY_BRACKET_RIGHT)
                                            {
                                                return ERROR_BAD_SYNTAX;
                                            }
                                            else
                                            {
                                                GET_NEXT_TOKEN;
                                                return prog(parserData);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                return ERROR_BAD_SYNTAX;
            }
            stack_pop(&(parserData->symstack));
        }

        // rule <prog> -> <stat> <prog>
        else
        {
            err_code = stat(parserData);
            if (err_code != ERROR_NO_ERROR)
            {
                return err_code;
            }
            else
            {
                GET_NEXT_TOKEN;
                return prog(parserData);
            }
        }
    }

    else if (parserData->token.type == TOKEN_DOLLAR_SIGN)
    {
        err_code = stat(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            GET_NEXT_TOKEN;
            return prog(parserData);
        }
    }

    /*
       after all the recursive runs of all <prog> rules, we always have to apply
       this final rule (because source code of the program is not infinite)
    */
    // rule <prog> -> ɛ
    return ERROR_NO_ERROR;
}

// rules starting from non-terminal <st_list>
int st_list(ParserData *parserData)
{
    int err_code;
    GET_NEXT_TOKEN;

    // rule <st_list> -> <stat> <st_list>
    if (parserData->token.type != TOKEN_CURLY_BRACKET_RIGHT)
    {
        GENERATE_CODE(generate_main_scope_start);
        err_code = stat(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            GENERATE_CODE(generate_main_scope_end);
            return st_list(parserData);
        }
    }

    /*
       after all the recursive runs of all <st_list> rules, we always have to apply
       this final rule (because source code of the program is not infinite)
    */
    // rule <st_list> -> ɛ
    return ERROR_NO_ERROR;
}

// rules starting from non-terminal <stat>
int stat(ParserData *parserData)
{
    bst_node_t *tempNode;
    int err_code;

    // rule <stat> -> IDENTIFIER
    if (parserData->token.type == TOKEN_IDENTIFIER)
    {

        // rule <stat> -> if (<expr>) {<st_list>} else {<st_list>}
        if (!strcmp(parserData->token.lexeme->str, "if"))
        {
            symtable_init(&tempNode);
            stack_push(&(parserData->symstack), tempNode);

            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_ROUND_BRACKET_LEFT)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                // giving control to the operator-precedence analysis
                err_code = expr(parserData);
                if (err_code != ERROR_NO_ERROR)
                {
                    return err_code;
                }
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
                {
                    return ERROR_BAD_SYNTAX;
                }
                else
                {
                    GET_NEXT_TOKEN;
                    if (parserData->token.type != TOKEN_CURLY_BRACKET_LEFT)
                    {
                        return ERROR_BAD_SYNTAX;
                    }
                    else
                    {
                        GENERATE_CODE(generate_if_head);

                        err_code = st_list(parserData);
                        if (err_code != ERROR_NO_ERROR)
                        {
                            return err_code;
                        }
                        if (parserData->token.type != TOKEN_CURLY_BRACKET_RIGHT)
                        {
                            return ERROR_BAD_SYNTAX;
                        }
                        else
                        {
                            GET_NEXT_TOKEN;
                            // check if mandatory 'else' follows the 'if' statement
                            if (!strcmp(parserData->token.lexeme->str, "else"))
                            {

                                GET_NEXT_TOKEN;
                                if (parserData->token.type != TOKEN_CURLY_BRACKET_LEFT)
                                {
                                    return ERROR_BAD_SYNTAX;
                                }
                                else
                                {
                                    err_code = st_list(parserData);
                                    if (err_code != ERROR_NO_ERROR)
                                    {
                                        return err_code;
                                    }
                                    if (parserData->token.type != TOKEN_CURLY_BRACKET_RIGHT)
                                    {
                                        return ERROR_BAD_SYNTAX;
                                    }
                                }
                            }
                            else
                            {
                                return ERROR_BAD_SYNTAX;
                            }
                        }
                    }
                }
            }
            stack_pop(&(parserData->symstack));
            return ERROR_NO_ERROR;
        }

        // rule <stat> -> while (<expr>) {<st_list>}
        else if (!strcmp(parserData->token.lexeme->str, "while"))
        {
            symtable_init(&tempNode);
            stack_push(&(parserData->symstack), tempNode);
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_ROUND_BRACKET_LEFT)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                // giving control to the operator-precedence analysis
                err_code = expr(parserData);
                if (err_code != ERROR_NO_ERROR)
                {
                    return err_code;
                }
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
                {
                    return ERROR_BAD_SYNTAX;
                }
                else
                {
                    GET_NEXT_TOKEN;
                    if (parserData->token.type != TOKEN_CURLY_BRACKET_LEFT)
                    {
                        return ERROR_BAD_SYNTAX;
                    }
                    else
                    {
                        err_code = st_list(parserData);
                        if (err_code != ERROR_NO_ERROR)
                        {
                            return err_code;
                        }
                        else
                        {
                            if (parserData->token.type != TOKEN_CURLY_BRACKET_RIGHT)
                            {
                                return ERROR_BAD_SYNTAX;
                            }
                        }
                    }
                }
            }
            stack_pop(&(parserData->symstack));
            return ERROR_NO_ERROR;
        }

        // rule <stat> -> return <expr> ;
        else if (!strcmp(parserData->token.lexeme->str, "return"))
        {
            // giving control to the operator-precedence analysis
            err_code = expr(parserData);
            if (err_code != ERROR_NO_ERROR)
            {
                return err_code;
            }
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_SEMICOLON)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                return ERROR_NO_ERROR;
            }
        }

        // note: in case when function name on input
        // rule <stat> -> <init_rhs> ;
        else
        {
            err_code = init_rhs(parserData);
            if (err_code != ERROR_NO_ERROR)
            {
                return err_code;
            }
            else
            {
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_SEMICOLON)
                {
                    return ERROR_BAD_SYNTAX;
                }
                else
                {
                    return ERROR_NO_ERROR;
                }
            }
        }
    }

    // note: in case when '$' or expression on input
    // rule <stat> -> <init_rhs> ;
    else
    {
        err_code = init_rhs(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_SEMICOLON)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                return ERROR_NO_ERROR;
            }
        }
    }
}

// rules starting from non-terminal <init_rhs>
int init_rhs(ParserData *parserData)
{
    int err_code;
    bst_node_t *tempNode;
    stack_item *tempItem = stack_top(&(parserData->symstack));

    // rule <init_rhs> -> $id = <rhs>
    if (parserData->token.type == TOKEN_DOLLAR_SIGN)
    {
        GET_NEXT_TOKEN;

        if (parserData->token.type != TOKEN_IDENTIFIER)
        {
            return ERROR_BAD_SYNTAX;
        }
        else
        {
            bool stop = false;
            if (parserData->symstack.top != NULL)
            {
                while ((tempItem != NULL) && (!stop))
                {
                    if (tempItem->in_func == true)
                    {
                        stop = true;
                    }
                    if ((tempNode = symtable_search(tempItem->symtable, parserData->token.lexeme->str)) != NULL)
                    {
                        break;
                    }
                    else
                    {
                        tempItem = tempItem->next;
                    }
                }
                if ((tempNode == NULL) && (!stop))
                {
                    if (symtable_search(parserData->local_table, parserData->token.lexeme->str) == NULL)
                    {
                        symtable_insert_symbol(&(stack_top(&(parserData->symstack))->symtable), parserData->token.lexeme->str);
                        tempNode = symtable_search(parserData->symstack.top->symtable, parserData->token.lexeme->str);
                    }
                }

                else if (tempNode == NULL)
                {
                    symtable_insert_symbol(&(stack_top(&(parserData->symstack))->symtable), parserData->token.lexeme->str);
                    tempNode = symtable_search(parserData->symstack.top->symtable, parserData->token.lexeme->str);
                }
            }

            else
            {
                symtable_insert_symbol(&(parserData->local_table), parserData->token.lexeme->str);
                tempNode = symtable_search(parserData->local_table, parserData->token.lexeme->str);
            }

            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_EQUAL)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                GET_NEXT_TOKEN;
                return rhs(parserData, tempNode);
            }
        }
    }

    // rule <init_rhs> -> <rhs>
    else
    {
        return rhs(parserData, NULL);
    }
}

// rules starting from non-terminal <rhs>
int rhs(ParserData *parserData, bst_node_t *tempNode)
{
    int err_code;
    bst_func_t *tempFunc;
    // rule <rhs> -> id (<in_param_init>)
    if (parserData->token.type == TOKEN_IDENTIFIER)
    {
        if ((tempFunc = symtable_search_func(parserData->global_table, parserData->token.lexeme->str)) == NULL)
        {
            return ERROR_FUNCTION_UNDEFINED;
        }
        else
        {
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_ROUND_BRACKET_LEFT)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                err_code = in_param_init(parserData);
                if (err_code != ERROR_NO_ERROR)
                {
                    return err_code;
                }
                else
                {

                    if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
                    {
                        return ERROR_BAD_SYNTAX;
                    }
                    else
                    {
                        if (tempNode != NULL)
                        {

                            switch (tempFunc->func.return_type)
                            {
                            case return_int:
                                tempNode->symbol.data_type = type_int;
                                break;
                            case return_float:
                                tempNode->symbol.data_type = type_float;
                                break;
                            case return_string:
                                tempNode->symbol.data_type = type_string;
                                break;
                            case return_void:
                                if (tempNode->symbol.null_capable)
                                {
                                    tempNode->symbol.data_type = type_null;
                                }
                                else
                                {
                                    return ERROR_PARAMETER_MISMATCH;
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        return ERROR_NO_ERROR;
                    }
                }
            }
        }
    }

    // rule <rhs> -> <expr>
    else
    {

        // giving control to the operator-precedence analysis
        err_code = expr(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            // tempNode->symbol.data_type = parserData->type;
            return ERROR_NO_ERROR;
        }
    }

    return ERROR_BAD_SYNTAX;
}

// rules starting from non-terminal <in_param_init>
int in_param_init(ParserData *parserData)
{
    int err_code;

    // rule <in_param_init> -> <in_param> <in_param_list>
    if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
    {
        err_code = in_param(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            if (parserData->token.type == TOKEN_ROUND_BRACKET_RIGHT)
            {
                return ERROR_NO_ERROR;
            }
            GET_NEXT_TOKEN;
            return in_param_list(parserData);
        }
    }

    // rule <in_param_init> -> ɛ
    return ERROR_NO_ERROR;
}

// rules starting from non-terminal <in_param_list>
int in_param_list(ParserData *parserData)
{
    int err_code;

    // <in_param_list> -> , <in_param> <in_param_list>
    if (parserData->token.type == TOKEN_COMMA)
    {
        err_code = in_param(parserData);
        if (err_code != ERROR_NO_ERROR)
        {
            return err_code;
        }
        else
        {
            GET_NEXT_TOKEN;
            return in_param_list(parserData);
        }
    }

    /*
       after all the recursive runs of all <in_param_list> rules, we always have to apply
       this final rule (because source code of the program is not infinite)
    */
    // rule <in_param_list> -> ɛ
    return ERROR_NO_ERROR;
}

// rules starting from non-terminal <in_param>
int in_param(ParserData *parserData)
{
    int err_code;
    bst_node_t *tempNode;
    stack_item *tempItem = stack_top(&(parserData->symstack));
    GET_NEXT_TOKEN;
    GENERATE_CODE(generate_function_param_declare, parserData->token.lexeme->str);
    // rule <in_param> -> $id
    if (parserData->token.type == TOKEN_DOLLAR_SIGN)
    {
        GET_NEXT_TOKEN;
        if (parserData->token.type != TOKEN_IDENTIFIER)
        {
            return ERROR_BAD_SYNTAX;
        }
        else
        {
            bool stop = false;
            while ((tempItem != NULL) && (!stop))
            {
                if (tempItem->in_func == true)
                {
                    stop = true;
                }
                if ((tempNode = symtable_search(tempItem->symtable, parserData->token.lexeme->str)) != NULL)
                {
                    break;
                }
                else
                {
                    tempItem = tempItem->next;
                    GENERATE_CODE(generate_var_declare, parserData->token.type)
                }
            }
            if ((tempNode == NULL) && (!stop))
            {
                tempNode = symtable_search(parserData->local_table, parserData->token.lexeme->str);
            }
            if (tempNode == NULL)
            {
                return ERROR_VARIABLE_UNDEFINED;
            }

            return ERROR_NO_ERROR;
        }
    }

    // rule <in_param> -> LITERAL
    else if ((parserData->token.type == TOKEN_INT) || (parserData->token.type == TOKEN_FLOAT) || (parserData->token.type == TOKEN_STRING))
    {

        return ERROR_NO_ERROR;
    }

    else if (parserData->token.type == TOKEN_ROUND_BRACKET_RIGHT)
    {
        return ERROR_NO_ERROR;
    }

    // in case when no rule can be applied
    return ERROR_BAD_SYNTAX;
}

// rules starting from non-terminal <expr>
int expr(ParserData *parserData)
{
    return analyze(parserData);
}

// rules starting from non-terminal <type>
int type(ParserData *parserData, bst_func_t *tempFunc)
{
    int err_code;

    // rule <type> -> ?type
    if (parserData->token.type == TOKEN_QUESTION_MARK)
    {
        GET_NEXT_TOKEN;
        if (parserData->token.type != TOKEN_IDENTIFIER)
        {
            return ERROR_BAD_SYNTAX;
        }
        else
        {
            tempFunc->func.null_capable = true;
            if (!strcmp(parserData->token.lexeme->str, "int"))
            {
                symtable_change_rv(&tempFunc, return_int);
            }
            else if (!strcmp(parserData->token.lexeme->str, "float"))
            {
                symtable_change_rv(&tempFunc, return_float);
            }
            else if (!strcmp(parserData->token.lexeme->str, "string"))
            {
                symtable_change_rv(&tempFunc, return_string);
            }
            else
            {
                return ERROR_BAD_SYNTAX;
            }

            return ERROR_NO_ERROR;
        }
    }

    // rule <type> -> type
    else if (parserData->token.type == TOKEN_IDENTIFIER)
    {
        if (!strcmp(parserData->token.lexeme->str, "int"))
        {
            symtable_change_rv(&tempFunc, return_int);
        }
        else if (!strcmp(parserData->token.lexeme->str, "float"))
        {
            symtable_change_rv(&tempFunc, return_float);
        }
        else if (!strcmp(parserData->token.lexeme->str, "string"))
        {
            symtable_change_rv(&tempFunc, return_string);
        }
        else if (!strcmp(parserData->token.lexeme->str, "null"))
        {
            symtable_change_rv(&tempFunc, return_void);
        }
        else
        {
            return ERROR_BAD_SYNTAX;
        }

        return ERROR_NO_ERROR;
    }

    return ERROR_BAD_SYNTAX;
}

// rules starting from non-terminal <out_param_init>
int out_param_init(ParserData *parserData)
{
    int err_code;

    // rule <out_param_init> -> type $id <out_param_list>
    if ((!strcmp(parserData->token.lexeme->str, "int")) ||
        (!strcmp(parserData->token.lexeme->str, "float")) ||
        (!strcmp(parserData->token.lexeme->str, "string")) ||
        (!strcmp(parserData->token.lexeme->str, "?int")) ||
        (!strcmp(parserData->token.lexeme->str, "?float")) ||
        (!strcmp(parserData->token.lexeme->str, "?string")))
    {
        GET_NEXT_TOKEN;
        if (parserData->token.type != TOKEN_DOLLAR_SIGN)
        {
            return ERROR_BAD_SYNTAX;
        }
        else
        {
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_IDENTIFIER)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                symtable_insert_symbol(&(stack_top(&(parserData->symstack))->symtable), parserData->token.lexeme->str);
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_ROUND_BRACKET_RIGHT)
                    return out_param_list(parserData);
                else if (parserData->token.type == TOKEN_ROUND_BRACKET_RIGHT)
                    ungetc(')', stdin);
            }
        }
    }

    // rule <out_param_init> -> ɛ
    return ERROR_NO_ERROR;
}

// rules starting from non-terminal <out_param_list>
int out_param_list(ParserData *parserData)
{
    int err_code;

    // <out_param_list> -> , type $id <out_param_list>
    if (parserData->token.type == TOKEN_COMMA)
    {
        GET_NEXT_TOKEN;
        if ((!strcmp(parserData->token.lexeme->str, "int")) ||
            (!strcmp(parserData->token.lexeme->str, "float")) ||
            (!strcmp(parserData->token.lexeme->str, "string")) ||
            (!strcmp(parserData->token.lexeme->str, "?int")) ||
            (!strcmp(parserData->token.lexeme->str, "?float")) ||
            (!strcmp(parserData->token.lexeme->str, "?string")))
        {
            GET_NEXT_TOKEN;
            if (parserData->token.type != TOKEN_DOLLAR_SIGN)
            {
                return ERROR_BAD_SYNTAX;
            }
            else
            {
                GET_NEXT_TOKEN;
                if (parserData->token.type != TOKEN_IDENTIFIER)
                {
                    return ERROR_BAD_SYNTAX;
                }
                else
                {
                    symtable_insert_symbol(&(stack_top(&(parserData->symstack))->symtable), parserData->token.lexeme->str);
                    GET_NEXT_TOKEN;
                    return out_param_list(parserData);
                }
            }
        }
    }

    /*
       after all the recursive runs of all <out_param_list> rules, we always have to apply
       this final rule (because source code of the program is not infinite)
    */
    // rule <out_param_list> -> ɛ
    return ERROR_NO_ERROR;
}

// initialization of the parserData structure
void init_var(ParserData *parserData)
{
    symtable_func_init(&(parserData->global_table));
    symtable_init(&(parserData->local_table));
    insert_buildin(&(parserData->global_table));
    stack_init(&(parserData->symstack));
    parserData->token.lexeme = dynStrInit();
}

void insert_buildin(bst_func_t **func_table)
{
    bst_func_t *temp;

    symtable_insert_func(func_table, "reads");
    temp = symtable_search_func(*func_table, "reads");
    symtable_change_rv(&temp, return_string);

    symtable_insert_func(func_table, "readi");
    temp = symtable_search_func(*func_table, "readi");
    symtable_change_rv(&temp, return_int);

    symtable_insert_func(func_table, "readf");
    temp = symtable_search_func(*func_table, "readf");
    symtable_change_rv(&temp, return_float);

    symtable_insert_func(func_table, "write");
    temp = symtable_search_func(*func_table, "write");
    symtable_change_rv(&temp, return_void);

    symtable_insert_func(func_table, "strlen");
    temp = symtable_search_func(*func_table, "strlen");
    symtable_change_rv(&temp, return_int);

    symtable_insert_func(func_table, "substring");
    temp = symtable_search_func(*func_table, "substring");
    symtable_change_rv(&temp, return_string);

    symtable_insert_func(func_table, "ord");
    temp = symtable_search_func(*func_table, "ord");
    symtable_change_rv(&temp, return_int);

    symtable_insert_func(func_table, "chr");
    temp = symtable_search_func(*func_table, "chr");
    symtable_change_rv(&temp, return_string);
}

// main parser function, analyses the code through recursive descent
int parser()
{
    int err_code;
    ParserData parserData;
    init_var(&parserData);

    if ((err_code = get_token(&(parserData.token))) == ERROR_NO_ERROR)
    {
        err_code = prog(&parserData);
    }

    return err_code;
}
