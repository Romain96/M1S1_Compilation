//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 1 
// 1 1 
// 1 2 1 
// 1 3 3 1 
// 1 4 6 4 1 
// 1 5 10 10 5 1 
// 1 6 15 20 15 6 1 
// 1 7 21 35 35 21 7 1 

int main() 
{
	int i, j;
  	int pascal[8][8];

  	for (i = 0; i < 8; i++) {
    		for (j = 0; j <= i; j++) {
      			if (i == j || j == 0)
        			pascal[i][j] = 1;
      			else
        			pascal[i][j] = pascal[i-1][j] + pascal[i-1][j-1];
    		}
  	}

  	for (i = 0; i < 8; i++) {
    		for (j = 0; j <= i; j++) {
      			printi((pascal[i][j]));
      			printf(" ");
    		}
    		printf("\n");
  	}

	// modification : un return est requis par notre grammaire !
	return 0;
}
