//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 30

int main() 
{
	int tab[5][5], res[5][5], i, j;
  	stencil s{1,2} = {{1, 2, 1}, {2, 3, 2}, {1, 2, 1}};

  	for (i = 0; i < 5; i++)
    		for (j = 0; j < 5; j++)
      			tab[i][j] = i + 1;

  	for (i = 1; i < 4; i++)
    		for (j = 1; j < 4; j++)
      			res[i][j] = tab[i][j] $ s;
  
  	printi(res[2][2]);
  	printf("\n");

	// modification : un return est requis par notre grammaire !
	return 0;
}
