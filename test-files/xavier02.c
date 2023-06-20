#include <stdio.h>

typedef enum
{
	KILOMETER = 1,
	MILES,
	LEAGUES
} Distance;

double kmConversions(double km, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
	case MILES:
		result = km * 0.621371;
		break;
	case LEAGUES:
		result = km * 0.207123;
	}

	return result;
}

double milesConversions(double miles, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
	case KILOMETER:
		result = miles * 1.60934;
		break;

	case LEAGUES:
		result = miles / 3.0;
		break;
	}

	return result;
}

double leagueConversions(double leagues, Distance typeOfResult)
{

	double result;

	switch (typeOfResult)
	{
	case KILOMETER:
		result = leagues * 4.82803;
		break;
	case MILES:
		result = leagues * 3.0;
		break;
	}

	return result;
}

int main()
{
	double valor;
	int opcao;

	printf("Escolha :\n");
	printf("1. Conversor quilômetros para milhas\n");
	printf("2. Conversor  milhas para quilômetros\n");
	printf("3. Conversor  quilômetros para léguas\n");
	printf("4. Conversor  léguas para quilômetros\n");
	scanf("%d", &opcao);

	printf("Digite o valor: ");
	scanf("%lf", &valor);

	switch (opcao)
	{
	case 1:
		printf("%.2f km = %.2f milhas\n", valor, converterKmParaMilhas(valor));
		break;
	case 2:
		printf("%.2f milhas = %.2f km\n", valor, converterMilhasParaKm(valor));
		break;
	case 3:
		printf("%.2f km = %.2f léguas\n", valor, converterKmParaLeguas(valor));
		break;
	case 4:
		printf("%.2f léguas = %.2f km\n", valor, converterLeguasParaKm(valor));
		break;
	default:
		printf("Opção inválida.\n");
	}

	return 0;
}