#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.c"
#include "arvrb.c"
#include "unidade.c"
#include <ctype.h>

// Função para limpar caracteres indesejados
void clearCharacters(char *str)
{
    char *end;

    // Remove espaços e caracteres indesejados do final
    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == ';' || *end == ','))
    {
        *end = '\0';
        end--;
    }

    // Remove espaços do início
    char *start = str;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    // Copia a string limpa para o início
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

            // Separa a palavra em inglês e suas traduções em português
            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
                // Limpa a palavra em inglês
                clearCharacters(palavraIngles);

                // Divide as traduções em português
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    // Limpa cada tradução em português
                    clearCharacters(traducaoPortugues);

                    // Insere a palavra na árvore
                    insertPortugueseWord(arvore, traducaoPortugues, palavraIngles, unidadeAtual);

                    // Próxima tradução
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}

void menu()
{
    printf("\n------------------------------------------------------------------------------------------------- \n");
    printf("\nMenu de opções:\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em português e as equivalentes em inglês.\n");
    printf("2 - Informar uma palavra em português e imprimir todas as palavras em inglês equivalentes.\n");
    printf("3 - Informar uma palavra em inglês e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("4 - Informar uma palavra em português e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("5 - Imprimir a arvore completa\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

int main()
{

    RedBlackTreePT *rootNode = NULL;

    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/trabalhoEd2.txt", &rootNode);

    int op;
    char word[50];
    int unit;
    int removedNode;
    do
    {
        menu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja imprimir as palavras: ");
            scanf("%d", &unit);
            printWordsByUnit(rootNode, unit);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja imprimir as palavras em ingles: ");
            scanf(" %[^\n]", word); // Ajuste para ler palavras com espaços
            showPortugueseTranslation(&rootNode, word);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em inglês que deseja remover: ");
            scanf("%s", word);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%d", &unit);

            // Inicializa o contador de remoções
            int totalRemovido = 0;

            // Chama a função para remover a palavra na unidade informada
            removeWordByUnit(&rootNode, word, unit, &totalRemovido, &rootNode);

            if (totalRemovido > 0)
            {
                printf("Palavra '%s' na unidade %d removida de %d ocorrência(s) e os nós correspondentes da Árvore Rubro-Negra foram tratados, se necessário.\n", word, unit, totalRemovido);
            }
            else
            {
                printf("A palavra '%s' na unidade %d não foi encontrada em nenhuma árvore.\n", word, unit);
            }

            printf("\n--------------------------------------------------------------- \n");
            break;

        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", word);
            removedNode = removeRBTreeNode(&rootNode, word);
            if (removedNode)
                printf("A palavra %s foi removida com sucesso!\n\n", word);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            showRedBlackTree(rootNode);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 0:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nSaindo do programa...\n");
            printf("\n--------------------------------------------------------------- \n");
            break;
        default:
            printf("Insira um valor válido. \n");
            break;
        }
    } while (op != 0);

    // freeTree(raiz);

    return 0;
}