#include "string_tree.h"
#include <stdio.h>

int main(){
    token* begin = init_tree();					// string_tree is created. All memory is dynamically allocated.

    set_value("arka", (void*)1, begin);
    set_value("arfa", (void*)2, begin);
    set_value("ar", (void*)3, begin);

	printf("\"arka\" found -> %i\n",
			get_value("arka", begin));
	printf("\"arfa\" found -> %i\n",
			get_value("arfa", begin)); 
	printf("\"none\" not found -> %i\n", 
			get_value("none", begin));

	remove_tree(begin);							// frees used memory.
}
