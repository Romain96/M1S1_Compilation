int main()
{
	int pi[] = {3,1,4,1,5,9,2,6,5};
	int size = 9;
	int i = 0;
	printf("pi = ");
	int nb =pi[0];
	printi(nb);
	printf(",");
	for (i = 1; i < size; i++)
	{
		nb = pi[i];
		printi(nb);
	}
	printf("\n");
	return 0;
}
