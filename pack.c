#include <string.h>

#include "pack.h"
#include "hide_lib.h"

#define SHIFT_AS_NULL 0xffffffff

void count_elements_rec(token* begin, packinfo* info, void* stopPtr){
	while(begin){
        if(begin->origin != stopPtr && begin->down) info->nodes += 2;
		else info->nodes++;

		if(begin->tok)
            info->text_length += strlen(begin->tok) + 1;

		count_elements_rec(begin->down, info, stopPtr);
		begin = begin->next;
	}
}

packinfo count_elements(token* begin, void* stopPtr){
	packinfo info = {256, 0};

	size_t i = 256;
	while(i--)
		if(begin[i].tok){
            info.nodes--;
			count_elements_rec(begin + i, &info, stopPtr);
        }

	return info;
}

int haveNext(uint8_t* ptr, size_t pos){
    ptr += pos / 4;
    return (*ptr >> (pos & 3) * 2) & HAVE_NEXT;
}

int haveValue(uint8_t* ptr, size_t pos){
    ptr += pos / 4;
    return (*ptr >> (pos & 3) * 2) & HAVE_VALUE;
}

void setFlags(uint8_t* ptr, size_t pos, uint8_t val){
    ptr += pos / 4;
    uint8_t mask = 3 << (pos & 3) * 2;
    *ptr &= ~mask;

    val <<= (pos & 3) * 2;
    *ptr |= val;
}

void pack_rec(size_t pos, token* node, pack* package, void* stopPtr);

void pack_childs(token* node, pack* package, void* stopPtr){
    size_t childs = 1;
    for(token* ptr = node; ptr = ptr->next; childs++);

    package->info.nodes += childs;

    pack_rec(package->info.nodes - childs, node, package, stopPtr);
}

void pack_rec(size_t pos, token* node, pack* package, void* stopPtr){
    enum {
        hasDown = 1,
        hasNext = 2,
        hasValue = 4
    } nodeType;

    do{
        uint8_t type =
            (node->down ? hasDown : 0) +
            (node->next ? hasNext : 0) +
            (node->origin != stopPtr ? hasValue : 0);

        package->text_shifts[pos] = package->info.text_length;
        strcpy(package->texts + package->info.text_length, node->tok);
        package->info.text_length += strlen(node->tok) + 1;

        size_t childId;
        switch (type) {
            case hasValue:
                package->values[pos] = node->origin;
                setFlags(package->flags, pos, HAVE_VALUE);
                return;

            case hasValue | hasDown:
                package->values[pos] = (void*)package->info.nodes;
                
                childId = package->info.nodes;
                package->values[childId] = node->origin;
                package->text_shifts[childId] = package->info.text_length - 1;
                setFlags(package->flags, childId, HAVE_VALUE);
                package->info.nodes++;

                pack_childs(node->down, package, stopPtr);
                break;

            case hasValue | hasNext:
                package->values[pos] = node->origin;
                setFlags(package->flags, pos, HAVE_VALUE | HAVE_NEXT);
                break;

            case hasDown:
                package->values[pos] = (void*)package->info.nodes;
                pack_childs(node->down, package, stopPtr);
                break;

            case hasDown | hasNext:
                package->values[pos] = (void*)package->info.nodes;
                setFlags(package->flags, pos, HAVE_NEXT);
                pack_childs(node->down, package, stopPtr);
                break;

            case hasValue | hasDown | hasNext:
                package->values[pos] = (void*)package->info.nodes;
                setFlags(package->flags, pos, HAVE_NEXT);

                childId = package->info.nodes;
                package->values[childId] = node->origin;
                package->text_shifts[childId] = package->info.text_length - 1;
                setFlags(package->flags, childId, HAVE_NEXT | HAVE_VALUE);
                package->info.nodes++;

                pack_childs(node->down, package, stopPtr);
                break;
        }

        pos++;
    }
    while(node = node->next);
}

pack pack_tree(token* begin, void* stopPtr){
    pack package;

	package.info = count_elements(begin, stopPtr);

	size_t mem =
        get_pack_data_length(package) +
        package.info.text_length;

        

	package.values =
        (void**)malloc(mem);

    package.text_shifts =
        (uint32_t*)(package.values + package.info.nodes);

    package.flags =
        (uint8_t*)(package.text_shifts + package.info.nodes);

	package.texts =
        (char*)package.flags + package.info.nodes / 4;

    memset(package.values, 0, mem);



    packinfo full_info = package.info;
	package.info = (packinfo){256, 0};
	for(int i = 0; i < 256; i++){
        if(!begin[i].tok){
            package.text_shifts[i] = full_info.text_length - 1;
            continue;
        }

        package.text_shifts[i] = package.info.text_length;
        strcpy(package.texts + package.info.text_length, begin[i].tok);
        package.info.text_length += strlen(begin[i].tok) + 1;

        if(begin[i].origin != stopPtr && begin[i].down){
            package.values[i] = (void*)package.info.nodes;
            setFlags(package.flags, i, HAVE_VALUE);

            package.values[package.info.nodes] = begin->origin;
            package.text_shifts[package.info.nodes] = full_info.text_length - 1;
            setFlags(package.flags, package.info.nodes, HAVE_VALUE | HAVE_NEXT);

            package.info.nodes++;

            pack_childs(begin[i].down, &package, stopPtr);
            continue;
        }


        if(begin[i].origin != stopPtr){
            package.values[i] = begin[i].origin;
            setFlags(package.flags, i, HAVE_VALUE);
            continue;
        }

        if(begin[i].down) {
            package.values[i] = (void*)package.info.nodes;
            pack_childs(begin[i].down, &package, stopPtr);
        }
	}

	return package;
}

void* find_pack_element_rec(size_t pos, char* src, pack* package, void* stopPtr){
    while(1){
        // find next (match 1st letters)
        while(package->texts[package->text_shifts[pos]] != src[0]){
            if(haveNext(package->flags, pos)) pos++;
            else return stopPtr;
        }

        char* tokPtr = package->texts + package->text_shifts[pos];
        size_t diff = strdif(src, tokPtr);

        // token not fully matched.
        if(tokPtr[diff]) return stopPtr;

        // there is no childs
        if(haveValue(package->flags, pos)){
            if(!src[diff]) // and token matched
                return package->values[pos];
            else // src is more than last token.
                return stopPtr;
        }

        if(package->values[pos] == stopPtr) return stopPtr;

        pos = (size_t)package->values[pos];
        src += diff;
    }
}

void* find_pack_element(char* src, pack package, void* stopPtr){
    if(package.texts + package.text_shifts[src[0]])
        return find_pack_element_rec(src[0], src, &package, stopPtr);
    else
        return stopPtr;
}

void remove_pack(pack package){
    free(package.values);
}

size_t get_pack_data_length(pack package){
    return  package.info.nodes * sizeof(void*) +
            package.info.nodes * sizeof(uint32_t) +
            package.info.nodes / 4;
}