# Makefile pour le lex
# Auteur : Romain PERRIN
# juste un petit makefile pour tester l'analyse lexicale seule

# génère un code d'analyse lexical en utilisant (f)lex
all: lexical_analysis.l
	lex -v lexical_analysis.l
	gcc -o test lex.yy.c -ll

# nettoie le dossier de travail
clean:
	rm lex.yy.c test
