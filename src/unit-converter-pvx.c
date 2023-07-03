#include <stdio.h>	// Biblioteca para trabalhar com fun��es de entrada e sa�da
#include <stdlib.h> // Biblioteca para trabalhar com fun��es de sistema
#include <locale.h> // Biblioteca para trabalhar com acentua��o
#include <stdarg.h> // Biblioteca para trabalhar com fun��es que recebem um n�mero vari�vel de argumentos
#include <string.h> // Biblioteca para trabalhar com fun��es manipula��o de strings
#include <ctype.h>	// Biblioteca para trabalhar com fun��es de caracteres
#include <stdbool.h> // Biblioteca para trabalhar com fun��es booleanas

/* 
	-- CONVERSOR DE UNIDADES PVX --
	
	Desenvolvido por:
	Guilherme Meneses Xavier Abreu Cardoso
	Jos� Vanderlei Furtuna Tom�
	P�mela Maria Pontes Frota 
	
	OBS: Para facilitar a clareza do c�digo por outros desenvolvedores, as v�riveis e fun��es foram nomeadas
	com nomes explicativos e na linguagem universal: ingl�s, esse � um padr�o utilizado pela comunidade de 
	desenvolvedores atuais.
*/

// SE��O: DEFINI��O DE TIPOS

// Tipos de convers�o
typedef enum
{
	TEMPERATURE = 1,
	DISTANCE,
	MASS,
	TIME,
	FORCE,
	VELOCITY,
} Conversions; // Tipos de convers�o

// Tipos de unidades de temperatura
typedef enum
{
	CELSIUS = 1,
	FAHRENHEIT,
	KELVIN
} Degrees;

// Tipos de unidades de dist�ncia
typedef enum
{
	KILOMETER = 1,
	MILES,
	LEAGUES
} Distance;

typedef enum
{
	KILOGRAM = 1,
	ARROBA
} Mass;

// Tipos de unidades de tempo
typedef enum
{
	SECONDS = 1,
	MINUTES,
	HOURS,
	DAYS
} Time;

// Tipo de unidades de for�a
typedef enum
{
	KILOGRAM_FORCE = 1,
	NEWTON
} Force;

// Tipos de unidades de velocidade
typedef enum
{
	KILOMETER_PER_HOUR = 1,
	METER_PER_SECOND
} Velocity;

// SE��O: FERRAMENTAS

// Verifica se o valor digitado � um n�mero ou n�o
bool isValidNumber(const char *input) {
	
	int i, points = 0;
	// Verifica se cada caractere � um d�gito, ponto ou v�rgula
    for (i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i]) && input[i] != '.' && input[i] != ',') {
            return false;
        }
    }
    
    // Verifica se h� apenas um ponto ou v�rgula
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '.' || input[i] == ',') {
            points++;
        }
    }
    
    // Se existir mais de uma virgula ou ponto, o n�mero n�o � valido
    if (points > 1) {
    	return false;
	}
    
    return true;
}

// Recebe um n�mero inteiro digitado pelo usu�rio e valida se realmente � um n�mero ou n�o
int getOption()
{
	char input[100];
	int option;

	do
	{
		printf("Selecione uma op��o: ");
		setbuf(stdin, NULL); // Limpa o buffer de entrada do programa
		
		fgets(input, sizeof(input), stdin); // Captura uma string digitada pelo usu�rio

		if (sscanf(input, " %d", &option) != 1) // Verifica se os caracteres lidos formam um n�mero inteiro
		{
			printf("Entrada inv�lida. Digite um n�mero. \n");
		}

	} while (sscanf(input, "%d", &option) != 1);

	return option;
}

// Recebe um n�mero decimal digitado pelo usu�rio
float getValue()
{
	int i;
	float value;
	char input[40];
	char *endOfString; // Ponteiro para o final da string

	do
	{
		fgets(input, sizeof(input), stdin); // L� a string digitada pelo usu�rio
		
		// Remove a quebra de linha no final do input
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        
        // Verifica se � um n�mero decimal v�lido
        if (isValidNumber(input) || input[0] != '\0') {
            // Substitui a v�rgula por ponto
            for (i = 0; i < strlen(input); i++) {
                if (input[i] == '.') {
                    input[i] = ',';
                    break;
                }
            }
            
            value = strtof(input, &endOfString);
        } else {
            printf("Entrada inv�lida. Digite um n�mero decimal: ");
        }

	} while (!isValidNumber(input) || input[0] == '\0');

	return value;
}

// Menu de encerramento das convers�es
void closingMenu()
{
	int option;
	do
	{
		printf("\n\nO que voc� quer fazer agora?\n1 - Voltar ao menu principal\n2 - Sair\n\n");
		option = getOption();

		switch (option)
		{
		case 1:
			system("CLS");
			return;
		case 2:
			exit(0); // Encerra o programa
		}

		if (option < 1 || option > 2)
		{
			printf("\nDigite uma op��o v�lida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
}

// SE��O: FUN��ES DE MANIPULA��O DE ARQUIVOS DO HIST�RICO

// Adiciona um valor ao arquivo de hist�rico
void addValueToHistory(const char *format, ...)
{
	va_list arguments; // cria uma variavel que vai receber a lista de argumentos da fun��o
	va_start(arguments, format); // Analisa o formato da string inserida a partir dos argumentos

	FILE *arquivo = fopen("history.txt", "a+"); // Abre o arquivo history no modo "append" (adicionar)
	
	// Valida��o da conex�o com o arquivo history
	if (arquivo == NULL)
	{
		printf("Erro ao abrir o arquivo de sa�da.\n");
		va_end(arguments);
		return;
	}

	vfprintf(arquivo, format, arguments); 	// Escreve a string formatada a partir dos argumentos no arquivo history
	fprintf(arquivo, "\n");					// Adiciona uma quebra de linha no final

	fclose(arquivo);	 // Fecha o arquivo history
	va_end(arguments); // Finaliza a lista de argumentos da fun��o
}

// Leitura do arquivo de hist�rico e exibi��o dos dados na tela
void readHistory()
{
	FILE *file;
	file = fopen("history.txt", "r"); // Abre o arquivo history no modo "read" (leitura)

	// Valida��o da conex�o com o arquivo history
	if (file == NULL)
	{
		printf("Hist�rico ainda n�o foi criado. Fa�a a sua primeira convers�o ou execute o programa como administrador.\n");
		return;
	}

	fseek(file, 0, SEEK_END); // Move o cursor para o fim do arquivo history
	long size = ftell(file);	// Determina o tamanho do arquivo history

	if (size == 0)
	{
		printf("N�o h� valores no hist�rico.\n"); // Exibe uma mensagem caso o arquivo esteja vazio
		fclose(file);
		return;
	}

	rewind(file); // Move o cursor de volta para o in�cio do arquivo history 

	char row[100]; // Buffer para armazenar cada linha do arquivo history

	while (fgets(row, sizeof(row), file))
	{
		printf("%s", row); // Exibe cada linha do arquivo history na tela
	}

	fclose(file); // Fecha o arquivo history
}

// Apaga o conte�do do arquivo de hist�rico
void cleanHistory()
{
	FILE *file;
	file = fopen("history.txt", "w"); // Abre o arquivo no modo "write" (escrever)

	// Verifica se o arquivo existe ou j� est� vazio
	if (file == NULL)
	{
		printf("O hist�rico j� est� vazio.\n");
		return;
	}

	fclose(file); // Fecha o arquivo
}

// SE��O: CONVERS�O DE UNIDADES DE TEMPERATURA

// Convers�o de graus celsius para fahrenheit e kelvin
float celsiusConversions(float initialDegrees, Degrees typeOfResult)
{
	float finalDegrees;

	switch (typeOfResult)
	{
		// Convers�o de celsius para fahrenheit
		case FAHRENHEIT:
			finalDegrees = (initialDegrees / 5.0) * 9 + 32; // F = (C / 5) * 9 + 32
			break;
			
		// Convers�o de celsius para kelvin
		case KELVIN:
			finalDegrees = initialDegrees + 273.15; // K = C + 273.15
			break;
	
		default:
			return initialDegrees;
	}

	return finalDegrees;
}

// Convers�o de graus fahrenheit para celsius e kelvin
float fahreheitConversions(float initialDegrees, Degrees typeOfReturn)
{
	float finalDegrees;
	switch (typeOfReturn)
	{
		// Convers�o de Fahreheit para Celsius
		case CELSIUS:
			finalDegrees = ((initialDegrees - 32) / 9) * 5;
			break;
			
		// Convers�o de Fahreheit para Kelvin
		case KELVIN:
			finalDegrees = ((initialDegrees - 32) / 9) * 5 + 273.15;
			break;
				
		default:
			finalDegrees = initialDegrees;
			break;
	}
	
	return finalDegrees;
}

// Convers�o de graus kelvin para celsius e fahrenheit
float kelvinConversions(float initialDegrees, Degrees typeOfResult)
{
	float finalDegrees;
	
	switch (typeOfResult)
	{
		// Convers�o de kelvin para celsius
		case CELSIUS:
			finalDegrees = initialDegrees - 273.15; // K = C + 273.15
			break;
			
		// Convers�o de kelvin para fahrenheit
		case FAHRENHEIT:
			finalDegrees = ((initialDegrees - 273.15) / 5) * 9 + 32; // F = (C / 5) * 9 + 32
			break;
			
		default:
			finalDegrees = initialDegrees;
			break;
	}
	
	return finalDegrees;
}

// SE��O: CONVERS�O DE UNIDADES DE DIST�NCIA

// Convers�o de quilometros para milhas e leguas
double kmConversions(double km, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
		// Convers�o de quil�metros para milhas
		case MILES:
			result = km * 0.621371; // 1 km = 0.621371 milhas
			break;
			
		// Convers�o de quil�metros para l�guas
		case LEAGUES:
			result = km * 0.207123; // 1 km = 0.207123 l�guas
			break;
			
		default:
			result = km;
			break;
	}

	return result;
}

// Convers�o de milhas para quil�metros e l�guas
double milesConversions(double miles, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
		// Convers�o de milhas para quil�metros
		case KILOMETER:
			result = miles * 1.60934; // 1 milha = 1.60934 km
			break;
			
		// Convers�o de milhas para l�guas
		case LEAGUES:
			result = miles / 3.0; // 1 milha = 3 l�guas
			break;
			
		default:
			result = miles;
			break;
	}

	return result;
}

// Convers�o de l�guas para quil�metros e milhas
float leaguesConversions(float leagues, Distance typeOfResult)
{

	float result;

	switch (typeOfResult)
	{
		// Convers�o de l�guas para quil�metros
		case KILOMETER:
			result = leagues * 4.82803; // 1 l�gua = 4.82803 km
			break;
			
		// Convers�o de l�guas para milhas
		case MILES:
			result = leagues * 3.0; // 1 l�gua = 3 milhas
			break;
			
		default:
			result = leagues;
			break;
	}

	return result;
}

// SE��O: CONVERS�O DE UNIDADES DE MASSA

// Convers�o de quilogramas para arrobas
float massConversions(float initialMass, Mass typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Convers�o de quilogramas para arrobas
		case KILOGRAM:
			result = initialMass * 14.688; // 1kg ~ 0.068 arrobas
			break;
	
		// Convers�o de arrobas para quilogramas
		case ARROBA:
			result = initialMass / 14.688; // 1 arroba ~ 14.688kg
			break;
			
		default:
			result = initialMass;
			break;
		}
		
		

	return result;
}

// SE��O: CONVERS�O DE UNIDADES DE TEMPO

// Convers�o de segundos para minutos, horas e dias
float secondsConversions(int seconds, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Convers�o de segundos para minutos
		case MINUTES:
			result = seconds / 60.0; // 1 minuto = 60 segundos
			break;
			
		// Convers�o de segundos para horas
		case HOURS:
			result = seconds / 3600.0; // 1 hora = 3600 segundos
			break;
		
		// Convers�o de segundos para dias
		case DAYS:
			result = (seconds / 3600.0) / 24; // 1 dia = 86400 segundos
			break;
			
		default:
			result = seconds;
			break;
	}

	return result;
}

// Convers�o de minutos para segundos, horas e dias
float minutesConversions(int minutes, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Convers�o de minutos para segundos
		case SECONDS:
			result = minutes * 60.0; // 1 minuto = 60 segundos
			break;
			
		// Convers�o de minutos para horas
		case HOURS:
			result = minutes / 60.0; // 1 hora = 60 minutos
			break;
			
		// Convers�o de minutos para dias
		case DAYS:
			result = (minutes / 60.0) / 24; // 1 dia = 24 horas
			break;
			
		default:
			result = minutes;
			break;
	}

	return result;
}

// Convers�o de horas para segundos, minutos e dias
float hoursConversions(int hours, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Convers�o de horas para segundos
		case SECONDS:
			result = hours * 3600.0; // 1 hora = 3600 segundos
			break;
			
		// Convers�o de horas para minutos
		case MINUTES:
			result = hours * 60.0; // 1 hora = 60 minutos
			break;
			
		// Convers�o de horas para dias
		case DAYS:
			result = hours / 24; // 1 dia = 24 horas
			break;
			
		default:
			result = hours * 3600.0; // 1 hora = 3600 segundos
			break;
	}

	return result;
}

// Convers�o de dias para segundos, minutos e horas
float daysConversions(int days, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Convers�o de dias para segundos
		case SECONDS:
			result = days * 86400.0; // 1 dia = 86400 segundos
			break;
			
		// Convers�o de dias para minutos
		case MINUTES:
			result = days * 1440.0; // 1 dia = 1440 minutos
			break;
			
		// Convers�o de dias para horas
		case HOURS:
			result = days * 24; // 1 dia = 24 horas
			break;
			
		default:
			result = days * 86400.0; // 1 dia = 86400 segundos
			break;
	}

	return result;
}

// SE��O: CONVERS�O DE UNIDADES DE FOR�A

float forceConversions(float initialForce, Force typeOfResult)
{

	float result;

	switch (typeOfResult)
	{
		// Convers�o de quilograma-for�a para newton
		case NEWTON:
			result = initialForce * 9.80665; // 1 kgf = 9.80665 N
			break;
	
		// Convers�o de newton para quilograma-for�a
		case KILOGRAM_FORCE:
			result = initialForce / 9.80665; // 1 N = 0.101972 kgf
			break;
			
		default:
			result = initialForce;
			break;
	}

	return result;
}

// SE��O: CONVERS�O DE UNIDADES DE VELOCIDADE

float velocityConversion(float initialVelocity, Velocity typeOfResult)
{
	float finalVelocity;

	switch (typeOfResult)
	{
	// Convers�oo de quil�metros por hora para metros por segundo
		case KILOMETER_PER_HOUR:
			finalVelocity = initialVelocity * 3.6;
			break;
	
		// Convers�o de metros por segundo para quil�metros por hora
		case METER_PER_SECOND:
			finalVelocity = initialVelocity / 3.6;
			break;
			
		default:
			finalVelocity = initialVelocity;
			break;
	}

	return finalVelocity;
}

// SE��O: MENUS DE CONVERS�O

// Menu principal
void menu(int *option)
{
	printf("1 - Temperatura\n2 - Dist�ncia\n3 - Massa\n4 - Tempo\n5 - For�a\n6 - Velocidade\n\n7 - Hist�rico   8 - Sair\n\n");
	*option = getOption();
}

// Menu de convers�o de unidades de temperatura
void temperatureMenu()
{
	float initialDegrees;
	int option, i = 0;
	char initialUnit, finalUnit;

	printf("-- UNIDADES DE TEMPERATURA -- \n\n");

	do
	{

		printf("Escreva a temperatura que deseja converter (Ex: 25C. Unidades poss�veis: C, F e K):\n");
		scanf("%f%c", &initialDegrees, &initialUnit);

		initialUnit = toupper(initialUnit);

		switch (initialUnit)
		{
			case 'C':
				do
				{
					printf("Para Kelvin ou Fahrenheit? (K/F): ");
					setbuf(stdin, NULL);
					scanf(" %c", &finalUnit);
	
					if ((finalUnit == 'K') || (finalUnit == 'k'))
					{
						printf("\033[1;32m%.3f graus Celsius � igual a %.3f graus Kelvin\033[0m\n", initialDegrees, celsiusConversions(initialDegrees, KELVIN));
						addValueToHistory("%.3f graus Celsius � igual a %.3f graus Kelvin", initialDegrees, celsiusConversions(initialDegrees, KELVIN));
						i++;
					}
					else if ((finalUnit == 'F') || (finalUnit == 'f'))
					{
						printf("\033[1;32m%.3f graus Celsius � igual a %.3f graus Fahrenheit\033[0m\n", initialDegrees, celsiusConversions(initialDegrees, FAHRENHEIT));
						addValueToHistory("%.3f graus Celsius � igual a %.3f graus Fahrenheit", initialDegrees, celsiusConversions(initialDegrees, FAHRENHEIT));
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura n�o identificada!!!\n");
					}
	
				} while (i == 0);
				break;
	
			case 'F':
				do
				{
					printf("Para Celcius ou Kelvin? (C/K): ");
					setbuf(stdin, NULL);			// Limpa o buffer do teclado
					scanf(" %c", &finalUnit); // Lê a unidade de saida
	
					// Valida��o da unidade de saida e convers�o de graus fahrenheit para celsius e kelvin
					if ((finalUnit == 'K') || (finalUnit == 'k'))
					{
						printf("\033[1;32m%.3f graus Fahrenheit � igual a %.3f graus Kelvin\033[0m\n", initialDegrees, fahreheitConversions(initialDegrees, KELVIN));
						addValueToHistory("%.3f graus Fahrenheit � igual a %.3f graus Kelvin", initialDegrees, fahreheitConversions(initialDegrees, KELVIN));
						i++;
					}
					else if ((finalUnit == 'C') || (finalUnit == 'c'))
					{
						printf("\033[1;32m%.3f graus Fahrenheit � igual a %.3f graus Celsius\033[0m\n", initialDegrees, fahreheitConversions(initialDegrees, CELSIUS));
						addValueToHistory("%.3f graus Fahrenheit � igual a %.3f graus Celsius", initialDegrees, fahreheitConversions(initialDegrees, CELSIUS));
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura n�o identificada! Tente novamente.\n");
					}
	
				} while (i == 0);
				break;
	
			case 'K':
				do
				{
					printf("Para Celcius ou Fahrenheit? (C/F): ");
					setbuf(stdin, NULL);			// Limpa o buffer do teclado
					scanf(" %c", &finalUnit); // L� a unidade de sa�da
	
					// Valida��o da unidade de sa�da e convers�o de graus kelvin para celsius e fahrenheit
					if ((finalUnit == 'F') || (finalUnit == 'f'))
					{
						printf("\033[1;32m%.3f graus Kelvin � igual a %.3f graus Fahrenheit\033[0m\n", initialDegrees, kelvinConversions(initialDegrees, FAHRENHEIT)); // Chama a função de conversão
						addValueToHistory("%.3f graus Kelvin � igual a %.3f graus Fahrenheit", initialDegrees, kelvinConversions(initialDegrees, FAHRENHEIT));				 // Adiciona o resultado ao arquivo de histórico
						i++;
					}
					else if ((finalUnit == 'C') || (finalUnit == 'c'))
					{
						printf("\033[1;32m%.3f graus Kelvin � igual a %.3f graus Celsius\033[0m\n", initialDegrees, kelvinConversions(initialDegrees, CELSIUS)); // Chama a função de conversão
						addValueToHistory("%.3f graus Kelvin � igual a %.3f graus Celsius", initialDegrees, kelvinConversions(initialDegrees, CELSIUS));				 // Adiciona o resultado ao arquivo de histórico
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura n�o identificada! Tente novamente\n");
					}
	
				} while (i == 0);
				break;
		}

		if (initialUnit != 'C' && initialUnit != 'F' && initialUnit != 'K')
		{
			printf("Unidade de medida de temperatura n�o identificada! Tente novamente\n ");
		}

	} while (initialUnit != 'C' && initialUnit != 'F' && initialUnit != 'K');

	closingMenu();
}

// Menu de convers�o de unidades de dist�ncia
void distanceMenu()
{
	int initialUnit, finalUnit;
	float value, result;

	do
	{
		printf("-- UNIDADES DE DIST�NCIA --\n\n");
		printf("Selecione a unidade de dist�ncia que voc� quer converter: \n\n1 - Quil�metro(km)\n2 - Milha(s)\n3 - L�gua(s)\n\n");
		initialUnit = getOption();

		// Valida��o da unidade de entrada
		if (initialUnit < 1 || initialUnit > 3)
		{
			system("CLS");
			printf("Digite uma op��o v�lida! Tente novamente: \n");
		}

	} while (initialUnit < 1 || initialUnit > 3);

	// Limpa a tela e exibe a unidade de entrada
	system("CLS");
	printf("-- UNIDADES DE DIST�NCIA --\n\n");
	
	// Imprime a unidade de medida selecionada
	printf("Unidade selecionada: ");
	switch (initialUnit)
	{
		case KILOMETER:
			printf("Quil�metro(km)\n");
			break;
		case MILES:
			printf("Milhas\n");
			break;
		case LEAGUES:
			printf("L�guas\n");
			break;
	}

	printf("\nDigite o valor que voc� quer converter: ");
	value = getValue();

	printf("\nSelecione a unidade de sa�da: \n");

	do
	{
		// Exibe apenas as unidades que n�o foram selecionadas como entrada
		if (initialUnit != KILOMETER)
			printf("1 - Quil�metro(km)\n");
		if (initialUnit != MILES)
			printf("2 - Milha(s)\n");
		if (initialUnit != LEAGUES)
			printf("3 - L�gua(l�guas)\n");
		printf("Op��o: ");
		scanf("%d", &finalUnit);

		// Valida��o da unidade de sa�da
		if (finalUnit <= 0 || finalUnit == initialUnit || finalUnit >= 4)
		{
			printf("Digite uma op��o v�lida! Tente novamente: \n");
		}

	} while (finalUnit <= 0 || finalUnit == initialUnit || finalUnit >= 4);

	// Convers�o de unidades
	switch (initialUnit)
	{
	case KILOMETER:
		switch (finalUnit)
		{
		case MILES:
			result = kmConversions(value, MILES);														// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f quil�metro(s) � igual a %.3f milha(s)\033[0m\n", value, result); 	// Exibe o resultado
			addValueToHistory("%.3f quil�metro(s) � igual a %.3f milha(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
			break;

		case LEAGUES:
			result = kmConversions(value, LEAGUES);													// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f quil�metro(s) � igual a %.3f l�gua(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f quil�metro(s) � igual a %.3f l�gua(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
			break;
		}
		break;

	case MILES:
		switch (finalUnit)
		{
			case KILOMETER:
				result = milesConversions(value, KILOMETER);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f milha(s) � igual a %.3f quil�metro(s)\033[0m\n", value, result); // Exibe o resultado
				addValueToHistory("%.3f milha(s) � igual a %.3f quil�metro(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case LEAGUES:
				result = milesConversions(value, LEAGUES);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f milha(s) � igual a %.3f l�gua(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f milha(s) � igual a %.3f l�gua(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;

	case LEAGUES:
		switch (finalUnit)
		{
			case KILOMETER:
				result = leaguesConversions(value, KILOMETER);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f l�gua(s) � igual a %.3f quil�metro(s)\033[0m\n", value, result); // Exibe o resultado
				addValueToHistory("%.3f l�gua(s) � igual a %.3f quil�metro(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case MILES:
				result = leaguesConversions(value, MILES);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f l�gua(s) � igual a %.3f milha(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f l�gua(s) � igual a %.3f milha(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;
	}

	closingMenu();
}

// Menu de convers�o de unidades de tempo
void timeMenu()
{
	int inputUnit, outputUnit;
	float value, result;
	do
	{
		printf("-- UNIDADES DE TEMPO --\n\n");
		printf("Selecione a unidade de tempo que voc� quer converter: \n\n1 - Segundos\n2 - Minutos\n3 - Horas\n4 - Dias\n\n");
		inputUnit = getOption();

		if (inputUnit < 1 || inputUnit > 4)
		{
			system("CLS");
			printf("Digite uma op��o v�lida! Tente novamente: \n\n");
		}
	} while (inputUnit < 1 || inputUnit > 4);

	// Limpa a tela e exibe a unidade de entrada
	system("CLS");
	printf("-- UNIDADES DE TEMPO --\n\n");
	// Imprime a unidade selecionada na tela
	printf("Unidade de entrada: ");
	switch (inputUnit)
	{
		case SECONDS:
			printf("Segundos\n");
			break;
		case MINUTES:
			printf("Minutos\n");
			break;
		case HOURS:
			printf("Horas\n");
			break;
		case DAYS:
			printf("Dias\n");
			break;
	}

	printf("Digite o valor que voc� quer converter: ");
	value = getValue();

	printf("\nSelecione a unidade de sa�da: \n\n");
	do
	{
		if (inputUnit != SECONDS)
			printf("1 - Segundos\n");
		if (inputUnit != MINUTES)
			printf("2 - Minutos\n");
		if (inputUnit != HOURS)
			printf("3 - Horas\n");
		if (inputUnit != DAYS)
			printf("4 - Dias\n");
		
		outputUnit = getOption();

		if (outputUnit <= 0 || outputUnit == inputUnit || outputUnit >= 5)
		{
			printf("Digite uma op��o valida! Tente novamente: \n");
		}

	} while (outputUnit <= 0 || outputUnit == inputUnit || outputUnit >= 5);

	switch (inputUnit)
	{
	case SECONDS:
		switch (outputUnit)
		{
			case MINUTES:
				result = secondsConversions(value, MINUTES);											// Chama a fun��o de convers�o
				printf("\033[0;32m%.3f segundo(s) � igual a %.3f minuto(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) � igual a %.3f minuto(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case HOURS:
				result = secondsConversions(value, HOURS);												// Chama a fun��o de convers�o
				printf("\033[0;32m%.3f segundo(s) � igual a %.4f hora(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) � igual a %.3f hora(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case DAYS:
				result = secondsConversions(value, DAYS);											// Chama a fun��o de convers�o
				printf("\033[0;32m%.3f segundo(s) � igual a %f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) � igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;

	case MINUTES:
		switch (outputUnit)
		{
			case SECONDS:
				result = minutesConversions(value, SECONDS);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f minuto(s) � igual a %.3f segundo(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) � igual a %.3f segundo(s)", value, result);			// Adiciona o resultado ao arquivo dehist�rico
				break;
	
			case HOURS:
				result = minutesConversions(value, HOURS);											// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f minuto(s) � igual a %.3f hora(s)\033[0m\n", value, result);	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) � igual a %.3f hora(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case DAYS:
				result = minutesConversions(value, DAYS);											 // Chama a fun��o de convers�o
				printf("\033[1;32m%.3f minuto(s) � igual a %.3f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) � igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;

	case HOURS:
		switch (outputUnit)
		{
			case SECONDS:
				result = hoursConversions(value, SECONDS);												// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f hora(s) � igual a %.3f segundo(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) � igual a %.3f segundo(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case MINUTES:
				result = hoursConversions(value, MINUTES);												// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f hora(s) � igual a %.3f minutos(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) � igual a %.3f minutos(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case DAYS:
				result = hoursConversions(value, DAYS);												// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f hora(s) � igual a %.3f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) � igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;

	case DAYS:
		switch (outputUnit)
		{
			case SECONDS:
				result = daysConversions(value, SECONDS);												// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f dia(s) � igual a %.3f segundo(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) � igual a %.3f segundo(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case MINUTES:
				result = daysConversions(value, MINUTES);												// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f dia(s) � igual a %.3f minutos(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) � igual a %.3f minutos(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
	
			case HOURS:
				result = daysConversions(value, HOURS);													// Chama a fun��o de convers�o
				printf("\033[1;32m%.3f dia(s) � igual a %.3f horas(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) � igual a %.3f horas(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
				break;
		}
		break;
	}

	closingMenu();
}

// Menu de convers�o de unidades de for�a
void forceMenu()
{
	int option;
	float result, value;

	do
	{
		printf("-- UNIDADES DE FOR�A --\n\n");
		printf("Selecione a convers�o que voc� deseja fazer: \n\n1 - Kilograma-for�a(kgf) para Newton(N)\n2 - Newton(N) para Kilograma-for�a(kgf)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma op��o valida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	switch(option) {
		case KILOGRAM_FORCE:
			printf("Unidade de Entrada: kgf\nDigite o valor que voc� deseja converter para Newton: ");
			break;
			
		case NEWTON:
			printf("Unidade de Entrada: Newton\nDigite o valor que voc� deseja converter para kgf: ");	
			break;
			
	}
	value = getValue();

	switch (option)
	{
		// Convers�o de quilogramas para arrobas
		case KILOGRAM_FORCE:
			result = forceConversions(value, NEWTON);													// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f kilograma-for�a(s) � igual a %f newton(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f kilograma-for�a(s) � igual a %f newton(s)", value, result); 		
			break;
	
		// Convers�o de arrobas para quilogramas
		case NEWTON:
			result = forceConversions(value, KILOGRAM_FORCE);											// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f newton(s) � igual a %f kilograma-for�a(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f newton(s) � igual a %f kilograma-for�a(s)", value, result);			// Adiciona o resultado ao arquivo de hist�rico
			break;
	}

	closingMenu();
}

//  Menu de convers��o de unidades de massa
void massMenu()
{
	int option;
	float value, result;

	do
	{
		printf("-- UNIDADES DE MASSA --\n\n");
		printf("Selecione a convers�o que voc� deseja fazer: \n\n1 - Kilograma(kg) para Arroba(arroba)\n2 - Arroba(arroba) para Kilograma(kg)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma op��o v�lida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);

	switch(option) {
		case KILOGRAM:
			printf("Unidade de Entrada: Kg\nDigite o valor que voc� deseja converter para Arroba: ");
			break;

		case ARROBA:
			printf("Unidade de Entrada: Arroba\nDigite o valor que voc� deseja converter para Kg: ");
			break;
	}
	value = getValue();

	switch (option)
	{
		// Convers�o de quilogramas para arrobas
		case KILOGRAM:
			result = massConversions(value, ARROBA);													// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f kilograma(s) � igual a %.3f arroba(s)\033[0m\n", value, result); 	// Exibe o resultado
			addValueToHistory("%.3f kilograma(s) � igual a %.3f arroba(s)", value, result); 			// Adiciona o resultado ao arquivo de hist�rico
			break;
	
		// Convers�o de arrobas para quilogramas
		case ARROBA:
			result = massConversions(value, KILOGRAM);													// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f arroba(s) � igual a %.3f kilograma(s)\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f arroba(s) � igual a %.3f kilograma(s)", value, result);				// Adiciona o resultado ao arquivo de hist�rico
			break;
	}

	closingMenu(); // Chama o menu de fechamento
}

// Menu de convers�o de unidades de velocidade
void velocityMenu()
{

	int option;
	float value, result;

	do
	{
		printf("-- UNIDADES DE VELOCIDADE --\n\n");
		printf("Selecione a convers�o que voc� deseja fazer: \n\n1 - Kilometro por hora(km/h) para Metro por segundo(m/s)\n2 - Metro por segundo(m/s) para Kilometro por hora(km/h)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma op��o v�lida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	

	switch (option) {
		case KILOMETER_PER_HOUR:
			printf("Unidade de Entrada: Km/h\nDigite o valor que voc� deseja converter para m/s: ");
			break;
		
		case METER_PER_SECOND:
			printf("Unidade de Entrada: m/s\nDigite o valor que voc� deseja converter para Km/s: ");
			break;
	}
	value = getValue();

	switch (option)
	{
		case KILOMETER_PER_HOUR:
			result = velocityConversion(value, METER_PER_SECOND);															// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f kilometro(s) por hora � igual a %.3f metro(s) por segundo\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f kilometro(s) por hora � igual a %.3f metro(s) por segundo", value, result);				// Adiciona o resultado ao arquivo de hist�rico
			break;
	
		case METER_PER_SECOND:
			result = velocityConversion(value, KILOMETER_PER_HOUR);															// Chama a fun��o de convers�o
			printf("\033[1;32m%.3f metro(s) por segundo � igual a %.3f kilometro(s) por hora\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f metro(s) por segundo � igual a %.3f kilometro(s) por hora", value, result);				// Adiciona o resultado ao arquivo de hist�rico
			break;
	}

	closingMenu();
}

// Menu do hist�rico de convers�es
void historyMenu()
{
	int option;
	printf(" -- HIST�RICO DE CONVERS�ES --\n\n");
	readHistory();
	do
	{
		printf("\n\n1 - Limpar hist�rico\n2 - Voltar\n");
		option = getOption();
		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma op��o v�lida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	
	switch (option)
	{
		case 1:
			system("CLS");
			cleanHistory();
			printf("Hist�rico limpo com sucesso!\n\n");
			break;
		case 2:
			system("CLS");
			break;
	}
	
	return;
}

int main()
{
	setlocale(LC_ALL, "Portuguese"); // Define a linguagem do programa para portugu�s

	int option;

	do
	{
		// Exibe o cabe�alho do programa usando c�digos de escape de cores ANSI
		do
		{
		printf(" -- CONVERSOR DE UNIDADES \x1b[30;45mP\x1b[0m\x1b[30;41mV\x1b[0m\x1b[30;48;5;202mX\x1b[0m --\n\n"); // -- CONVERS�O DE UNIDADE PVX --

		printf("Bem vindo(a) ao sistema de convers�o de unidades\n");
		printf("Selecione os tipos de unidades que voc� deseja converter: \n");

			menu(&option); // Exibe o menu principal
			system("CLS"); // Limpa a tela

			if (option < 1 || option > 8) // Valida��o da op��o selecionada para imprimir uma mensagem de erro
			{
				system("CLS");
				printf("Digite uma op��o v�lida e tente novamente: \n");
			}
		} while (option < 1 || option > 8); // Valida��o da op��o selecionada

		// Verifica qual op��o foi selecionada e chama o menu correspondente
		switch (option)
		{
			case TEMPERATURE: // Convers�o de unidades de temperatura
				temperatureMenu();
				break;
	
			case DISTANCE: // Convers�o de unidades de dist�ncia
				distanceMenu();
				break;
	
			case MASS: // Convers�o de unidades de massa
				massMenu();
				break;
	
			case TIME: // Convers�o de unidades de tempo
				timeMenu();
				break;
	
			case FORCE: // Convers�o de unidades de for�a
				forceMenu();
				break;
	
			case VELOCITY: // Convers�o de unidades de velocidade
				velocityMenu();
				break;
	
			case 7: // Hist�rico de convers�es
				historyMenu();
				break;
				
			case 8: // Mensagem de encerramento do programa
				printf("Encerrando o Conversor \x1b[30;45mP\x1b[0m\x1b[30;41mV\x1b[0m\x1b[30;48;5;202mX\x1b[0m...\nVolte sempre!");
				break;
		}
		
	} while (option != 8); // Encerra o programa quando a op��o Sair for selecionada
	
	return 0;
}
