all: test

test: string_tree.o pack.o main.c
	gcc *.o main.c -o test -g

string_tree.o: inc/string_tree.c inc/string_tree.h
	gcc inc/string_tree.c -c string_tree.o -g

pack.o: inc/pack.c inc/pack.h
	gcc inc/pack.c -c pack.o -g