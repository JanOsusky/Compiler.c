/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Implementace skeneru (scanner.c)
 *
 *   @author David Prochazka (xproch0q)
 */

#include <stdio.h>

#include "scanner.h"
#include "error.h"

const char prolog[] = "<?php declare(strict_types=1);";
int prolog_check = 0;

int get_token(lexToken *token)
{
    int c, esc_seq, i;
    int state = STATE_START;
    strFree(token->lexeme);

    while (1) {
        c = getchar();

        switch (state) {
            case STATE_PROLOG:
                if (prolog[i] == '\0') {
                    ungetc(c, stdin);
                    prolog_check = 1;
                    state = STATE_START;
                }
                else if (i == 5) {
                    if ((c != ' ') && (c != '\t') && (c != '\n')) {
                        ungetc(c, stdin);
                        i++;
                    }
                }
                else if (c == prolog[i]) {
                    i++;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_START:
                if (!prolog_check) {
                    ungetc(c, stdin);
                    i = 0;
                    state = STATE_PROLOG;
                }
                else if ((c >= '0') && (c <= '9')) {
                    ungetc(c, stdin);
                    state = STATE_INTEGER;
                }
                else if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_')) {
                    ungetc(c, stdin);
                    state = STATE_IDENTIFIER;
                }
                else if (c == '"') {
                    state = STATE_STRING;
                }
                else if (c == '/') {
                    state = STATE_SLASH;
                }
                else if (c == '<') {
                    state = STATE_LESS_THAN;
                }
                else if (c == '>') {
                    state = STATE_MORE_THAN;
                }
                else if (c == '=') {
                    state = STATE_EQUAL_1;
                }
                else if (c == '!') {
                    state = STATE_NOT_EQUAL_1;
                }
                else if (c == '?') {
                    state = STATE_NULL_TYPE;
                }
                else if (c == '+') {
                    token->type = TOKEN_PLUS;
                    return ERROR_NO_ERROR;
                }
                else if (c == '-') {
                    token->type = TOKEN_MINUS;
                    return ERROR_NO_ERROR;
                }
                else if (c == '*') {
                    token->type = TOKEN_ASTERISK;
                    return ERROR_NO_ERROR;
                }
                else if (c == '.') {
                    token->type = TOKEN_DOT;
                    return ERROR_NO_ERROR;
                }
                else if (c == ',') {
                    token->type = TOKEN_COMMA;
                    return ERROR_NO_ERROR;
                }
                else if (c == ';') {
                    token->type = TOKEN_SEMICOLON;
                    return ERROR_NO_ERROR;
                }
                else if (c == ':') {
                    token->type = TOKEN_COLON;
                    return ERROR_NO_ERROR;
                }
                else if (c == '$') {
                    token->type = TOKEN_DOLLAR_SIGN;
                    return ERROR_NO_ERROR;
                }
                else if (c == '(') {
                    token->type = TOKEN_ROUND_BRACKET_LEFT;
                    return ERROR_NO_ERROR;
                }
                else if (c == ')') {
                    token->type = TOKEN_ROUND_BRACKET_RIGHT;
                    return ERROR_NO_ERROR;
                }
                else if (c == '{') {
                    token->type = TOKEN_CURLY_BRACKET_LEFT;
                    return ERROR_NO_ERROR;
                }
                else if (c == '}') {
                    token->type = TOKEN_CURLY_BRACKET_RIGHT;
                    return ERROR_NO_ERROR;
                }
                else if (c == EOF) {
                    token->type = TOKEN_EOF;
                    return ERROR_NO_ERROR;
                }
                else if ((c != ' ') && (c != '\t') && (c != '\n')) {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_INTEGER:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_INTEGER;
                }
                else if (c == '.') {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_POINT;
                }
                else if ((c == 'e') || (c == 'E')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_INT;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_DECIMAL_POINT:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_POINT_DECIMAL;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_DECIMAL_POINT_DECIMAL:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_POINT_DECIMAL;
                }
                else if ((c == 'e') || (c == 'E')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_FLOAT;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_DECIMAL_E:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E_DECIMAL;
                }
                else if ((c == '+') || (c == '-')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E_SIGN;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_DECIMAL_E_SIGN:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E_DECIMAL;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_DECIMAL_E_DECIMAL:
                if ((c >= '0') && (c <= '9')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_DECIMAL_E_DECIMAL;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_FLOAT;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_IDENTIFIER:
                if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_')) {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_IDENTIFIER;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_IDENTIFIER;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_STRING:
                if ((c < ' ') || (c == '$')) {
                    return ERROR_BAD_LEXEME;
                }
                else if (c == '"') {
                    token->type = TOKEN_STRING;
                    return ERROR_NO_ERROR;
                }
                else if (c == '\\') {
                    state = STATE_ESCAPE_SEQUENCE;
                }
                else {
                    if (!addChar(c, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                break;

            case STATE_ESCAPE_SEQUENCE:
                if (c == 'x') {
                    esc_seq = 0;
                    state = STATE_HEX_CHAR_1;
                }
                else if ((c >= '0') && (c <= '3')) {
                    esc_seq = 64 * (c - '0');
                    state = STATE_OCT_CHAR_1;
                }
                else if (c == '"') {
                    if (!addChar('"', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if (c == 'n') {
                    if (!addChar('\n', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if (c == 't') {
                    if (!addChar('\t', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if (c == '\\') {
                    if (!addChar('\\', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if (c == '$') {
                    if (!addChar('$', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else {
                    if (!addChar('\\', token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    ungetc(c, stdin);
                    state = STATE_STRING;
                }
                break;

            case STATE_HEX_CHAR_1:
                if ((c >= '0') && (c <= '9')) {
                    esc_seq += 16 * (c - '0');
                    state = STATE_HEX_CHAR_2;
                }
                else if ((c >= 'a') && (c <= 'f')) {
                    esc_seq += 16 * (c - 'a' + 10);
                    state = STATE_HEX_CHAR_2;
                }
                else if ((c >= 'A') && (c <= 'F')) {
                    esc_seq += 16 * (c - 'A' + 10);
                    state = STATE_HEX_CHAR_2;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_HEX_CHAR_2:
                if ((c >= '0') && (c <= '9')) {
                    esc_seq += c - '0';
                    if (!addChar(esc_seq, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if ((c >= 'a') && (c <= 'f')) {
                    esc_seq += c - 'a' + 10;
                    if (!addChar(esc_seq, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else if ((c >= 'A') && (c <= 'F')) {
                    esc_seq += c - 'A' + 10;
                    if (!addChar(esc_seq, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_OCT_CHAR_1:
                if ((c >= '0') && (c <= '7')) {
                    esc_seq += 8 * (c - '0');
                    state = STATE_OCT_CHAR_2;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_OCT_CHAR_2:
                if ((c >= '0') && (c <= '7')) {
                    esc_seq += c - '0';
                    if (!addChar(esc_seq, token->lexeme)) {
                        return ERROR_INTERNAL;
                    }
                    state = STATE_STRING;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_SLASH:
                if (c == '/') {
                    state = STATE_LINE_COMMENT;
                }
                else if (c == '*') {
                    state = STATE_BLOCK_COMMENT;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_SLASH;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_LINE_COMMENT:
                if (c == '\n') {
                    state = STATE_START;
                }
                else if (c == EOF) {
                    token->type = TOKEN_EOF;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_BLOCK_COMMENT:
                if (c == '*') {
                    state = STATE_BLOCK_COMMENT_END;
                }
                else if (c == EOF) {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_BLOCK_COMMENT_END:
                if (c == '*') {
                    state = STATE_BLOCK_COMMENT_END;
                }
                else if (c == '/') {
                    state = STATE_START;
                }
                else if (c == EOF) {
                    return ERROR_BAD_LEXEME;
                }
                else {
                    state = STATE_BLOCK_COMMENT;
                }
                break;

            case STATE_LESS_THAN:
                if (c == '=') {
                    token->type = TOKEN_LESS_THAN_OR_EQUAL;
                    return ERROR_NO_ERROR;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_LESS_THAN;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_MORE_THAN:
                if (c == '=') {
                    token->type = TOKEN_MORE_THAN_OR_EQUAL;
                    return ERROR_NO_ERROR;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_MORE_THAN;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_EQUAL_1:
                if (c == '=') {
                    state = STATE_EQUAL_2;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_EQUAL;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_EQUAL_2:
                if (c == '=') {
                    token->type = TOKEN_TYPE_EQUAL;
                    return ERROR_NO_ERROR;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_NOT_EQUAL_1:
                if (c == '=') {
                    state = STATE_NOT_EQUAL_2;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_NOT_EQUAL_2:
                if (c == '=') {
                    token->type = TOKEN_TYPE_NOT_EQUAL;
                    return ERROR_NO_ERROR;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;

            case STATE_NULL_TYPE:
                if (c == '>') {
                    state = STATE_EPILOG;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_QUESTION_MARK;
                    return ERROR_NO_ERROR;
                }
                break;

            case STATE_EPILOG:
                if (c == EOF) {
                    token->type = TOKEN_EOF;
                    return ERROR_NO_ERROR;
                }
                else {
                    return ERROR_BAD_LEXEME;
                }
                break;
        }
    }
}
