run:
	if [[ -L makefile ]]; then cd .. && make $(shell pwd)/test; else make all; fi

all: $(addsuffix /test, $(filter-out ., $(shell find -maxdepth 1 -type d)))

clean:
	if [[ -L makefile ]]; then cd .. && make $(shell pwd)/clean; else make clean_all; fi

clean_all: $(addsuffix /clean, $(filter-out ., $(shell find -maxdepth 1 -type d)))
	rm string_tree.o
	 
%/test: string_tree.o %/main.o
	gcc $^ -o $@

%/main.o: string_tree.o %/main.c
	gcc -c $(@:.o=.c) -o $@

%/clean:
	# That works automatically.
	# rm $(@:/clean=/main.o)
	rm $(@:/clean=/test)

string_tree.o: ../src/string_tree.c ../src/string_tree.h
	gcc -c ../src/string_tree.c -o ../templates/string_tree.o
