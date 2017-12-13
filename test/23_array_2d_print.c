// test de parcours d'un tableau 2d
int main()
{
	int tab[] = {{1,2,3,4,5},{6,7,8,9,10}};
	int sizex = 2;
	int sizey = 5;
	int i;
	int j;
	int nb;
	for (i = 0; i < sizex; i++)
	{
		for (j = 0; j < sizey; j++)
		{
			printf("tab[");
			printi(i);
			printf(",");
			printi(j);
			printf("] = ");
			nb = tab[i][j];
			printi(nb);
			printf(" ");
		}
		printf("\n");
	}
	return 0;
}
