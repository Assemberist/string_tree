#include <stdio.h>

#include "string_tree.h"
#include "hide_lib.h"

void remove_rec(token* begin){
    if(begin->down){
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

token* slide_down(char* src, token* begin){
	char syn;

	if(*src){
		goto enter;

		while(begin->next){
			begin = begin->next;

			enter:

			if(syn = strdif(begin->tok, src)){
				if(begin->tok[syn]){
					token* ptr = new_token(begin->tok + syn);
					ptr->origin = begin->origin;
					begin->origin = 0;
					ptr->down = begin->down;
					begin->tok[syn] = 0;
					begin->down = ptr;
				}
				if(src[syn]){
					if(begin->down){
						src += syn;
						begin = begin->down;
						goto enter;
					}
					else return begin->down = new_token(src + syn);
				}
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

			if(!begin->tok[diff]){
				if(!src[diff]) return begin->origin;
				else {
					src += diff;
					begin = begin->down;
					continue;
				}
			}

			return 0;
		}
		else begin = begin->next;
	}
	return 0;
}

void* get_value(char* src, token* begin){
	return find_down(src, begin + *src);
}
