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
  	a = 40;
  	b = 10;
  	c = 2;
  	d = (a + b / 1 / c) - 3; 
  	printi(d);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
