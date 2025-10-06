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

void set_stop_value(void* stop);
void reset_stop_value(pack p, void* stop);

pack pack_tree(token* begin);
void* find_pack_element(char* src, pack package);
void remove_pack(pack package);

size_t get_pack_depth(pack p);

// ToDo below //

void import_words(char* path);
void import_tree(char* path);
void export_tree(char* path, pack package);

void* search_pack(char* src, pack package);
void* update_pack(char* src, void* element, pack package);

// text not included
// stored in pack.info.text_length
size_t get_pack_data_length(pack package);

token* unpack_tree(void* pack);

#endif
