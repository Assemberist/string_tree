#ifndef PACK_H
#define PACK_H

#include "string_tree.h"

typedef struct packinfo{
	size_t nodes;
	size_t text_length;
} packinfo;

typedef struct pack{
	packinfo info;

    void** values;
    uint32_t* text_shifts;
    uint8_t *flags;

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
