#include <stdio.h>	// Biblioteca para trabalhar com funções de entrada e saída
#include <stdlib.h> // Biblioteca para trabalhar com funções de sistema
#include <locale.h> // Biblioteca para trabalhar com acentuação
#include <stdarg.h> // Biblioteca para trabalhar com funções que recebem um número variável de argumentos
#include <string.h> // Biblioteca para trabalhar com funções manipulação de strings
#include <ctype.h>	// Biblioteca para trabalhar com funções de caracteres
#include <stdbool.h> // Biblioteca para trabalhar com funções booleanas

/* 
	-- CONVERSOR DE UNIDADES PVX --
	
	Desenvolvido por:
	Guilherme Meneses Xavier Abreu Cardoso
	José Vanderlei Furtuna Tomé
	Pâmela Maria Pontes Frota 
	
	OBS: Para facilitar a clareza do código por outros desenvolvedores, as váriveis e funções foram nomeadas
	com nomes explicativos e na linguagem universal: inglês, esse é um padrão utilizado pela comunidade de 
	desenvolvedores atuais.
*/

// SEÇÃO: DEFINIÇÃO DE TIPOS

// Tipos de conversão
typedef enum
{
	TEMPERATURE = 1,
	DISTANCE,
	MASS,
	TIME,
	FORCE,
	VELOCITY,
} Conversions; // Tipos de conversão

// Tipos de unidades de temperatura
typedef enum
{
	CELSIUS = 1,
	FAHRENHEIT,
	KELVIN
} Degrees;

// Tipos de unidades de distância
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

// Tipo de unidades de força
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

// SEÇÃO: FERRAMENTAS

// Verifica se o valor digitado é um número ou não
bool isValidNumber(const char *input) {
	
	int i, points = 0;
	// Verifica se cada caractere é um dígito, ponto ou vírgula
    for (i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i]) && input[i] != '.' && input[i] != ',') {
            return false;
        }
    }
    
    // Verifica se há apenas um ponto ou vírgula
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '.' || input[i] == ',') {
            points++;
        }
    }
    
    // Se existir mais de uma virgula ou ponto, o número não é valido
    if (points > 1) {
    	return false;
	}
    
    return true;
}

// Recebe um número inteiro digitado pelo usuário e valida se realmente é um número ou não
int getOption()
{
	char input[100];
	int option;

	do
	{
		printf("Selecione uma opção: ");
		setbuf(stdin, NULL); // Limpa o buffer de entrada do programa
		
		fgets(input, sizeof(input), stdin); // Captura uma string digitada pelo usuário

		if (sscanf(input, " %d", &option) != 1) // Verifica se os caracteres lidos formam um número inteiro
		{
			printf("Entrada inválida. Digite um número. \n");
		}

	} while (sscanf(input, "%d", &option) != 1);

	return option;
}

// Recebe um número decimal digitado pelo usuário
float getValue()
{
	int i;
	float value;
	char input[40];
	char *endOfString; // Ponteiro para o final da string

	do
	{
		fgets(input, sizeof(input), stdin); // Lê a string digitada pelo usuário
		
		// Remove a quebra de linha no final do input
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        
        // Verifica se é um número decimal válido
        if (isValidNumber(input) || input[0] != '\0') {
            // Substitui a vírgula por ponto
            for (i = 0; i < strlen(input); i++) {
                if (input[i] == '.') {
                    input[i] = ',';
                    break;
                }
            }
            
            value = strtof(input, &endOfString);
        } else {
            printf("Entrada inválida. Digite um número decimal: ");
        }

	} while (!isValidNumber(input) || input[0] == '\0');

	return value;
}

// Menu de encerramento das conversões
void closingMenu()
{
	int option;
	do
	{
		printf("\n\nO que você quer fazer agora?\n1 - Voltar ao menu principal\n2 - Sair\n\n");
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
			printf("\nDigite uma opção válida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
}

// SEÇÃO: FUNÇÕES DE MANIPULAÇÃO DE ARQUIVOS DO HISTÓRICO

// Adiciona um valor ao arquivo de histórico
void addValueToHistory(const char *format, ...)
{
	va_list arguments; // cria uma variavel que vai receber a lista de argumentos da função
	va_start(arguments, format); // Analisa o formato da string inserida a partir dos argumentos

	FILE *arquivo = fopen("history.txt", "a+"); // Abre o arquivo history no modo "append" (adicionar)
	
	// Validação da conexão com o arquivo history
	if (arquivo == NULL)
	{
		printf("Erro ao abrir o arquivo de saída.\n");
		va_end(arguments);
		return;
	}

	vfprintf(arquivo, format, arguments); 	// Escreve a string formatada a partir dos argumentos no arquivo history
	fprintf(arquivo, "\n");					// Adiciona uma quebra de linha no final

	fclose(arquivo);	 // Fecha o arquivo history
	va_end(arguments); // Finaliza a lista de argumentos da função
}

// Leitura do arquivo de histórico e exibição dos dados na tela
void readHistory()
{
	FILE *file;
	file = fopen("history.txt", "r"); // Abre o arquivo history no modo "read" (leitura)

	// Validação da conexão com o arquivo history
	if (file == NULL)
	{
		printf("Histórico ainda não foi criado. Faça a sua primeira conversão ou execute o programa como administrador.\n");
		return;
	}

	fseek(file, 0, SEEK_END); // Move o cursor para o fim do arquivo history
	long size = ftell(file);	// Determina o tamanho do arquivo history

	if (size == 0)
	{
		printf("Não há valores no histórico.\n"); // Exibe uma mensagem caso o arquivo esteja vazio
		fclose(file);
		return;
	}

	rewind(file); // Move o cursor de volta para o início do arquivo history 

	char row[100]; // Buffer para armazenar cada linha do arquivo history

	while (fgets(row, sizeof(row), file))
	{
		printf("%s", row); // Exibe cada linha do arquivo history na tela
	}

	fclose(file); // Fecha o arquivo history
}

// Apaga o conteúdo do arquivo de histórico
void cleanHistory()
{
	FILE *file;
	file = fopen("history.txt", "w"); // Abre o arquivo no modo "write" (escrever)

	// Verifica se o arquivo existe ou já está vazio
	if (file == NULL)
	{
		printf("O histórico já está vazio.\n");
		return;
	}

	fclose(file); // Fecha o arquivo
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE TEMPERATURA

// Conversão de graus celsius para fahrenheit e kelvin
float celsiusConversions(float initialDegrees, Degrees typeOfResult)
{
	float finalDegrees;

	switch (typeOfResult)
	{
		// Conversão de celsius para fahrenheit
		case FAHRENHEIT:
			finalDegrees = (initialDegrees / 5.0) * 9 + 32; // F = (C / 5) * 9 + 32
			break;
			
		// Conversão de celsius para kelvin
		case KELVIN:
			finalDegrees = initialDegrees + 273.15; // K = C + 273.15
			break;
	
		default:
			return initialDegrees;
	}

	return finalDegrees;
}

// Conversão de graus fahrenheit para celsius e kelvin
float fahreheitConversions(float initialDegrees, Degrees typeOfReturn)
{
	float finalDegrees;
	switch (typeOfReturn)
	{
		// Conversão de Fahreheit para Celsius
		case CELSIUS:
			finalDegrees = ((initialDegrees - 32) / 9) * 5;
			break;
			
		// Conversão de Fahreheit para Kelvin
		case KELVIN:
			finalDegrees = ((initialDegrees - 32) / 9) * 5 + 273.15;
			break;
				
		default:
			finalDegrees = initialDegrees;
			break;
	}
	
	return finalDegrees;
}

// Conversão de graus kelvin para celsius e fahrenheit
float kelvinConversions(float initialDegrees, Degrees typeOfResult)
{
	float finalDegrees;
	
	switch (typeOfResult)
	{
		// Conversão de kelvin para celsius
		case CELSIUS:
			finalDegrees = initialDegrees - 273.15; // K = C + 273.15
			break;
			
		// Conversão de kelvin para fahrenheit
		case FAHRENHEIT:
			finalDegrees = ((initialDegrees - 273.15) / 5) * 9 + 32; // F = (C / 5) * 9 + 32
			break;
			
		default:
			finalDegrees = initialDegrees;
			break;
	}
	
	return finalDegrees;
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE DISTÂNCIA

// Conversão de quilometros para milhas e leguas
double kmConversions(double km, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
		// Conversão de quilômetros para milhas
		case MILES:
			result = km * 0.621371; // 1 km = 0.621371 milhas
			break;
			
		// Conversão de quilômetros para léguas
		case LEAGUES:
			result = km * 0.207123; // 1 km = 0.207123 léguas
			break;
			
		default:
			result = km;
			break;
	}

	return result;
}

// Conversão de milhas para quilômetros e léguas
double milesConversions(double miles, Distance typeOfResult)
{
	double result;

	switch (typeOfResult)
	{
		// Conversão de milhas para quilômetros
		case KILOMETER:
			result = miles * 1.60934; // 1 milha = 1.60934 km
			break;
			
		// Conversão de milhas para léguas
		case LEAGUES:
			result = miles / 3.0; // 1 milha = 3 léguas
			break;
			
		default:
			result = miles;
			break;
	}

	return result;
}

// Conversão de léguas para quilômetros e milhas
float leaguesConversions(float leagues, Distance typeOfResult)
{

	float result;

	switch (typeOfResult)
	{
		// Conversão de léguas para quilômetros
		case KILOMETER:
			result = leagues * 4.82803; // 1 légua = 4.82803 km
			break;
			
		// Conversão de léguas para milhas
		case MILES:
			result = leagues * 3.0; // 1 légua = 3 milhas
			break;
			
		default:
			result = leagues;
			break;
	}

	return result;
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE MASSA

// Conversão de quilogramas para arrobas
float massConversions(float initialMass, Mass typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Conversão de quilogramas para arrobas
		case KILOGRAM:
			result = initialMass * 14.688; // 1kg ~ 0.068 arrobas
			break;
	
		// Conversão de arrobas para quilogramas
		case ARROBA:
			result = initialMass / 14.688; // 1 arroba ~ 14.688kg
			break;
			
		default:
			result = initialMass;
			break;
		}
		
		

	return result;
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE TEMPO

// Conversão de segundos para minutos, horas e dias
float secondsConversions(int seconds, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Conversão de segundos para minutos
		case MINUTES:
			result = seconds / 60.0; // 1 minuto = 60 segundos
			break;
			
		// Conversão de segundos para horas
		case HOURS:
			result = seconds / 3600.0; // 1 hora = 3600 segundos
			break;
		
		// Conversão de segundos para dias
		case DAYS:
			result = (seconds / 3600.0) / 24; // 1 dia = 86400 segundos
			break;
			
		default:
			result = seconds;
			break;
	}

	return result;
}

// Conversão de minutos para segundos, horas e dias
float minutesConversions(int minutes, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Conversão de minutos para segundos
		case SECONDS:
			result = minutes * 60.0; // 1 minuto = 60 segundos
			break;
			
		// Conversão de minutos para horas
		case HOURS:
			result = minutes / 60.0; // 1 hora = 60 minutos
			break;
			
		// Conversão de minutos para dias
		case DAYS:
			result = (minutes / 60.0) / 24; // 1 dia = 24 horas
			break;
			
		default:
			result = minutes;
			break;
	}

	return result;
}

// Conversão de horas para segundos, minutos e dias
float hoursConversions(int hours, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Conversão de horas para segundos
		case SECONDS:
			result = hours * 3600.0; // 1 hora = 3600 segundos
			break;
			
		// Conversão de horas para minutos
		case MINUTES:
			result = hours * 60.0; // 1 hora = 60 minutos
			break;
			
		// Conversão de horas para dias
		case DAYS:
			result = hours / 24; // 1 dia = 24 horas
			break;
			
		default:
			result = hours * 3600.0; // 1 hora = 3600 segundos
			break;
	}

	return result;
}

// Conversão de dias para segundos, minutos e horas
float daysConversions(int days, Time typeOfResult)
{
	float result;

	switch (typeOfResult)
	{
		// Conversão de dias para segundos
		case SECONDS:
			result = days * 86400.0; // 1 dia = 86400 segundos
			break;
			
		// Conversão de dias para minutos
		case MINUTES:
			result = days * 1440.0; // 1 dia = 1440 minutos
			break;
			
		// Conversão de dias para horas
		case HOURS:
			result = days * 24; // 1 dia = 24 horas
			break;
			
		default:
			result = days * 86400.0; // 1 dia = 86400 segundos
			break;
	}

	return result;
}

// SEÇÂO: CONVERSÂO DE UNIDADES DE FORÇA

float forceConversions(float initialForce, Force typeOfResult)
{

	float result;

	switch (typeOfResult)
	{
		// Conversão de quilograma-força para newton
		case NEWTON:
			result = initialForce * 9.80665; // 1 kgf = 9.80665 N
			break;
	
		// Conversão de newton para quilograma-força
		case KILOGRAM_FORCE:
			result = initialForce / 9.80665; // 1 N = 0.101972 kgf
			break;
			
		default:
			result = initialForce;
			break;
	}

	return result;
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE VELOCIDADE

float velocityConversion(float initialVelocity, Velocity typeOfResult)
{
	float finalVelocity;

	switch (typeOfResult)
	{
	// Conversãoo de quilômetros por hora para metros por segundo
		case KILOMETER_PER_HOUR:
			finalVelocity = initialVelocity * 3.6;
			break;
	
		// Conversão de metros por segundo para quilï¿½metros por hora
		case METER_PER_SECOND:
			finalVelocity = initialVelocity / 3.6;
			break;
			
		default:
			finalVelocity = initialVelocity;
			break;
	}

	return finalVelocity;
}

// SEÇÃO: MENUS DE CONVERSÃO

// Menu principal
void menu(int *option)
{
	printf("1 - Temperatura\n2 - Distância\n3 - Massa\n4 - Tempo\n5 - Força\n6 - Velocidade\n\n7 - Histórico   8 - Sair\n\n");
	*option = getOption();
}

// Menu de conversão de unidades de temperatura
void temperatureMenu()
{
	float initialDegrees;
	int option, i = 0;
	char initialUnit, finalUnit;

	printf("-- UNIDADES DE TEMPERATURA -- \n\n");

	do
	{

		printf("Escreva a temperatura que deseja converter (Ex: 25C. Unidades possíveis: C, F e K):\n");
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
						printf("\033[1;32m%.3f graus Celsius é igual a %.3f graus Kelvin\033[0m\n", initialDegrees, celsiusConversions(initialDegrees, KELVIN));
						addValueToHistory("%.3f graus Celsius é igual a %.3f graus Kelvin", initialDegrees, celsiusConversions(initialDegrees, KELVIN));
						i++;
					}
					else if ((finalUnit == 'F') || (finalUnit == 'f'))
					{
						printf("\033[1;32m%.3f graus Celsius é igual a %.3f graus Fahrenheit\033[0m\n", initialDegrees, celsiusConversions(initialDegrees, FAHRENHEIT));
						addValueToHistory("%.3f graus Celsius é igual a %.3f graus Fahrenheit", initialDegrees, celsiusConversions(initialDegrees, FAHRENHEIT));
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura não identificada!!!\n");
					}
	
				} while (i == 0);
				break;
	
			case 'F':
				do
				{
					printf("Para Celcius ou Kelvin? (C/K): ");
					setbuf(stdin, NULL);			// Limpa o buffer do teclado
					scanf(" %c", &finalUnit); // LÃª a unidade de saida
	
					// Validação da unidade de saida e conversão de graus fahrenheit para celsius e kelvin
					if ((finalUnit == 'K') || (finalUnit == 'k'))
					{
						printf("\033[1;32m%.3f graus Fahrenheit é igual a %.3f graus Kelvin\033[0m\n", initialDegrees, fahreheitConversions(initialDegrees, KELVIN));
						addValueToHistory("%.3f graus Fahrenheit é igual a %.3f graus Kelvin", initialDegrees, fahreheitConversions(initialDegrees, KELVIN));
						i++;
					}
					else if ((finalUnit == 'C') || (finalUnit == 'c'))
					{
						printf("\033[1;32m%.3f graus Fahrenheit é igual a %.3f graus Celsius\033[0m\n", initialDegrees, fahreheitConversions(initialDegrees, CELSIUS));
						addValueToHistory("%.3f graus Fahrenheit é igual a %.3f graus Celsius", initialDegrees, fahreheitConversions(initialDegrees, CELSIUS));
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura não identificada! Tente novamente.\n");
					}
	
				} while (i == 0);
				break;
	
			case 'K':
				do
				{
					printf("Para Celcius ou Fahrenheit? (C/F): ");
					setbuf(stdin, NULL);			// Limpa o buffer do teclado
					scanf(" %c", &finalUnit); // Lê a unidade de saída
	
					// Validação da unidade de saída e conversão de graus kelvin para celsius e fahrenheit
					if ((finalUnit == 'F') || (finalUnit == 'f'))
					{
						printf("\033[1;32m%.3f graus Kelvin é igual a %.3f graus Fahrenheit\033[0m\n", initialDegrees, kelvinConversions(initialDegrees, FAHRENHEIT)); // Chama a funÃ§Ã£o de conversÃ£o
						addValueToHistory("%.3f graus Kelvin é igual a %.3f graus Fahrenheit", initialDegrees, kelvinConversions(initialDegrees, FAHRENHEIT));				 // Adiciona o resultado ao arquivo de histÃ³rico
						i++;
					}
					else if ((finalUnit == 'C') || (finalUnit == 'c'))
					{
						printf("\033[1;32m%.3f graus Kelvin é igual a %.3f graus Celsius\033[0m\n", initialDegrees, kelvinConversions(initialDegrees, CELSIUS)); // Chama a funÃ§Ã£o de conversÃ£o
						addValueToHistory("%.3f graus Kelvin é igual a %.3f graus Celsius", initialDegrees, kelvinConversions(initialDegrees, CELSIUS));				 // Adiciona o resultado ao arquivo de histÃ³rico
						i++;
					}
					else
					{
						printf("Unidade de medida de temperatura não identificada! Tente novamente\n");
					}
	
				} while (i == 0);
				break;
		}

		if (initialUnit != 'C' && initialUnit != 'F' && initialUnit != 'K')
		{
			printf("Unidade de medida de temperatura não identificada! Tente novamente\n ");
		}

	} while (initialUnit != 'C' && initialUnit != 'F' && initialUnit != 'K');

	closingMenu();
}

// Menu de conversão de unidades de distância
void distanceMenu()
{
	int initialUnit, finalUnit;
	float value, result;

	do
	{
		printf("-- UNIDADES DE DISTÂNCIA --\n\n");
		printf("Selecione a unidade de distância que você quer converter: \n\n1 - Quilômetro(km)\n2 - Milha(s)\n3 - Légua(s)\n\n");
		initialUnit = getOption();

		// Validação da unidade de entrada
		if (initialUnit < 1 || initialUnit > 3)
		{
			system("CLS");
			printf("Digite uma opção válida! Tente novamente: \n");
		}

	} while (initialUnit < 1 || initialUnit > 3);

	// Limpa a tela e exibe a unidade de entrada
	system("CLS");
	printf("-- UNIDADES DE DISTÂNCIA --\n\n");
	
	// Imprime a unidade de medida selecionada
	printf("Unidade selecionada: ");
	switch (initialUnit)
	{
		case KILOMETER:
			printf("Quilômetro(km)\n");
			break;
		case MILES:
			printf("Milhas\n");
			break;
		case LEAGUES:
			printf("Léguas\n");
			break;
	}

	printf("\nDigite o valor que você quer converter: ");
	value = getValue();

	printf("\nSelecione a unidade de saída: \n");

	do
	{
		// Exibe apenas as unidades que não foram selecionadas como entrada
		if (initialUnit != KILOMETER)
			printf("1 - Quilômetro(km)\n");
		if (initialUnit != MILES)
			printf("2 - Milha(s)\n");
		if (initialUnit != LEAGUES)
			printf("3 - Légua(léguas)\n");
		printf("Opção: ");
		scanf("%d", &finalUnit);

		// Validação da unidade de saída
		if (finalUnit <= 0 || finalUnit == initialUnit || finalUnit >= 4)
		{
			printf("Digite uma opção válida! Tente novamente: \n");
		}

	} while (finalUnit <= 0 || finalUnit == initialUnit || finalUnit >= 4);

	// Conversão de unidades
	switch (initialUnit)
	{
	case KILOMETER:
		switch (finalUnit)
		{
		case MILES:
			result = kmConversions(value, MILES);														// Chama a função de conversão
			printf("\033[1;32m%.3f quilômetro(s) é igual a %.3f milha(s)\033[0m\n", value, result); 	// Exibe o resultado
			addValueToHistory("%.3f quilômetro(s) é igual a %.3f milha(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
			break;

		case LEAGUES:
			result = kmConversions(value, LEAGUES);													// Chama a função de conversão
			printf("\033[1;32m%.3f quilômetro(s) é igual a %.3f légua(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f quilômetro(s) é igual a %.3f légua(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
			break;
		}
		break;

	case MILES:
		switch (finalUnit)
		{
			case KILOMETER:
				result = milesConversions(value, KILOMETER);											// Chama a função de conversão
				printf("\033[1;32m%.3f milha(s) é igual a %.3f quilômetro(s)\033[0m\n", value, result); // Exibe o resultado
				addValueToHistory("%.3f milha(s) é igual a %.3f quilômetro(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
	
			case LEAGUES:
				result = milesConversions(value, LEAGUES);											// Chama a função de conversão
				printf("\033[1;32m%.3f milha(s) é igual a %.3f légua(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f milha(s) é igual a %.3f légua(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;

	case LEAGUES:
		switch (finalUnit)
		{
			case KILOMETER:
				result = leaguesConversions(value, KILOMETER);											// Chama a função de conversão
				printf("\033[1;32m%.3f légua(s) é igual a %.3f quilômetro(s)\033[0m\n", value, result); // Exibe o resultado
				addValueToHistory("%.3f légua(s) é igual a %.3f quilômetro(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
	
			case MILES:
				result = leaguesConversions(value, MILES);											// Chama a função de conversão
				printf("\033[1;32m%.3f légua(s) é igual a %.3f milha(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f légua(s) é igual a %.3f milha(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;
	}

	closingMenu();
}

// Menu de conversão de unidades de tempo
void timeMenu()
{
	int inputUnit, outputUnit;
	float value, result;
	do
	{
		printf("-- UNIDADES DE TEMPO --\n\n");
		printf("Selecione a unidade de tempo que você quer converter: \n\n1 - Segundos\n2 - Minutos\n3 - Horas\n4 - Dias\n\n");
		inputUnit = getOption();

		if (inputUnit < 1 || inputUnit > 4)
		{
			system("CLS");
			printf("Digite uma opção válida! Tente novamente: \n\n");
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

	printf("Digite o valor que você quer converter: ");
	value = getValue();

	printf("\nSelecione a unidade de saída: \n\n");
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
			printf("Digite uma opção valida! Tente novamente: \n");
		}

	} while (outputUnit <= 0 || outputUnit == inputUnit || outputUnit >= 5);

	switch (inputUnit)
	{
	case SECONDS:
		switch (outputUnit)
		{
			case MINUTES:
				result = secondsConversions(value, MINUTES);											// Chama a função de conversão
				printf("\033[0;32m%.3f segundo(s) é igual a %.3f minuto(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) é igual a %.3f minuto(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
	
			case HOURS:
				result = secondsConversions(value, HOURS);												// Chama a função de conversão
				printf("\033[0;32m%.3f segundo(s) é igual a %.4f hora(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) é igual a %.3f hora(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
	
			case DAYS:
				result = secondsConversions(value, DAYS);											// Chama a função de conversão
				printf("\033[0;32m%.3f segundo(s) é igual a %f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f segundo(s) é igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;

	case MINUTES:
		switch (outputUnit)
		{
			case SECONDS:
				result = minutesConversions(value, SECONDS);											// Chama a função de conversão
				printf("\033[1;32m%.3f minuto(s) é igual a %.3f segundo(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) é igual a %.3f segundo(s)", value, result);			// Adiciona o resultado ao arquivo dehistórico
				break;
	
			case HOURS:
				result = minutesConversions(value, HOURS);											// Chama a função de conversão
				printf("\033[1;32m%.3f minuto(s) é igual a %.3f hora(s)\033[0m\n", value, result);	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) é igual a %.3f hora(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
	
			case DAYS:
				result = minutesConversions(value, DAYS);											 // Chama a função de conversão
				printf("\033[1;32m%.3f minuto(s) é igual a %.3f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f minuto(s) é igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;

	case HOURS:
		switch (outputUnit)
		{
			case SECONDS:
				result = hoursConversions(value, SECONDS);												// Chama a função de conversão
				printf("\033[1;32m%.3f hora(s) é igual a %.3f segundo(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) é igual a %.3f segundo(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
	
			case MINUTES:
				result = hoursConversions(value, MINUTES);												// Chama a função de conversão
				printf("\033[1;32m%.3f hora(s) é igual a %.3f minutos(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) é igual a %.3f minutos(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
	
			case DAYS:
				result = hoursConversions(value, DAYS);												// Chama a função de conversão
				printf("\033[1;32m%.3f hora(s) é igual a %.3f dias(s)\033[0m\n", value, result); 	// Exibe o resultado
				addValueToHistory("%.3f hora(s) é igual a %.3f dias(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;

	case DAYS:
		switch (outputUnit)
		{
			case SECONDS:
				result = daysConversions(value, SECONDS);												// Chama a função de conversão
				printf("\033[1;32m%.3f dia(s) é igual a %.3f segundo(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) é igual a %.3f segundo(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
	
			case MINUTES:
				result = daysConversions(value, MINUTES);												// Chama a função de conversão
				printf("\033[1;32m%.3f dia(s) é igual a %.3f minutos(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) é igual a %.3f minutos(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
	
			case HOURS:
				result = daysConversions(value, HOURS);													// Chama a função de conversão
				printf("\033[1;32m%.3f dia(s) é igual a %.3f horas(s)\033[0m\n", value, result); 		// Exibe o resultado
				addValueToHistory("%.3f dia(s) é igual a %.3f horas(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
				break;
		}
		break;
	}

	closingMenu();
}

// Menu de conversão de unidades de força
void forceMenu()
{
	int option;
	float result, value;

	do
	{
		printf("-- UNIDADES DE FORÇA --\n\n");
		printf("Selecione a conversão que você deseja fazer: \n\n1 - Kilograma-força(kgf) para Newton(N)\n2 - Newton(N) para Kilograma-força(kgf)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma opção valida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	switch(option) {
		case KILOGRAM_FORCE:
			printf("Unidade de Entrada: kgf\nDigite o valor que você deseja converter para Newton: ");
			break;
			
		case NEWTON:
			printf("Unidade de Entrada: Newton\nDigite o valor que você deseja converter para kgf: ");	
			break;
			
	}
	value = getValue();

	switch (option)
	{
		// Conversão de quilogramas para arrobas
		case KILOGRAM_FORCE:
			result = forceConversions(value, NEWTON);													// Chama a função de conversão
			printf("\033[1;32m%.3f kilograma-força(s) é igual a %f newton(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f kilograma-força(s) é igual a %f newton(s)", value, result); 		
			break;
	
		// Conversão de arrobas para quilogramas
		case NEWTON:
			result = forceConversions(value, KILOGRAM_FORCE);											// Chama a função de conversão
			printf("\033[1;32m%.3f newton(s) é igual a %f kilograma-força(s)\033[0m\n", value, result); // Exibe o resultado
			addValueToHistory("%.3f newton(s) é igual a %f kilograma-força(s)", value, result);			// Adiciona o resultado ao arquivo de histórico
			break;
	}

	closingMenu();
}

//  Menu de conversção de unidades de massa
void massMenu()
{
	int option;
	float value, result;

	do
	{
		printf("-- UNIDADES DE MASSA --\n\n");
		printf("Selecione a conversão que você deseja fazer: \n\n1 - Kilograma(kg) para Arroba(arroba)\n2 - Arroba(arroba) para Kilograma(kg)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma opção válida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);

	switch(option) {
		case KILOGRAM:
			printf("Unidade de Entrada: Kg\nDigite o valor que você deseja converter para Arroba: ");
			break;

		case ARROBA:
			printf("Unidade de Entrada: Arroba\nDigite o valor que você deseja converter para Kg: ");
			break;
	}
	value = getValue();

	switch (option)
	{
		// Conversão de quilogramas para arrobas
		case KILOGRAM:
			result = massConversions(value, ARROBA);													// Chama a função de conversão
			printf("\033[1;32m%.3f kilograma(s) é igual a %.3f arroba(s)\033[0m\n", value, result); 	// Exibe o resultado
			addValueToHistory("%.3f kilograma(s) é igual a %.3f arroba(s)", value, result); 			// Adiciona o resultado ao arquivo de histórico
			break;
	
		// Conversão de arrobas para quilogramas
		case ARROBA:
			result = massConversions(value, KILOGRAM);													// Chama a função de conversão
			printf("\033[1;32m%.3f arroba(s) é igual a %.3f kilograma(s)\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f arroba(s) é igual a %.3f kilograma(s)", value, result);				// Adiciona o resultado ao arquivo de histórico
			break;
	}

	closingMenu(); // Chama o menu de fechamento
}

// Menu de conversão de unidades de velocidade
void velocityMenu()
{

	int option;
	float value, result;

	do
	{
		printf("-- UNIDADES DE VELOCIDADE --\n\n");
		printf("Selecione a conversão que você deseja fazer: \n\n1 - Kilometro por hora(km/h) para Metro por segundo(m/s)\n2 - Metro por segundo(m/s) para Kilometro por hora(km/h)\n\n");
		option = getOption();

		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma opção válida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	

	switch (option) {
		case KILOMETER_PER_HOUR:
			printf("Unidade de Entrada: Km/h\nDigite o valor que você deseja converter para m/s: ");
			break;
		
		case METER_PER_SECOND:
			printf("Unidade de Entrada: m/s\nDigite o valor que você deseja converter para Km/s: ");
			break;
	}
	value = getValue();

	switch (option)
	{
		case KILOMETER_PER_HOUR:
			result = velocityConversion(value, METER_PER_SECOND);															// Chama a função de conversão
			printf("\033[1;32m%.3f kilometro(s) por hora é igual a %.3f metro(s) por segundo\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f kilometro(s) por hora é igual a %.3f metro(s) por segundo", value, result);				// Adiciona o resultado ao arquivo de histórico
			break;
	
		case METER_PER_SECOND:
			result = velocityConversion(value, KILOMETER_PER_HOUR);															// Chama a função de conversão
			printf("\033[1;32m%.3f metro(s) por segundo é igual a %.3f kilometro(s) por hora\033[0m\n", value, result);		// Exibe o resultado
			addValueToHistory("%.3f metro(s) por segundo é igual a %.3f kilometro(s) por hora", value, result);				// Adiciona o resultado ao arquivo de histórico
			break;
	}

	closingMenu();
}

// Menu do histórico de conversões
void historyMenu()
{
	int option;
	printf(" -- HISTÓRICO DE CONVERSÕES --\n\n");
	readHistory();
	do
	{
		printf("\n\n1 - Limpar histórico\n2 - Voltar\n");
		option = getOption();
		if (option < 1 || option > 2)
		{
			system("CLS");
			printf("Digite uma opção válida! Tente novamente: \n");
		}
	} while (option < 1 || option > 2);
	
	switch (option)
	{
		case 1:
			system("CLS");
			cleanHistory();
			printf("Histórico limpo com sucesso!\n\n");
			break;
		case 2:
			system("CLS");
			break;
	}
	
	return;
}

int main()
{
	setlocale(LC_ALL, "Portuguese"); // Define a linguagem do programa para português

	int option;

	do
	{
		// Exibe o cabeçalho do programa usando códigos de escape de cores ANSI
		do
		{
		printf(" -- CONVERSOR DE UNIDADES \x1b[30;45mP\x1b[0m\x1b[30;41mV\x1b[0m\x1b[30;48;5;202mX\x1b[0m --\n\n"); // -- CONVERSÃO DE UNIDADE PVX --

		printf("Bem vindo(a) ao sistema de conversão de unidades\n");
		printf("Selecione os tipos de unidades que você deseja converter: \n");

			menu(&option); // Exibe o menu principal
			system("CLS"); // Limpa a tela

			if (option < 1 || option > 8) // Validação da opção selecionada para imprimir uma mensagem de erro
			{
				system("CLS");
				printf("Digite uma opção válida e tente novamente: \n");
			}
		} while (option < 1 || option > 8); // Validação da opção selecionada

		// Verifica qual opçõo foi selecionada e chama o menu correspondente
		switch (option)
		{
			case TEMPERATURE: // Conversão de unidades de temperatura
				temperatureMenu();
				break;
	
			case DISTANCE: // Conversão de unidades de distância
				distanceMenu();
				break;
	
			case MASS: // Conversão de unidades de massa
				massMenu();
				break;
	
			case TIME: // Conversão de unidades de tempo
				timeMenu();
				break;
	
			case FORCE: // Conversão de unidades de força
				forceMenu();
				break;
	
			case VELOCITY: // Conversão de unidades de velocidade
				velocityMenu();
				break;
	
			case 7: // Histórico de conversões
				historyMenu();
				break;
				
			case 8: // Mensagem de encerramento do programa
				printf("Encerrando o Conversor \x1b[30;45mP\x1b[0m\x1b[30;41mV\x1b[0m\x1b[30;48;5;202mX\x1b[0m...\nVolte sempre!");
				break;
		}
		
	} while (option != 8); // Encerra o programa quando a opção Sair for selecionada
	
	return 0;
}
