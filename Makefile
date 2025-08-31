SRCs=$(wildcard *.c)

wb.o: $(SRCs:.c=.o)
	ld -r $+ -o $@

%.o: %.c %.h
	gcc $< -c $@

clean:
	rm *.o