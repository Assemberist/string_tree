SRCs=$(wildcard *.c)

lib: test

test: $(SRCs:.c=.o)
	gcc $+ -o $@ -g

%.o: %.c
	gcc -c $< -g