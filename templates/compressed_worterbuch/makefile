all: test

test: wortbook.o main.c
	gcc *.o main.c -o test -g

wortbook.o: inc/wortbook.o
	cp $< $@

inc/wortbook.o: $(wildcard inc/*.c) $(wildcard inc/*.h)
	cd inc && make

pack.o: inc/pack.c inc/pack.h
	gcc inc/pack.c -c pack.o -g

	wortbook.o