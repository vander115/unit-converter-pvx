#include <stdio.h>

float kgToArroba(float kg)
{
	return kg / 14.69; // Conversão aproximada de kg para arroba
}

float arrobaToKg(float arroba)
{
	return arroba * 14.69; // Conversão aproximada de arroba para kg
}

int main()
{
	float weight;
	int option;

	printf("Conversor de Peso\n");
	printf("-----------------\n");

	printf("Selecione a conversão:\n");
	printf("1. Quilograma para Arroba\n");
	printf("2. Arroba para Quilograma\n");
	scanf("%d", &option);

	switch (option)
	{
	case 1:
		printf("Digite o peso em quilogramas: ");
		scanf("%f", &weight);
		printf("%.2f kg equivale a %.2f arroba(s)\n", weight, kgToArroba(weight));
		break;
	case 2:
		printf("Digite o peso em arroba(s): ");
		scanf("%f", &weight);
		printf("%.2f arroba(s) equivale a %.2f kg\n", weight, arrobaToKg(weight));
		break;
	default:
		printf("Opção inválida.\n");
		break;
	}

	return 0;
}