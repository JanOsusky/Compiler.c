/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Zavadec prekladace (main.c)
 *
 *   @author David Prochazka (xproch0q)
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "error.h"

int main()
{
    int err_code = parser();

    switch (err_code) {
        case ERROR_NO_ERROR:
            fprintf(stderr, "Preklad probehl bez chyb\n");
            return 0;
        case ERROR_BAD_LEXEME:
            fprintf(stderr, "Chyba 1: Skener narazil na neplatny lexem\n");
            break;
        case ERROR_BAD_SYNTAX:
            fprintf(stderr, "Chyba 2: Zdrojovy soubor ma chybnou syntaxi\n");
            break;
        case ERROR_FUNCTION_UNDEFINED:
            fprintf(stderr, "Chyba 3: Funkce neni definovana / snaha o redefinici\n");
            break;
        case ERROR_PARAMETER_MISMATCH:
            fprintf(stderr, "Chyba 4: Neocekavany pocet nebo typ parametru funkce\n");
            break;
        case ERROR_VARIABLE_UNDEFINED:
            fprintf(stderr, "Chyba 5: Promenna neni definovana\n");
            break;
        case ERROR_BAD_RETURN:
            fprintf(stderr, "Chyba 6: Neocekavany zpusob navratu z funkce\n");
            break;
        case ERROR_EXPRESSION_UNEVALUABLE:
            fprintf(stderr, "Chyba 7: Konverze datoveho typu se nezdarila\n");
            break;
        case ERROR_SEMANTIC_OTHER:
            fprintf(stderr, "Chyba 8: Jina semanticka chyba\n");
            break;
        case ERROR_INTERNAL:
            fprintf(stderr, "Chyba 99: Interni chyba prekladace\n");
            break;
    }

    exit(err_code);
}
