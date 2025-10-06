#include "../pack.h"
#include <stdio.h>

int main(){
    token* tree = init_tree();					// string_tree is created. All memory is dynamically allocated.

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
        set_value(tests[i], (void*)(i+1), tree);

	pack p = pack_tree(tree);
	remove_tree(tree);

    printf("Max depth = %d", get_pack_depth(p));

    // frees used memory.
    remove_pack(p);
}
