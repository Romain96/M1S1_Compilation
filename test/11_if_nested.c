//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 42

// EDIT : test OK

int main() 
{
	int big;
  	int medium;
  	int little;
  	big = 100;
  	medium = 10;
  	little = 1;

  	if (big > medium) {
    		if (medium < little) {
     			little = 2;
    		} else {
      			little = 42;
    		}
  	} else {
    		little = 3;
  	}

  	printi(little);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
