#include <stdio.h>  // Biblioteca para trabalhar com funções de entrada e saída
#include <stdlib.h> // Biblioteca para trabalhar com funções de sistema
#include <locale.h> // Biblioteca para trabalhar com acentuação
#include <stdarg.h> // Biblioteca para trabalhar com funções que recebem um número variável de argumentos

// SEÇÃO: FUNÇÕES DE FERRAMENTAS

// Lê um número inteiro digitado pelo usuário
int getNumber()
{
  char entrada[100];
  int numero;

  do
  {
    printf("Selecione uma opção: ");
    fgets(entrada, sizeof(entrada), stdin);

    if (sscanf(entrada, " %d", &numero) != 1)
    {
      printf("Entrada inválida. Digite um número.\n");
    }
  } while (sscanf(entrada, "%d", &numero) != 1);

  return numero;
}

// SEÇÃO: FUNÇÕES DE MANIPULAÇÃO DE ARQUIVOS

// Adiciona um valor ao arquivo de histórico
void addValueToHistory(const char *format, ...)
{
  va_list arguments;
  va_start(arguments, format);

  FILE *arquivo = fopen("history.txt", "a+"); // Abre o arquivo no modo "append" (adicionar)
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    va_end(arguments);
    return;
  }

  vfprintf(arquivo, format, arguments); // Escreve no arquivo de saída
  fprintf(arquivo, "\n");               // Adiciona uma quebra de linha no final

  fclose(arquivo);   // Fecha o arquivo
  va_end(arguments); // Finaliza a lista de argumentos
}

// Lê o arquivo de histórico e exibe na tela
void readHistory()
{
  FILE *file;
  file = fopen("history.txt", "r"); // Abre o arquivo no modo "read"

  if (file == NULL)
  {
    printf("Nao ha valores no historico.\n");
    return;
  }

  fseek(file, 0, SEEK_END); // Move o cursor para o fim do arquivo
  long size = ftell(file);  // Determina o tamanho do arquivo

  if (size == 0)
  {
    printf("O histórico está vazio.\n"); // Exibe uma mensagem caso o arquivo esteja vazio
    fclose(file);
    return;
  }

  rewind(file); // Move o cursor de volta para o início do arquivo

  char linha[100]; // Buffer para armazenar cada linha do arquivo

  while (fgets(linha, sizeof(linha), file))
  {
    printf("%s", linha); // Exibe cada linha do arquivo
  }

  fclose(file); // Fecha o arquivo
}

// Apaga o conteúdo do arquivo de histórico
void cleanHistory()
{
  FILE *file;
  file = fopen("history.txt", "w"); // Abre o arquivo no modo "write"

  if (file == NULL)
  {
    printf("O arquivo já está vazio!.\n");
    return;
  }

  fclose(file); // Fecha o arquivo
}

typedef enum
{
  TEMPERATURE = 1,
  DISTANCE,
  MASS,
  TIME,
  FORCE,
  VELOCITY,
} conversions; // Tipos de conversões

// Menu principal
void menu(int *option)
{
  printf("1 - Temperatura\n2 - Dist�ncia\n3 - Massa\n4 - Tempo\n5 - For�a\n6 - Velocidade\n\n7 - Historico   8 - Sair\n\n");
  *option = getNumber();
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE TEMPO
typedef enum
{
  SECONDS = 1,
  MINUTES,
  HOURS,
  DAYS
} time;

// Convers�o de segundos para minutos, horas e dias
float secondsConversions(int seconds, time typeOfResult)
{
  float result;

  switch (typeOfResult)
  {
  case MINUTES:
    result = seconds / 60.0; // 1 minuto = 60 segundos
    break;
  case HOURS:
    result = seconds / 3600.0; // 1 hora = 3600 segundos
    break;
  case DAYS:
    result = (seconds / 3600.0) / 24; // 1 dia = 86400 segundos
    break;
  default:
    result = seconds / 60.0; // 1 minuto = 60 segundos
    break;
  }

  return result;
}

// Convers�o de minutos para segundos, horas e dias
float minutesConversions(int minutes, time typeOfResult)
{
  float result;

  switch (typeOfResult)
  {
  case SECONDS:
    result = minutes * 60.0; // 1 minuto = 60 segundos
    break;
  case HOURS:
    result = minutes / 60.0; // 1 hora = 60 minutos
    break;
  case DAYS:
    result = (minutes / 60.0) / 24; // 1 dia = 24 horas
    break;
  default:
    result = minutes * 60.0; // 1 minuto = 60 segundos
    break;
  }

  return result;
}

// Convers�o de horas para segundos, minutos e dias
float hoursConversions(int hours, time typeOfResult)
{
  float result;

  switch (typeOfResult)
  {
  case SECONDS:
    result = hours * 3600.0; // 1 hora = 3600 segundos
    break;
  case MINUTES:
    result = hours * 60.0; // 1 hora = 60 minutos
    break;
  case DAYS:
    result = hours / 24; // 1 dia = 24 horas
    break;
  default:
    result = hours * 3600.0; // 1 hora = 3600 segundos
    break;
  }

  return result;
}

// Convers�o de dias para segundos, minutos e horas;
float daysConversions(int days, time typeOfResult)
{
  float result;

  switch (typeOfResult)
  {
  case SECONDS:
    result = days * 86400.0; // 1 dia = 86400 segundos
    break;
  case MINUTES:
    result = days * 1440.0; // 1 dia = 1440 minutos
    break;
  case HOURS:
    result = days * 24; // 1 dia = 24 horas
    break;
  default:
    result = days * 86400.0; // 1 dia = 86400 segundos
    break;
  }

  return result;
}

// SEÇÃO: CONVERSÃO DE UNIDADES DE FORÇA

// Convers�o de Kilograma-for�a (kgf) para Newtons (N)
float kilogramForceToNewton(float kgf)
{
  return kgf * 9.80665; // 1 kgf = 9.80665 N
}

// Convers�o de Newtons (N) para Kilograma-for�a (kgf)
float NewtonTokilogramForce(float N)
{
  return N / 9.80665; // 1 N = 0.101972 kgf
}

// SEÇÃO: MENUS DE CONVERSÃO

// Menu de convers�o de unidades de temperatura
void temperatureMenu(){};

// Menu de convers�o de unidades de tempo
void timeMenu()
{
  int inputUnit, outputUnit;
  float value, result;
  printf("-- UNIDADES DE TEMPO --\n\n");
  do
  {
    printf("Selecione a unidade de tempo que voc� quer converter: \n1 - Segundos\n2 - Minutos\n3 - Horas\n4 - Dias\n");
    inputUnit = getNumber();

    if (inputUnit < 0 || inputUnit > 7)
    {
      system("CLS");
      printf("Digite uma op��o v�lida! Tente novamente: \n");
    }
  } while (inputUnit < 1 || inputUnit > 4);
  system("CLS");
  printf("Unidade de entrada: ");
  switch (inputUnit)
  {
  case SECONDS:
    printf("Segundos\n");
    break;
  case MINUTES:
    printf("Dias\n");
    break;
  case HOURS:
    printf("Horas\n");
    break;
  case DAYS:
    printf("Dias\n");
    break;
  }
  printf("Digite o valor que você quer converter: ");
  scanf("%f", &value);

  printf("\nSelecione a unidade de saida: \n");
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
    printf("Opção: ");
    scanf("%d", &outputUnit);

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
      result = secondsConversions(value, MINUTES);
      printf("%.2f segundo(s) é igual a %.2f minuto(s)", value, result);
      addValueToHistory("%.2f segundo(s) é igual a %.2f minuto(s)", value, result);
      break;

    case HOURS:
      result = secondsConversions(value, HOURS);
      printf("%.2f segundo(s) é igual a %.2f hora(s)", value, result);
      addValueToHistory("%.2f segundo(s) é igual a %.2f hora(s)", value, result);
      break;

    case DAYS:
      result = secondsConversions(value, DAYS);
      printf("%.2f segundo(s) é igual a %.2f dias(s)", value, result);
      addValueToHistory("%.2f segundo(s) é igual a %.2f dias(s)", value, result);
      break;
    }
    break;

  case MINUTES:
    switch (outputUnit)
    {
    case SECONDS:
      result = minutesConversions(value, SECONDS);
      printf("%.2f minuto(s) é igual a %.2f segundo(s)", value, result);
      addValueToHistory("%.2f minuto(s) é igual a %.2f segundo(s)", value, result);
      break;

    case HOURS:
      result = minutesConversions(value, HOURS);
      printf("%.2f minuto(s) é igual a %.2f hora(s)", value, result);
      addValueToHistory("%.2f minuto(s) é igual a %.2f hora(s)", value, result);
      break;

    case DAYS:
      result = minutesConversions(value, DAYS);
      printf("%.2f minuto(s) é igual a %.2f dias(s)", value, result);
      addValueToHistory("%.2f minuto(s) é igual a %.2f dias(s)", value, result);
      break;
    }
    break;

  case HOURS:
    switch (outputUnit)
    {
    case SECONDS:
      result = hoursConversions(value, SECONDS);
      printf("%.2f hora(s) é igual a %.2f segundo(s)", value, result);
      addValueToHistory("%.2f hora(s) é igual a %.2f segundo(s)", value, result);
      break;

    case MINUTES:
      result = hoursConversions(value, MINUTES);
      printf("%.2f hora(s) é igual a %.2f minutos(s)", value, result);
      addValueToHistory("%.2f hora(s) é igual a %.2f minutos(s)", value, result);
      break;

    case DAYS:
      result = hoursConversions(value, DAYS);
      printf("%.2f hora(s) é igual a %.2f dias(s)", value, result);
      addValueToHistory("%.2f hora(s) é igual a %.2f dias(s)", value, result);
      break;
    }
    break;

  case DAYS:
    switch (outputUnit)
    {
    case SECONDS:
      result = daysConversions(value, SECONDS);
      printf("%.2f dia(s) é igual a %.2f segundo(s)", value, result);
      addValueToHistory("%.2f dia(s) é igual a %.2f segundo(s)", value, result);
      break;

    case MINUTES:
      result = daysConversions(value, MINUTES);
      printf("%.2f dia(s) é igual a %.2f minutos(s)", value, result);
      addValueToHistory("%.2f dia(s) é igual a %.2f minutos(s)", value, result);
      break;

    case HOURS:
      result = daysConversions(value, HOURS);
      printf("%.2f dia(s) é igual a %.2f horas(s)", value, result);
      addValueToHistory("%.2f dia(s) é igual a %.2f horas(s)", value, result);
      break;
    }
    break;
  }
}

// Menu de convers�o de unidades de for�a
void forceMenu()
{
  int option;
  float result, value;

  printf("-- UNIDADES DE FORÇA --\n\n");
  do
  {
    printf("Selecione a convers�o que voc� deseja fazer: \n1 - Kilograma-força(kgf) para Newton(N)\n2 - Newton(N) para Kilograma-força(kgf)\n");
    option = getNumber();

    if (option < 1 || option > 2)
    {
      system("CLS");
      printf("Digite uma op��o valida! Tente novamente: \n");
    }
  } while (option < 1 || option > 2);

  printf("Digite o valor que voc� deseja converter: ");
  scanf("%f", &value);

  switch (option)
  {
  case 1:
    result = kilogramForceToNewton(value);
    break;
  case 2:
    result = NewtonTokilogramForce(value);
    break;
  }

  printf("RESULTADO: \n%.3f kilograma-for�a(s) � igual a %f newton(s)", value, result);
}

// Menu do historico de convers�es
void historyMenu()
{
  int option;
  printf(" -- HISTORICO DE CONVERS�ES --\n\n");
  readHistory();
  do
  {
    printf("\n\n1 - Limpar historico\n2 - Voltar\n");
    option = getNumber();
    if (option < 1 || option > 2)
    {
      system("CLS");
      printf("Digite uma op��o valida! Tente novamente: \n");
    }
  } while (option < 1 || option > 2);
  switch (option)
  {
  case 1:
    system("CLS");
    cleanHistory();
    printf("Historico limpo com sucesso!\n\n");
    break;
  case 2:
    system("CLS");
    break;
  }
}

int main()
{
  setlocale(LC_ALL, "Portuguese"); // Define a linguagem do programa para português

  int option;

  do
  {
    printf(" -- CONVERS�O DE UNIDADES --\n\n");

    printf("Bem vindo ao sistema de convers�o de unidades\n");
    printf("Selecione os tipos de unidades que voc� deseja converter: \n");

    do
    {
      menu(&option); // Exibe o menu principal
      system("CLS"); // Limpa a tela

      if (option < 0 || option > 8) // Validação da opção selecionada para imprimir uma mensagem de erro
      {
        system("CLS");
        printf("Digite uma op��o v�lida e tente novamente: \n");
      }
    } while (option < 0 || option > 8); // Validação da opção selecionada

    // Verifica qual opção foi selecionada e chama o menu correspondente
    if (option == TIME)
    {
      timeMenu();
    }
    else if (option == FORCE)
    {
      forceMenu();
    }
    else if (option == 7)
    {
      historyMenu(); // Exibe o histórico de conversões
    }
  } while (option != 8); // Encerra o programa quando a opção 8 for selecionada

  return 0;
}