SRCs=$(wildcard *.c)

all: $(basename $(SRCs))

e%: wb.o e%.c
	gcc $+ -o $@

wb.o: ../wb.o
	cp $< $@

../wb.o: $(wildcard ../*.c) $(wildcard ../*.h)
	cd .. && make

clean:
	rm $(basename $(SRCs))
	rm ../*.o
	rm *.o