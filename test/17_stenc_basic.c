//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 28
// 4

int main() 
{
	int tab[7], i, somme, moyenne;
	stencil s{3,1} = { 1, 1, 1, 1, 1, 1, 1 };

  	for (i = 0; i < 7; i++)
    		tab[i] = i;

  	somme = tab[3] $ s;
  	printi(somme);
  	printf("\n");

  	moyenne = (tab[3] $ s) / 7;
  	printi(moyenne);
  	printf("\n");
}
