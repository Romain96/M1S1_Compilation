//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// -55
// 55

// EDIT : test OK

int main() 
{
	int a;
	int b;
 	int c;
  	int d;
  	int e;
  	a = 1;
  	b = 2;
  	c = -2;
  	d = 54;
  	e = 1;
  	while (e) {
    		c = c + 1;
    		b = b - 1;
		// edit : {} obligatoires dans notre grammaire
    		if (c > d)
		{
      			e = 0;
		}
  	}
  	printi(b);
  	printf("\n");
  	printi(c);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
