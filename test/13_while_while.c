//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 0 40 41 42 1 40 41 42 

// EDIT : test OK

int main() 
{
	int a;
  	int b;
  	int c;
  	a = 0;
  	b = 40;
  	c = -2;
	// edit : pas d'entiers dans nos conditions (e = 2, f = 43)
	int d = 2;
	int e = 43;
  	while (a < d) {
    		printi(a);
    		printf(" ");
    		while (b < e) {
      			printi(b);
      			printf(" ");
      			b = b + 1;
    		}
    		a = a + 1;
    		b = 40;
  	}
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
