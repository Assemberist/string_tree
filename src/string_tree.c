#include "string_tree.h"
#include <stddef.h>
#include <stdio.h>

void remove_rec(token* begin){
	if(begin->down) {
		remove_rec(begin->down);
		free(begin->down);
	}
	if(begin->next){
		remove_rec(begin->next);
		free(begin->next);
	}
	free(begin->tok);
}

void remove_tree(token* begin){
	size_t len = 256;
	while(len--) remove_rec(begin + len);
	free(begin);
}

token* init_tree(){
	token* ptr = (token*)malloc(sizeof(token)<<8);
	memset(ptr, 0, sizeof(token)<<8);
	return ptr;
}

token* new_token(char* _token){
	token* ret = (token*)malloc(sizeof(token));
	memset(ret, 0, sizeof(token));
	if(_token){
		ret->tok = (char*)malloc(strlen(_token)+1);
		strcpy(ret->tok, _token);
	}
	return ret;
}

size_t strdif(char* str1, char* str2){
	size_t ret = 0;
	while(*(str1++)==*(str2++)) {
		++ret;
		if(!*str1) return ret;
	}
	return ret;
}

token* slide_down(char* src, token* begin){
	char syn;
	
	if(*src){
		goto enter;

		while(begin->next){
			begin = begin->next;
		
			enter:
		
			if(syn = strdif(begin->tok, src)){
				if(syn < strlen(begin->tok)){
					token* ptr = new_token(begin->tok + syn);
					ptr->origin = begin->origin;
					begin->origin = 0;
					ptr->down = begin->down;
					begin->tok[syn] = 0;
					begin->down = ptr;
				}
				if(syn < strlen(src)) 
					return begin->down ? slide_down(src + syn, begin->down) : (begin->down = new_token(src + syn));
				return begin;
			}
		}
	}
	else while(begin->next) begin = begin->next;

	begin->next = new_token(src);
	return begin->next;
}

void set_value(char* src, void* element, token* begin){
	token* ptr = begin + *src;
	if(ptr->tok) slide_down(src, ptr)->origin = element;
	else{
		ptr->origin = element;
		ptr->tok = (char*)malloc(strlen(src)+1);
		strcpy(ptr->tok, src);
	}
}

void* find_down(char* src, token* begin){
	while(begin){
		if(!begin->tok) return 0;
		if(*begin->tok == *src){
			size_t diff = strdif(begin->tok, src);

			if(strlen(begin->tok) == strlen(src)) 
				return begin->origin;

			if(diff < strlen(src))
				return find_down(src + diff, begin->down);

			return 0;
		}
		else begin = begin->next;
	}
	return 0;
}

void* get_value(char* src, token* begin){
	return find_down(src, begin + *src);
}

void count_elements_rec(token* begin, packinfo* info){
	while(begin){
		info->nodes++;
		info->text_length += strlen(begin->tok) + 1;

		count_elements_rec(begin->down, info);
		begin = begin->next;
	}
}

packinfo count_elements(token* begin){
	packinfo info = {0, 0};

	size_t i = 256;
	while(i--)
		if(begin[i].tok)
			count_elements_rec(begin + i, &info);

	return info;
}

pack fill_pack_rec(token* begin, pack package){
	packinfo this;

	goto fill1;

	while(begin->next){
		begin = begin->next;
		package.elements[this.nodes].next = ++package.info.nodes;

	fill1:
		this = package.info;

		strcpy(package.texts + this.text_length, begin->tok);
		package.elements[this.nodes].text = this.text_length;
		package.info.text_length += strlen(begin->tok) + 1;

		package.elements[this.nodes].origin = begin->origin;

		if(begin->down){
			++package.info.nodes;
			package = fill_pack_rec(begin->down, package);
		}
	}

	package.elements[this.nodes].next = -1;

	return package;
}

void fill_pack(token* begin, pack package){
	package.info = (packinfo){0, 0};

	size_t i = 256;
	while(i--)
		if(begin[i].tok)
			package = fill_pack_rec(begin + i, package);
}

pack pack_tree(token* begin){
    pack package;

	package.info = count_elements(begin);
	size_t mem = package.info.nodes * sizeof(pack_element);

	package.elements = (pack_element*)malloc(package.info.text_length + mem);
	package.texts = (char*)package.elements + mem;

	fill_pack(begin, package);

	return package;
}
