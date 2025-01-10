#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "portugues.c"
#include "ingles.c"
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
        int currentUnit = -1;

        while (fgets(inputLine, sizeof(inputLine), dataFile))
        {
            inputLine[strcspn(inputLine, "\n")] = 0;

            if (inputLine[0] == '%')
            {

                if (sscanf(inputLine, "%% Unidade %d", &currentUnit) != 1)
                {
                    currentUnit = -1;
                }
            }
            else if (currentUnit != -1)
            {

                char englishWord[50], portugueseTranslations[200];

                if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
                {
                    clearCharacters(englishWord);

                    char *currentPortugueseTranslation = strtok(portugueseTranslations, ",");
                    while (currentPortugueseTranslation != NULL)
                    {
                        while (*currentPortugueseTranslation == ' ')
                        {
                            currentPortugueseTranslation++;
                        }

                        clearCharacters(currentPortugueseTranslation);
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

    char userInput[50];
    int unit;
    int option;

    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/Trabalho_seguda_prova/Rubro-Negra/trabalhoEd2.txt", &rootNode);

    do
    {
        menu();
        printf(">> ");
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
            scanf(" %[^\n]", userInput);

            printAllTranslations(rootNode, userInput);
            printf("---------------------------------------------------------------\n");
            break;

        case 3:
            printf("\n---------------------------------------------------------------\n");
            printf("Digite a palavra em ingles para remover: ");
            scanf(" %[^\n]", userInput);
            printf("Informe a unidade associada: ");
            scanf("%d", &unit);

            int removalResult = 0;
            removalResult = remove_english_word_by_unit(rootNode, userInput, unit, &rootNode);

            if (removalResult)
            {
                printf("\nPalavra '%s' removida com sucesso da unidade %d.\n", userInput, unit);
            }
            else
            {
                printf("\nFalha ao remover a palavra '%s' da unidade %d ou palavra não encontrada.\n", userInput, unit);
            }
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
        case 5:
            print_tree23(rootNode);
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
