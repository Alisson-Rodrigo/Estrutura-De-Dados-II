#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.c"
#include "arvrb.c"

void loadFile(const char *nomeArquivo, RedBlackTreePT **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL)
    {
        char linha[256];

        int unidadeAtual = 0;

        while (fgets(linha, sizeof(linha), arquivo))
        {
            linha[strcspn(linha, "\n")] = 0;

            if (linha[0] == '%')
            {
                // Atualiza a unidade corretamente
                sscanf(linha, "%% Unidade %d", &unidadeAtual);
            }
            else
            {
                char palavraIngles[50], traducoesPortugues[200];
                sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);

                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    insertPortugueseWord(arvore, traducaoPortugues, palavraIngles, unidadeAtual);

                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }

        fclose(arquivo);
        printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
    }
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
            scanf("%s", word);
            showPortugueseTranslation(&rootNode, word);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em inglês que deseja remover: ");
            scanf("%s", word);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%d", &unit);

            // Busca o nó correspondente na Árvore Vermelho-Preto
            RedBlackTreePT *nodeToRemove = SearchEnglishWordInRBTree(rootNode, word, unit);

            if (nodeToRemove != NULL)
            {
                printf("Nó vermelho-preto encontrado: Palavra em português: '%s'\n", nodeToRemove->info.portugueseWord);

                // Remover a palavra da árvore binária associada
                int removed = removeEnglishWord(&nodeToRemove->info.englishWordNode, word);

                if (removed)
                {
                    printf("Palavra '%s' da unidade %d removida da árvore binária.\n", word, unit);

                    // Se a árvore binária ficar vazia, remover o nó da Árvore Vermelho-Preto
                    if (nodeToRemove->info.englishWordNode == NULL)
                    {
                        removeRBTreeNode(&rootNode, nodeToRemove->info.portugueseWord);
                        printf("O nó correspondente à palavra '%s' foi removido da árvore vermelho-preto.\n", word);
                    }
                }
                else
                {
                    printf("A palavra '%s' não foi encontrada na árvore binária associada à unidade %d.\n", word, unit);
                }
            }
            else
            {
                printf("A palavra '%s' não foi encontrada na árvore vermelho-preto.\n", word);
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