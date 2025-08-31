#include "hide_lib.h"

size_t strdif(char* str1, char* str2){
	size_t ret = 0;

	if(*str1 == '\0' || *str2 == '\0') return 0;

	while(*(str1++)==*(str2++)) {
		++ret;
		if(!*str1) return ret;
	}
	return ret;
}