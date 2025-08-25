#ifndef STRING_TREE
#define STRING_TREE

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct token{
	char* tok;
	void* origin;
	struct token* next;
	struct token* down;
}token;

token* init_tree();
void set_value(char* src, void* element, token* begin);
void* get_value(char* src, token* begin);
void* delete_pair(char* src, token* begin);
void remove_tree(token* begin);

#endif
