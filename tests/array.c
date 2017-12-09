// manipulation of arrays
int main()
{
	int array[] = {1,2,3,4,5,6,7,8,9,10};
	int value;
	value = array[4];
	printf("fifth value ofarray is ");
	printi(value);
	printf("\n");
	array[4] = 42;
	value = array[4];
	printf("now fifth value of array is ");
	printi(value);
	printf("\n");
	return 0;
}
