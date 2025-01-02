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

void loadFile(const char *nomeArquivo, RedBlackTreePT **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Remove o caractere de nova linha
        linha[strcspn(linha, "\n")] = 0;

        if (linha[0] == '%')
        {
            // Atualiza a unidade atual
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        }
        else
        {
            char palavraIngles[50], traducoesPortugues[200];

            // Separa a palavra em ingles e suas traducoes em portugues
            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
                // Limpa a palavra em ingles
                clearCharacters(palavraIngles);

                // Divide as traducoes em portugues
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    // Limpa cada traducao em portugues
                    clearCharacters(traducaoPortugues);

                    // Insere a palavra na arvore
                    insertPortugueseWord(arvore, traducaoPortugues, palavraIngles, unidadeAtual);

                    // Proxima traducao
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
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
    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/trabalhoEd2.txt", &rootNode);

    int opcao;
    char palavra[50];
    int unidade;


    do
    {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n>>> Consultar palavras por unidade <<<\n");
            printf("Informe o numero da unidade desejada: ");
            scanf("%d", &unidade);
            printf("\nResultado:\n");
            printf("%% Unidade %d\n", unidade);
            printWordsByUnit(rootNode, unidade);
            break;

        case 2:
            printf("\n>>> Buscar traducoes em ingles <<<\n");
            printf("Digite uma palavra em portugues: ");
            scanf(" %[^\n]", palavra); // Lê a entrada até encontrar um caractere de nova linha
            printf("\nTraducoes encontradas:\n");
            showPortugueseTranslation(&rootNode, palavra);
            break;

        case 3:
            printf("\n>>> Remover palavra em ingles de uma unidade <<<\n");
            printf("Digite a palavra em ingles: ");
            scanf("%s", palavra);
            printf("Informe a unidade: ");
            scanf("%d", &unidade);

            int totalRemovido = 0;
            removeWordByUnit(&rootNode, palavra, unidade, &totalRemovido, &rootNode);

            if (totalRemovido > 0)
            {
                printf("\nPalavra '%s' removida com sucesso da unidade %d.\n", palavra, unidade);
            }
            else
            {
                printf("\nA palavra '%s' nao foi encontrada na unidade %d.\n", palavra, unidade);
            }
            break;

        case 4:
            printf("\n>>> Excluir palavra em portugues e traducoes <<<\n");
            printf("Digite a palavra em portugues: ");
            scanf(" %[^\n]", palavra); // Lê até o caractere de nova linha

            printf("Digite a unidade à qual a palavra pertence: ");
            scanf("%d", &unidade);

            // Chama a função para remover a palavra da unidade
            removerPalavraPortuguesaPorUnidade(&rootNode, palavra, unidade);

            break;

        case 5:
            printf("\n>>> Estrutura completa da Arvore 2-3 <<<\n");
            printf("\nArvore Atual:\n");
            showRedBlackTree(rootNode);
            break;

        case 6:
            printf("\n>>> Mostrar Arvore Binaria de uma palavra <<<\n");
            printf("Digite a palavra em portugues: ");
            scanf("%s", palavra);
            printf("\nEstrutura da Arvore Binaria:\n");
            exibir_arvorebianria_dada_palavra_portuguesa(rootNode, palavra);
            break;

        case 0:
            printf("\nEncerrando o programa...\n");
            break;

        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }

        printf("\n=================================================================\n");

    } while (opcao != 0);

    // Liberacao de memoria das arvores
    // freeTree(rootNode);

    return 0;
}
