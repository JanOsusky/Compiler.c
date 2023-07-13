/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Implementace skeneru (scanner.h)
 *
 *   @author David Prochazka (xproch0q)
 */

#ifndef _SCANNER_H
#define _SCANNER_H

#include "dynStr.h"

#define STATE_PROLOG 0
#define STATE_START 1
#define STATE_INTEGER 2
#define STATE_DECIMAL_POINT 3
#define STATE_DECIMAL_POINT_DECIMAL 4
#define STATE_DECIMAL_E 5
#define STATE_DECIMAL_E_SIGN 6
#define STATE_DECIMAL_E_DECIMAL 7
#define STATE_IDENTIFIER 8
#define STATE_STRING 9
#define STATE_ESCAPE_SEQUENCE 10
#define STATE_HEX_CHAR_1 11
#define STATE_HEX_CHAR_2 12
#define STATE_OCT_CHAR_1 13
#define STATE_OCT_CHAR_2 14
#define STATE_SLASH 15
#define STATE_LINE_COMMENT 16
#define STATE_BLOCK_COMMENT 17
#define STATE_BLOCK_COMMENT_END 18
#define STATE_LESS_THAN 19
#define STATE_MORE_THAN 20
#define STATE_EQUAL_1 21
#define STATE_EQUAL_2 22
#define STATE_NOT_EQUAL_1 23
#define STATE_NOT_EQUAL_2 24
#define STATE_NULL_TYPE 25
#define STATE_EPILOG 26

#define TOKEN_INT 0
#define TOKEN_FLOAT 1
#define TOKEN_IDENTIFIER 2
#define TOKEN_STRING 3
#define TOKEN_PLUS 4
#define TOKEN_MINUS 5
#define TOKEN_ASTERISK 6
#define TOKEN_SLASH 7
#define TOKEN_LESS_THAN 8
#define TOKEN_LESS_THAN_OR_EQUAL 9
#define TOKEN_MORE_THAN 10
#define TOKEN_MORE_THAN_OR_EQUAL 11
#define TOKEN_EQUAL 12
#define TOKEN_TYPE_EQUAL 13
#define TOKEN_TYPE_NOT_EQUAL 14
#define TOKEN_DOT 15
#define TOKEN_COMMA 16
#define TOKEN_SEMICOLON 17
#define TOKEN_COLON 18
#define TOKEN_QUESTION_MARK 19
#define TOKEN_DOLLAR_SIGN 20
#define TOKEN_ROUND_BRACKET_LEFT 21
#define TOKEN_ROUND_BRACKET_RIGHT 22
#define TOKEN_CURLY_BRACKET_LEFT 23
#define TOKEN_CURLY_BRACKET_RIGHT 24
#define TOKEN_EOF 25

typedef struct {
    dynStr *lexeme;
    int type;
} lexToken;

int get_token(lexToken *token);

#endif
