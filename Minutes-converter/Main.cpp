#include <stdio.h>
#include <stdlib.h>

int Converter(int minutes, int* heures);

int main()
{
	int heures = 0;
	int minutes = 0;

	printf("Entrez un chiffre\n");
	scanf_s("%d", &minutes);

	minutes = Converter(minutes, &heures);

	printf("\n\nCela fait %d heures et %d minutes.\n", heures, minutes);



	return 0;
}


int Converter(int minutes, int* heures)
{
	*heures = minutes / 60;
	minutes = minutes % 60;


	return minutes;
}
