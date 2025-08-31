#include "pack.h"
#include "string_tree.h"
#include <stdint.h>
#include <string.h>

#define SHIFT_AS_NULL 0xffffffff

void count_elements_rec(token* begin, packinfo* info){
	while(begin){
        if(begin->origin && begin->down) info->nodes += 2;
		else info->nodes++;

		if(begin->tok)
            info->text_length += strlen(begin->tok) + 1;

		count_elements_rec(begin->down, info);
		begin = begin->next;
	}
}

packinfo count_elements(token* begin){
	packinfo info = {256, 0};

	size_t i = 256;
	while(i--)
		if(begin[i].tok)
			count_elements_rec(begin + i, &info);

	return info;
}

#define HAVE_VALUE  2
#define HAVE_NEXT   1

int haveNext(uint8_t* ptr, size_t pos){
    ptr += pos / 4 + (pos & 3 ? 1 : 0);
    return (*ptr >> (pos & 3) * 2) & HAVE_NEXT;
}

int haveValue(uint8_t* ptr, size_t pos){
    ptr += pos / 4 + (pos & 3 ? 1 : 0);
    return (*ptr >> (pos & 3) * 2) & HAVE_VALUE;
}

void setFlags(uint8_t* ptr, size_t pos, uint8_t val){
    ptr += pos / 4 + (pos & 3 ? 1 : 0);
    uint8_t mask = 3 << (pos & 3) * 2;
    *ptr &= ~mask;

    val <<= (pos & 3) * 2;
    *ptr |= val;
}

void pack_rec(size_t pos, token* node, pack* package);

void pack_childs(token* node, pack* package){
    size_t childs = 1;
    for(token* ptr = node; ptr = ptr->next; childs++);

    package->info.nodes += childs;

    pack_rec(package->info.nodes - childs, node, package);
}

void pack_rec(size_t pos, token* node, pack* package){
    typedef enum nodeType{
        hasDown = 1,
        hasNext = 2,
        hasValue = 4
    } nodeType;

    do{
        uint8_t type =
            (node->down ? hasDown : 0) +
            (node->next ? hasNext : 0) +
            (node->origin ? hasValue : 0);

        size_t childId;
        switch (type) {
            case hasValue:
                package->values[pos] = node->origin;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;
                setFlags(package->flags, pos, HAVE_VALUE);
                return;

            case hasValue | hasDown:
                package->values[pos] = (void*)package->info.nodes;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;

                childId = package->info.nodes;
                package->values[childId] = node->origin;
                package->text_shifts[childId] = package->info.text_length - 1;
                setFlags(package->flags, childId, HAVE_VALUE);
                package->info.nodes++;

                pack_childs(node->down, package);
                break;

            case hasValue | hasNext:
                package->values[pos] = node->origin;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;
                setFlags(package->flags, pos, HAVE_VALUE | HAVE_NEXT);
                package->info.nodes++;
                break;

            case hasDown:
                package->values[pos] = (void*)package->info.nodes;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;
                pack_childs(node->down, package);
                break;

            case hasDown | hasNext:
                package->values[pos] = (void*)package->info.nodes;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;
                setFlags(package->flags, pos, HAVE_NEXT);
                pack_childs(node->down, package);
                break;

            case hasValue | hasDown | hasNext:
                package->values[pos] = (void*)package->info.nodes;
                package->text_shifts[pos] = package->info.text_length;
                strcpy(package->texts + package->info.text_length, node->tok);
                package->info.text_length += strlen(node->tok) + 1;
                setFlags(package->flags, pos, HAVE_NEXT);

                childId = package->info.nodes;
                package->values[childId] = node->origin;
                package->text_shifts[childId] = package->info.text_length - 1;
                setFlags(package->flags, childId, HAVE_NEXT | HAVE_VALUE);
                package->info.nodes++;

                pack_childs(node->down, package);
                break;
        }

        pos++;
    }
    while(node = node->next);
}

pack pack_tree(token* begin){
    pack package;

	package.info = count_elements(begin);

	size_t mem =
        package.info.nodes * sizeof(void*) +
        package.info.nodes * sizeof(uint32_t) +
        package.info.nodes / 4 +
        package.info.text_length;

	if(package.info.nodes & 3) mem++;

	mem += package.info.text_length;



	package.values =
        (void**)malloc(mem);

    package.text_shifts =
        (uint32_t*)(package.values + package.info.nodes);

    package.flags =
        (uint8_t*)(package.text_shifts + package.info.nodes);

	package.texts =
        (char*)package.flags + package.info.nodes / 4;

	if(package.info.nodes & 3)
        package.texts++;

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

        if(begin[i].origin && begin[i].down){
            package.values[i] = (void*)package.info.nodes;
            setFlags(package.flags, i, HAVE_VALUE);

            package.values[package.info.nodes] = begin->origin;
            package.text_shifts[package.info.nodes] = full_info.text_length - 1;
            setFlags(package.flags, package.info.nodes, HAVE_VALUE | HAVE_NEXT);

            package.info.nodes++;

            pack_childs(begin[i].down, &package);
            continue;
        }


        if(begin[i].origin){
            package.values[i] = begin[i].origin;
            setFlags(package.flags, i, HAVE_VALUE);
            continue;
        }

        if(begin[i].down) {
            package.values[i] = (void*)package.info.nodes++;
            pack_childs(begin[i].down, &package);
        }
	}

	return package;
}

void* find_pack_element(char* src, pack package){
    return 0;
}
