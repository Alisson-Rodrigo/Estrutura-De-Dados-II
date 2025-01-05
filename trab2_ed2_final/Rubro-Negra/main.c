#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.c"
#include "arvrb.c"
#include "unidade.c"
#include <ctype.h>

// Funcao para limpar caracteres indesejados
void clearCharacters(char *str)
{
    char *end;

    // Remove espacos e caracteres indesejados do final
    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == ';' || *end == ','))
    {
        *end = '\0';
        end--;
    }

    // Remove espacos do inicio
    char *start = str;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    // Copia a string limpa para o inicio
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
        // Remove o caractere de nova linha
        inputLine[strcspn(inputLine, "\n")] = 0;

        if (inputLine[0] == '%')
        {
            // Atualiza a unidade atual
            sscanf(inputLine, "%% Unidade %d", &currentUnit);
        }
        else
        {
            char englishWord[50], portugueseTranslations[200];

            // Separa a palavra em ingles e suas traducoes em portugues
            if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
            {
                // Limpa a palavra em ingles
                clearCharacters(englishWord);

                // Divide as traducoes em portugues
                char *portugueseTranslationToken = strtok(portugueseTranslations, ",;");
                while (portugueseTranslationToken != NULL)
                {
                    // Limpa cada traducao em portugues
                    clearCharacters(portugueseTranslationToken);

                    // Insere a palavra na arvore
                    insertPortugueseWord(treeRef, portugueseTranslationToken, englishWord, currentUnit);

                    // Proxima traducao
                    portugueseTranslationToken = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(filePointer);
}

void exibirMenu()
{
    printf("\n=================================================================\n");
    printf("                  SISTEMA DE GERENCIAMENTO DE PALAVRAS          \n");
    printf("=================================================================\n");
    printf("1 - Consultar palavras por unidade (Portugues e Ingles).\n");
    printf("2 - Buscar traducoes em ingles para uma palavra em portugues.\n");
    printf("3 - Remover uma palavra em ingles de uma unidade especifica.\n");
    printf("4 - Excluir uma palavra em portugues e suas traducoes.\n");
    printf("5 - Exibir toda a estrutura da Arvore 2-3.\n");
    printf("6 - Mostrar a Arvore Binaria de uma palavra em portugues.\n");
    printf("0 - Encerrar o programa.\n");
    printf("=================================================================\n");
    printf("Escolha uma opcao: ");
}

int main()
{
    RedBlackTreePT *rootNode = NULL;

    // Carregar os dados iniciais do arquivo
    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/text.txt", &rootNode);

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
            scanf(" %[^\n]", inputWord); // Lê a entrada até encontrar um caractere de nova linha
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
            scanf(" %[^\n]", inputWord); // Lê até o caractere de nova linha

            printf("Digite a unidade à qual a palavra pertence: ");
            scanf("%d", &unitValue);

            // Chama a função para remover a palavra da unidade
            removerPalavraPortuguesaPorUnidade(&rootNode, inputWord, unitValue);

            break;

        case 5:
            printf("\n>>> Estrutura completa da Arvore 2-3 <<<\n");
            printf("\nArvore Atual:\n");
            showRedBlackTree(rootNode);
            break;

        case 6:
            printf("\n>>> Mostrar Arvore Binaria de uma palavra <<<\n");
            printf("Digite a palavra em portugues: ");
            scanf("%s", inputWord);
            printf("\nEstrutura da Arvore Binaria:\n");
            exibir_arvorebianria_dada_palavra_portuguesa(rootNode, inputWord);
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

    // Liberacao de memoria das arvores
    // freeTree(rootNode);

    return 0;
}
