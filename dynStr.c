/**
 *   IFJ - Formalni jazyky a prekladace
 *   zimni semestr 2022/2023
 *
 *   Dynamicky string (dynStr.c)
 *
 *   @author Vojtech Adamek (xadame44)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynStr.h"

dynStr *dynStrInit (void) {
	dynStr *dynStr = malloc(sizeof(*dynStr));		
	if (dynStr == NULL) return NULL;

	dynStr->str = calloc(1, sizeof(char) * ALLOC_SIZE);
	if(dynStr->str == NULL) return NULL;
	
	dynStr->allocated = ALLOC_SIZE;
	dynStr->len = 0;
	return dynStr;
}

void dynStrFree (dynStr *dynStr) {
	if(dynStr->str != NULL)
		free(dynStr->str);
	free(dynStr);
}

void strFree (dynStr *dynStr) {
	dynStr->len = 0;
	strcpy(dynStr->str, "");
}

int addChar (char c, dynStr *dynStr) {
	if (dynStr->len == dynStr->allocated-1) {
		dynStr->str = realloc(dynStr->str, dynStr->allocated + ALLOC_SIZE);		
		if (dynStr->str == NULL) 
			return 0;
		
		dynStr->allocated += ALLOC_SIZE;
	}

	dynStr->str[dynStr->len] = c;
	dynStr->str[++(dynStr->len)] = '\0';
	return 1;
}
