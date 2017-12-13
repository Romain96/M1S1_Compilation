// tests d'erreurs sémantiques
int main()
{
	// erreur 1 : utiliser un variable non déclarée
	a = 0;

	// erreur 2 : utiliser une varaible non initialisée
	int b;
	b++;

	// erreur 3 : accéder à une case (dim 2) d'un tableau (dim 3)
	int tab[] = {{{1,2},{3,4}},{{5,6},{7,8}}};
	int d = tab[0][0];
	printi(d);

	// obligatoire dans notre grammaire
	return 0;
}
