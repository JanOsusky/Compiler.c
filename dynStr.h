/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Dynamicky string (dynStr.h)
 *
 *   @author Vojtech Adamek (xadame44)
 */

#ifndef _DYNSTR_H
#define _DYNSTR_H

#define ALLOC_SIZE 8

typedef struct {
	char *str;
	unsigned int allocated;
	unsigned int len;
} dynStr;	

/**
 * Creates and initializes structure for dynamic string.
 * 
 * Returns NULL if malloc fails in allocating structure or
 * allocating string, else returns pointer to created structure.
 */
dynStr *dynStrInit(void);

/**
 * Frees the dynamic structure.
 */
void dynStrFree(dynStr *dynStr);

/**
 * Adds a single character behind the last character in 
 * the string.
 * 
 * If length of the string with added character would exceed
 * allocated size, reallocs the string to a larger size.
 * 
 * Returns 0 if reallocation has failed.
 */
int addChar(char c, dynStr *dynStr);

/**
 * Frees only the string of the structure.
 */
void strFree(dynStr *dynStr);

#endif
