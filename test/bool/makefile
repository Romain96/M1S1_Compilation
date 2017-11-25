EXEC = bool
CC = gcc
FLAGS = -Wall
LEX = lex
YACC = yacc
LIBS = -ly -lfl

all: $(EXEC).l $(EXEC).y symbol_table.c symbol_table.h quad.c quad.h list.h list.c
	gcc -c symbol_table.c
	gcc -c quad.c
	gcc -c list.c
	$(YACC) -d $(EXEC).y
	$(LEX) -l $(EXEC).l
	gcc -o $(EXEC) lex.yy.c y.tab.c symbol_table.o quad.o list.o $(FLAGS) $(LIBS)

clean:
	rm $(EXEC) lex.yy.c y.tab.h y.tab.c *.o