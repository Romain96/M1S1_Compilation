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
  	if (a == b) {
    		printf("NOT ");
  	}
  	printf("OK !!!");
	// modification : un return est requis par notre grammaire !
	return 0;
}
