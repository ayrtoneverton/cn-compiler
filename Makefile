# Makefile

CPNAME = cn.exe

main: all clean

all: syntactic.tab.c lex.yy.c
	gcc -Wall -ansi -pedantic syntactic.tab.c -o $(CPNAME)

d: syntactic.tab.c lex.yy.c
	gcc -g -Wall -ansi -pedantic syntactic.tab.c -o $(CPNAME)
	gdb $(CPNAME)

syntactic.tab.c: syntactic.y
	bison -d -v -g syntactic.y

lex.yy.c: lexicon.l
	flex lexicon.l

clean:
	rm -f *.yy.c *.yy.h *.tab.c *.tab.h *.output *.dot

clean-all: clean
	rm -f $(CPNAME)
