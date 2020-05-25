all: clean
	gcc -Wall -Wextra -pedantic -o2 -o jcedit jcedit.c syntax.c

run: all
	./jcedit

clean:
	rm -f jcedit
