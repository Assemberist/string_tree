#ifndef STRING_TREE
#define STRING_TREE

#include <stdlib.h>
#include <string.h>

typedef struct token{
	char* tok;
	void* origin;
	struct token* next;
	struct token* down; 
}token;

token* init_tree();
void add_element(char* src, void* element, token* begin);
void update_element(char* src, void* element, token* begin);
void* find_element(char* src, token* begin);
void remove_tree(token* begin);

void* pack_tree(token* begin);
token* unpack_tree(void* pack);

void import_words(char* path);
void import_tree(char* path);

void export_tree(char* path, void* pack);

void* update_pack(char* src, void* element, void* begin);
void* find_element(char* src, token* begin);

#endif
