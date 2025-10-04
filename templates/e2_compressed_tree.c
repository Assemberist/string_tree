#include "../pack.h"
#include <stdio.h>

int main(){
    token* tree = init_tree();					// string_tree is created. All memory is dynamically allocated.

    // set_value(key, value, tree); 
    // Don't use NULL as value!!!
    set_value("arka", (void*)1, tree);
    set_value("arkan", (void*)2, tree);
    set_value("arfa", (void*)3, tree);

	pack p = pack_tree(tree);
	remove_tree(tree);							// frees used memory.

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
        printf("%s -> %d\n", tests[i], find_pack_element(tests[i], p));

    remove_pack(p);
}
