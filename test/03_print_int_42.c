//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Output:
// 42

// EDIT : test OK

int main() 
{
	int val;
  	val = 42;
  	printi(val);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
