//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// EDIT : test OK

int main() 
{
	int a;
  	int b;
  	a = 40;
  	b = 10;
	// edit : replacer a*b par c et 0 par d (par d'entier)
	int c = a*b;
	int d = 0;
  	if ((a == b) || (a < b) && (c > d)) {
    		printf("NOT ");
  	}
  	printf("OK !!!");
	// modification : un return est requis par notre grammaire !
	return 0;
}
