%{
	#include <stdio.h>
%}

%%

int|stencil 				printf("TYPE ");
"+"|"-"|"*"|"/"|"++"|"--"|"$"|"=" 	printf("OPERATEUR ");
if|then|else|while|for 			printf("STRUCT_CONTROLE ");
printf|printi 				printf("AFFICHAGE ");
[a-z_][a-zA-Z0-9]* 			printf("ID ");
[0-9]+ 					printf("ENTIER ");
";" 					printf("PONCTUATION ");
"{"|"}"|"("|")" 			printf("DELIMITEUR ");
\n|\t|[ ]				printf("%s", yytext);

%%

int main()
{
	yylex();
	return 0;
}
