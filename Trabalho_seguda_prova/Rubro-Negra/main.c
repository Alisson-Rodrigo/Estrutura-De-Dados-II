#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingles.c"
#include "portugues.c"
#include "unidade.c"
#include <ctype.h>

void clearCharacters(char *str)
{
    char *end;


    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == ';' || *end == ','))
    {
        *end = '\0';
        end--;
    }


    char *start = str;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    memmove(str, start, strlen(start) + 1);
}

void loadFile(const char *fileName, RedBlackTreePT **treeRef)
{
    FILE *filePointer = fopen(fileName, "r");
    if (filePointer == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char inputLine[256];
    int currentUnit = 0;

    while (fgets(inputLine, sizeof(inputLine), filePointer))
    {
     
        inputLine[strcspn(inputLine, "\n")] = 0;

        if (inputLine[0] == '%')
        {
     
            sscanf(inputLine, "%% Unidade %d", &currentUnit);
        }
        else
        {
            char englishWord[50], portugueseTranslations[200];

       
            if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
            {

                clearCharacters(englishWord);

       
                char *portugueseTranslationToken = strtok(portugueseTranslations, ",;");
                while (portugueseTranslationToken != NULL)
                {
             
                    clearCharacters(portugueseTranslationToken);

               
                    insertPortugueseWord(treeRef, portugueseTranslationToken, englishWord, currentUnit);

               
                    portugueseTranslationToken = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(filePointer);
}

void exibirMenu()
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
    RedBlackTreePT *rootNode = NULL;

    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/Trabalho_seguda_prova/Rubro-Negra/trabalhoEd2.txt", &rootNode);

    int option;
    char inputWord[50];
    int unitValue;

    do
    {
        exibirMenu();
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("\n>>> Consultar palavras por unidade <<<\n");
            printf("Informe o numero da unidade desejada: ");
            scanf("%d", &unitValue);
            printf("\nResultado:\n");
            printf("%% Unidade %d\n", unitValue);
            printWordsByUnit(rootNode, unitValue);
            break;

        case 2:
            printf("\n>>> Buscar traducoes em ingles <<<\n");
            printf("Digite uma palavra em portugues: ");
            scanf(" %[^\n]", inputWord); 
            printf("\nTraducoes encontradas:\n");
            showPortugueseTranslation(&rootNode, inputWord);
            break;

        case 3:
            printf("\n>>> Remover palavra em ingles de uma unidade <<<\n");
            printf("Digite a palavra em ingles: ");
            scanf("%s", inputWord);
            printf("Informe a unidade: ");
            scanf("%d", &unitValue);

            int totalRemoved = 0;
            removeWordByUnit(&rootNode, inputWord, unitValue, &totalRemoved, &rootNode);

            if (totalRemoved > 0)
            {
                printf("\nPalavra '%s' removida com sucesso da unidade %d.\n", inputWord, unitValue);
            }
            else
            {
                printf("\nA palavra '%s' nao foi encontrada na unidade %d.\n", inputWord, unitValue);
            }
            break;

        case 4:
            printf("\n>>> Excluir palavra em portugues e traducoes <<<\n");
            printf("Digite a palavra em portugues: ");
            scanf(" %[^\n]", inputWord); 

            printf("Digite a unidade à qual a palavra pertence: ");
            scanf("%d", &unitValue);

         
            deleteWordFromTreeByUnit(&rootNode, inputWord, unitValue);

            break;

        case 0:
            printf("\nEncerrando o programa...\n");
            break;

        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }

        printf("\n=================================================================\n");

    } while (option != 0);

 
 

    return 0;
}
