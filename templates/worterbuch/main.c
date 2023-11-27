#include "string_tree.h"
#include <stdio.h>

int main(){
    token* begin = init_tree();					// string_tree is created. All memory is dynamically allocated.

    add_element("arka", (void*)1, begin);
    add_element("arfa", (void*)2, begin);
    add_element("ar", (void*)3, begin);

	printf("\"arka\" found -> %i\n",
			find_element("arka", begin));
	printf("\"arfa\" found -> %i\n",
			find_element("arfa", begin)); 
	printf("\"none\" not found -> %i\n", 
			find_element("none", begin));

	remove_tree(begin);							// frees used memory.
}
