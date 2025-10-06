#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../pack.h"

packinfo count_elements(token* begin);
int haveValue(uint8_t* ptr, size_t pos);
int haveNext(uint8_t* ptr, size_t pos);


int main(){
    token* tree = init_tree();

    char* values[] =
        {
            "flex",
            "myst",
            "energy",
            "gryaz",
            "pil",
            "air",
            "fire",
            "water",
            "ground"
        };
    
    for(int i = 0; i < sizeof(values)/sizeof(char*); i++)
        set_value(values[i], (void*)(i+1), tree);

    packinfo infi = count_elements(tree);

    pack p = pack_tree(tree);

    // pack size
	size_t mem =
        get_pack_data_length(p) +
        infi.text_length;

    FILE* out = fopen("out.bin", "wb");
    fwrite(p.values, mem, 1, out);
    fclose(out);

    remove_pack(p);

    remove_tree(tree);
}