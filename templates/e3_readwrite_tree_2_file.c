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

    
    set_value("arka", (void*)1, tree);
    set_value("arfa", (void*)2, tree);
    set_value("arkan", (void*)3, tree);

    packinfo infi = count_elements(tree);

    pack p = pack_tree(tree);

    // pack size
	size_t mem =
        infi.nodes * sizeof(void*) +
        infi.nodes * sizeof(uint32_t) +
        infi.nodes / 4 +
        (infi.nodes & 3 ? 1 : 0) +
        infi.text_length;

    FILE* out = fopen("out.bin", "wb");
    fwrite(p.values, mem, 1, out);
    fclose(out);

    remove_pack(p);

    remove_tree(tree);
}