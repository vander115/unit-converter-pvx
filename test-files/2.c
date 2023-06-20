#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	KILOMETER_PER_HOUR = 1,
	METER_PER_SECOND
} Speed;

float speedConversion(float initialVelocity, Speed typeOfConversion)
{
	float finalVelocity;

	if (typeOfConversion = KILOMETER_PER_HOUR)
	{ // converte para Km/h
		finalVelocity = initialVelocity * 3.6;
	}

	if (typeOfConversion = METER_PER_SECOND)
	{ // converte para m/s
		finalVelocity = initialVelocity / 3.6;
	}
	return finalVelocity;
}

int main()
{
	float initialVelocity;
	int option;
	do
	{
		printf("Digite a unidade de medida que deseja converter:\n [1] m/s -> Km/h \n [2] Km/s -> m/s\n ");
		scanf("%d", &option);
	} while (option != 1 && option != 2);
	printf("Digite a velocidade: ");
	scanf("%f", &initialVelocity);
	if (option == 1)
	{
		printf("A velocidade convertida eh: %.2f Km/h", speedConversion(initialVelocity, KILOMETER_PER_HOUR));
	}
	if (option == 2)
	{
		printf("A velocidade convertida eh: %.2f m/s", speedConversion(initialVelocity, METER_PER_SECOND));
	}
}
