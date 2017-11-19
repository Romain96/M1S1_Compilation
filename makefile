# génère un code d'analyse lexical en utilisant (f)lex
all: stenc.l stenc.y
	yacc -d stenc.y
	lex stenc.l
	gcc -o stenc y.tab.c y.tab.h lex.yy.c -ly -ll

# nettoie le dossier de travail (supprime les fichier générés ppar Lex et l'exécutable)
clean:
	rm *.o y.tab.c y.tab.h lex.yy.c stenc
