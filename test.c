int main()
{
	int tableau[] = {1,2,3,4,5};
	int nb;
	nb = tableau[4];
	printf("la 5e valeur du tableau est ");
	printi(nb);
	printf("\n");
	printf("mettons 42 dans la 5e case du tableau\n");
	tableau[4] = 42;
	printf("la 5e valeur du tableau est ");
	nb = tableau[4];
	printi(nb);
	printf("\n");
	return 0;
}
