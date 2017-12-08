EXEC = stenc
CC = gcc
FLAGS = -Wall
LEX = lex
YACC = yacc
LIBS = -ly -lfl

all: $(EXEC).l $(EXEC).y symbol_table.c symbol_table.h quad.c quad.h list.h list.c mips_generator.h mips_generator.c array_parser.c array_parser.h stack.c stack.h
	gcc -c symbol_table.c
	gcc -c quad.c
	gcc -c list.c
	gcc -c mips_generator.c
	gcc -c stack.c
	gcc -c array_parser.c
	$(YACC) -d $(EXEC).y
	$(LEX) -l $(EXEC).l
	gcc -o $(EXEC) lex.yy.c y.tab.c symbol_table.o quad.o list.o mips_generator.o stack.o array_parser.o $(FLAGS) $(LIBS)

clean:
	rm $(EXEC) lex.yy.c y.tab.h y.tab.c *.o
