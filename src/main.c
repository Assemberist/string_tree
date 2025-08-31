#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pack.h"

packinfo count_elements(token* begin);
int haveValue(uint8_t* ptr, size_t pos);
int haveNext(uint8_t* ptr, size_t pos);


int main(){
    token* tree = init_tree();

    /*
    set_value("arka", (void*)1, tree);
    set_value("arfa", (void*)2, tree);
    set_value("arkan", (void*)3, tree);

    packinfo infi = count_elements(tree);

    pack p = pack_tree(tree);
*/

/*	size_t mem =
        infi.nodes * sizeof(void*) +
        infi.nodes * sizeof(uint32_t) +
        infi.nodes / 4 +
        (infi.nodes & 3 ? 1 : 0) +
        infi.text_length;

    FILE* out = fopen("out.bin", "wb");

    fwrite(p.values, mem, 1, out);
*/

/*  for(size_t i = 256; i < 256 + p.info.nodes; i++)
        printf("%s -> %d %s %s\n",
               p.texts + p.text_shifts[i],
               haveValue(p.flags, i) ? (int)p.values[i] : 0,
               haveValue(p.flags, i) ? "VAL" : "",
               haveNext(p.flags, i) ? "NEXT" : "");
*/

/*
    char* tests[] = 
        {
            "or",
            "a",
            "ar",
            "ard",
            "ark",
            "arka",
            "arkan",
            "arkania",
            "arkat",
            "arf",
            "arfa",
            "arfan"
        };

    for(size_t i = sizeof(tests)/sizeof(char*); i--;)
        printf("%s -> %d\n", tests[i], (int)find_pack_element(tests[i], p));
*/    

    typedef struct word{
        char* str;
        struct word* next;
    } word;

    word w;
    word* end = &w;    

    char buffer[256];
    FILE* r = fopen("words.txt", "r");

    while(fgets(buffer, sizeof(buffer), r)){
        end->next = (word*)malloc(sizeof(word));
        strtok(buffer, "\n");
        end = end->next;
        end->str = strdup(buffer);
        end->next = NULL;
    }

    end = w.next;
    while(end){
        set_value(end->str, end->str, tree);
        end = end->next;
    }

    end = w.next;
    while(end){
        char* val = get_value(end->str, tree);
        if(strstr(val, end->str)) printf("%s -> %s\n", end->str, val);
        end = end->next;
    }

    remove_tree(tree);

    return 0;
}
