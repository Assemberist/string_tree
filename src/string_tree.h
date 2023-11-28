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

typedef struct pack_element{
	void* origin;
	uint32_t text;
	uint32_t next;
} pack_element;

typedef struct packinfo{
	size_t nodes;
	size_t text_length;
} packinfo;

typedef struct pack{
	packinfo info;
	pack_element* elements;
	char* texts;
} pack;

pack pack_tree(token* begin);
token* unpack_tree(void* pack);

void import_words(char* path);
void import_tree(char* path);
void export_tree(char* path, pack package);

void* search_pack(char* src, pack package);
void* update_pack(char* src, void* element, pack package);
void* find_pack_element(char* src, pack package);

void remove_pack(pack package);

#endif
