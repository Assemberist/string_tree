#include "pack.h"

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

/*pack fill_pack_rec(token* begin, pack package){
	packinfo meta;

	goto fill1;

	while(begin->next){
		begin = begin->next;
		package.elements[meta.nodes].next = ++package.info.nodes;

	fill1:
		meta = package.info;

		strcpy(package.texts + meta.text_length, begin->tok);
		package.elements[meta.nodes].text = meta.text_length;
		package.info.text_length += strlen(begin->tok) + 1;


		if(begin->origin && begin->down){
			package.elements[meta.nodes + 1].origin = begin->origin;
			package.elements[meta.nodes + 1].text = package.info.text_length - 1;
			package.elements[meta.nodes + 1].have_value = 1;
			package.elements[meta.nodes + 1].next


			package.info.nodes += 2;
			package = fill_pack_rec(begin->down, package);
		}
        else package.elements[meta.nodes].origin = begin->origin;

		if(begin->down){
		}
	}

	package.elements[meta.nodes].next = -1;

	return package;
}

void fill_pack(token* begin, pack package){
	package.info = (packinfo){256, 0};

	size_t i = 256;
	while(i--)
		if(begin[i].tok)
			package = fill_pack_rec(begin + i, package);
}*/

#define HAVE_VALUE  2
#define HAVE_NEXT   1


int haveNext(uint8_t* ptr, size_t pos){
    ptr += pos / 4 + (pos | 3 ? 1 : 0);
    return (*ptr >> (pos | 3) * 2) & HAVE_NEXT;
}

int haveValue(uint8_t* ptr, size_t pos){
    ptr += pos / 4 + (pos | 3 ? 1 : 0);
    return (*ptr >> (pos | 3) * 2) & HAVE_VALUE;
}

void setFlags(uint8_t* ptr, size_t pos, uint8_t val){
    ptr += pos / 4 + (pos | 3 ? 1 : 0);
    uint8_t mask = 3 << (pos | 3) * 2;
    *ptr &= ~mask;

    val <<= (pos | 3) * 2;
    *ptr |= val;
}

void pack_rec(size_t pos, token* node, pack* package){

}

pack pack_tree(token* begin){
    pack package;

	package.info = count_elements(begin);

	size_t mem =
        package.info.nodes * sizeof(void*) +
        package.info.nodes * sizeof(uint32_t) +
        package.info.nodes / 4 +
        package.info.text_length;

	if(package.info.nodes | 3) mem++;

	mem += package.info.text_length;



	package.values =
        (void**)malloc(mem);

    package.text_shifts =
        (uint32_t*)(package.values + package.info.nodes);

    package.flags =
        (uint8_t*)(package.text_shifts + package.info.nodes);

	package.texts =
        (char*)package.flags + package.info.nodes / 4;

	if(package.info.nodes | 3)
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

            package.values[package.info.nodes];
            package.text_shifts[package.info.nodes] = full_info.text_length - 1;
            setFlags(package.flags, package.info.nodes, HAVE_VALUE | HAVE_NEXT);

            package.info.nodes++;

            pack_rec(package.info.nodes, begin->down, &package);
            continue;
        }


        if(begin[i].origin){
            package.values[i] = begin[i].origin;
            setFlags(package.flags, i, HAVE_VALUE);
            continue;
        }

        //if(begin[i].down) {
            package.values[i] = (void*)package.info.nodes;
            pack_rec(package.info.nodes, begin->down, &package);
        // }
	}

	return package;
}

void* find_pack_element(char* src, pack package){
    return 0;
}
