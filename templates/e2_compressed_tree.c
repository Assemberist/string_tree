#include "../pack.h"
#include "../hide_lib.h"
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
	remove_tree(tree);							// frees used memory.

    for(int i = 0; i < p.info.nodes; i++)
        if(p.values[i])
            printf("%i:\t%s -> %i    %s %s\n",
                    i,
                    p.texts + p.text_shifts[i],
                    p.values[i],
                    haveNext(p.flags, i) ? "Next" : "----",
                    haveValue(p.flags, i) ? "Val" : "---");

    puts("\n");

    for(size_t i = sizeof(tests)/sizeof(char*); i--;)
        printf("%s -> %d\n", tests[i], find_pack_element(tests[i], p));

    remove_pack(p);
}
