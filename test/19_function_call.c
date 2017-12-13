//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 120

int factorial(int n) 
{
	int accu = 1;

  	while (n != 0) {
    		accu = accu * n;
    		n = n - 1;
  	}

  	return accu;
}

int main() 
{
  	int fac;
  	fac = factorial(5);
  	printi(fac);
  	printf("\n");

	// modification : un return est requis par notre grammaire !
	return 0;
}
