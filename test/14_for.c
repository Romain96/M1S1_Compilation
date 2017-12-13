//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

// Result:
// 42

// EDIT : test OK

int main() 
{
	int a;
  	int i;
  	a = 0;
	// edit remplacement de 21 par b = 21 et de i = i + 1 par i++
	int b = 21;
  	for (i = 0; i < b; i++) {
    		a = a + 3;
    		a = a - 1;
  	}
  	printi(a);
  	printf("\n");
	// modification : un return est requis par notre grammaire !
	return 0;
}
