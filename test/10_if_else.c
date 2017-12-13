//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// -3

// EDIT : test OK

int main() 
{
	int a;
  	int b;
  	int c;
  	a = -15;
  	b = 5;
  	c = -2 * a;
  	if ((a == b) || (a > b)) {
    		b = b - 5;
    		c = b - a;
    		c = c - 2;
  	} else {
    		b = b + 5;
    		c = b + a;
    		c = c + 2; 
  	}
  	printi(c);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
