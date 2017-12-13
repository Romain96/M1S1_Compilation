//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 42

// EDIT : test OK

int main() 
{
	int a;
  	int b;
  	int c;
  	int d;
  	a = 30;
  	b = 10;
  	c = a + b;
  	d = c + 2;
  	printi(d);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
