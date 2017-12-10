// manipulation of arrays
int main()
{
	int array[] = {3,1,4,1,5,9,2,6,5};
	int size = 9;
	int i;
	int nb;
	for (i = 0; i < size; i++)
	{
		printf("array[");
		printi(i);
		printf("] contains ");
		nb = array[i];
		printi(nb);	
		printf("\n");
	}
	return 0;
}
