int main()
{
	int tableau_un[] = {1,2,3,4,5};
	int tableau_deux[] = {6,7,8,9,10};
	tableau_un[0] = tableau_deux[4];
	int nb = tableau_un[0];
	printi(nb);
	return 0;
}
