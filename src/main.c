#include "string_tree.h"
#include <stdio.h>

int main(){
    token* begin = init_tree();
    add_element("arka", (void*)0, begin);
    add_element("arfa", (void*)1, begin);
    add_element("ar", (void*)2, begin);

    pack package = pack_tree(begin);

    remove_tree(begin);
}
