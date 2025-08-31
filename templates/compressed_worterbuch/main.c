#include <stdio.h>

#include "inc/pack.h"

packinfo count_elements(token* begin);
int haveValue(uint8_t* ptr, size_t pos);
int haveNext(uint8_t* ptr, size_t pos);


int main(){
    token* tree = init_tree();

    set_value("arka", (void*)1, tree);
    set_value("arfa", (void*)2, tree);
    set_value("arkan", (void*)3, tree);

    packinfo infi = count_elements(tree);

	size_t mem =
        infi.nodes * sizeof(void*) +
        infi.nodes * sizeof(uint32_t) +
        infi.nodes / 4 +
        (infi.nodes & 3 ? 1 : 0) +
        infi.text_length;

    pack p = pack_tree(tree);

    for(size_t i = 256; i < 261; i++)
        printf("%s -> %d %s %s\n",
               p.texts + p.text_shifts[i],
               haveValue(p.flags, i) ? (int)p.values[i] : 0,
               haveValue(p.flags, i) ? "VAL" : "",
               haveNext(p.flags, i) ? "NEXT" : "");

    remove_tree(tree);

    return 0;
}
