#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arv23.c"
#include "arvbin.c"
#include "unidade.c"

void clearCharacters(char *str)
{
    char *end = str + strlen(str) - 1;

    while (end > str && (*end == ';' || isspace((unsigned char)*end)))
    {
        *end = '\0';
        end--;
    }

    while (*str && isspace((unsigned char)*str))
    {
        str++;
    }
}

void loadFile(const char *nomeArquivo, PortugueseTree **arvore)
{
    FILE *dataFile = fopen(nomeArquivo, "r");

    if (dataFile != NULL)
    {
        char inputLine[256];
        int currentUnit = -1; // Inicia com um valor inválido

        while (fgets(inputLine, sizeof(inputLine), dataFile))
        {
            inputLine[strcspn(inputLine, "\n")] = 0; // Remove o caractere de nova linha

            if (inputLine[0] == '%')
            {
                // Verifica a linha de unidade e extrai o número
                if (sscanf(inputLine, "%% Unidade %d", &currentUnit) != 1)
                {
                    currentUnit = -1; // Reseta para inválido
                }
            }
            else if (currentUnit != -1)
            {
                // Processa linhas de palavras e traduções
                char englishWord[50], portugueseTranslations[200];

                if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
                {
                    clearCharacters(englishWord); // Limpa caracteres indesejados

                    // Divide as traduções em português separadas por vírgula
                    char *currentPortugueseTranslation = strtok(portugueseTranslations, ",");
                    while (currentPortugueseTranslation != NULL)
                    {
                        while (*currentPortugueseTranslation == ' ')
                        {
                            currentPortugueseTranslation++; // Remove espaços no início
                        }

                        clearCharacters(currentPortugueseTranslation); // Limpa caracteres indesejados
                        insertPortugueseTerm(arvore, currentPortugueseTranslation, englishWord, currentUnit);

                        currentPortugueseTranslation = strtok(NULL, ",");
                    }
                }
            }
        }

        fclose(dataFile);
    }
}


void menu()
{
    printf("\n===============================================================\n");
    printf("                         MENU DE OPCOES                       \n");
    printf("===============================================================\n");
    printf("[1] Exibir palavras e traducoes (por unidade)\n");
    printf("[2] Buscar traducoes de uma palavra em portugues\n");
    printf("[3] Remover palavra em ingles de uma unidade\n");
    printf("[4] Remover palavra em portugues de uma unidade\n");
    printf("[0] Sair\n");
    printf("===============================================================\n");
    printf("Escolha uma opcao: ");
}

int main()
{
    PortugueseTree *rootNode = NULL;
    PortugueseTree *parentNode = NULL;

    char userInput[50];
    int unit;
    int option;

    // Carrega o arquivo inicial
    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/text.txt", &rootNode);

    // Loop do menu principal
    do
    {
        menu();
        printf(">> "); // Indicador de entrada
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("\n---------------------------------------------------------------\n");
            printf("Informe a unidade para exibir as palavras e suas traducoes: ");
            scanf("%d", &unit);

            int hasUnitBeenPrinted = 0;
            printFormattedWordsByUnit(rootNode, unit, &hasUnitBeenPrinted);

            if (!hasUnitBeenPrinted)
            {
                printf("Nenhuma palavra encontrada para a unidade %d.\n", unit);
            }
            printf("---------------------------------------------------------------\n");
            break;

        case 2:
            printf("\n---------------------------------------------------------------\n");
            printf("Digite a palavra em portugues para buscar as equivalentes em ingles: ");
            scanf(" %[^\n]", userInput); // Lê uma linha inteira

            printAllTranslations(rootNode, userInput);
            printf("---------------------------------------------------------------\n");
            break;

        case 3:
            printf("\n---------------------------------------------------------------\n");
            printf("Digite a palavra em ingles para remover: ");
            scanf(" %[^\n]", userInput);
            printf("Informe a unidade associada: ");
            scanf("%d", &unit);

            removeEnglishTranslation(&rootNode, userInput, unit, &parentNode);

            printf("\nPalavra '%s' removida com sucesso da unidade %d.\n", userInput, unit);
            printf("---------------------------------------------------------------\n");
            break;

        case 4:
            printf("\n---------------------------------------------------------------\n");
            printf("Digite a palavra em portugues para remover: ");
            scanf(" %[^\n]", userInput);
            printf("Informe a unidade associada: ");
            scanf("%d", &unit);

            if (Remove_word_from_portuguese_unit(&rootNode, userInput, unit))
            {
                printf("\nPalavra '%s' removida com sucesso da unidade %d.\n", userInput, unit);
            }
            else
            {
                printf("\nFalha ao remover a palavra '%s' da unidade %d.\n", userInput, unit);
            }
            printf("---------------------------------------------------------------\n");
            break;

        case 0:
            printf("\n===============================================================\n");
            printf("Saindo do programa... Ate logo!\n");
            printf("===============================================================\n");
            break;

        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }
    } while (option != 0);

    deallocateTree(&rootNode);

    return 0;
}
