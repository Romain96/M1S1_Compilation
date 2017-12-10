int main()
{
	int tab[] = {1,2,3,4,5};
	int size = 5;
	int i = 0;
	int nb;
	for (i = 0; i < size; i++)
	{
		printi(i);
		nb = tab[i];
		printi(nb);
		printi(i);
		printf("\n");
	}
	return 0;
}
