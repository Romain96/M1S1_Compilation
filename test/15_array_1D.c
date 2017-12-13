//#include <stdio.h>
//#define printf(X) fprintf(stdout, "%s", (X))
//#define printi(X) fprintf(stdout, "%d", (X))

int main() 
{
	// edit : init est a[] ou a[] = {...}
	//int a[10];
	int a[] = {1,2,3,4,5,6,7,8,9,10};
 	int i;
  	a[0] = 1;

	// edit : remplacement de 10 par b = 10 et i = i + 1 par i++
	int b = 10;
	int indice;
	int temp;
  	for(i = 1; i < b; i++) {
		// edit : i-1 en indice ne fonctionnera pas mais indice = i - 1 et a[indice] oui
		indice = i - 1;
		// edit *2 non plus passage par une variable temporaire
		temp = a[indice];
		temp = temp * 2;    		
		a[i] = temp;
		temp = a[i];
    		printi(temp);
  	}
	// modification : un return est requis par notre grammaire !
	return 0;	
}
