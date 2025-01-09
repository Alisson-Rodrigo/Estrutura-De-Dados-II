#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/arv23.c"
#include "src/arvbin.c"
#include "src/remocao.h"

void carregarArquivo(const char *nomeArquivo, Portugues23 **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL)
    {
        char linha[256];

        char unidadeAtual[50];

        while (fgets(linha, sizeof(linha), arquivo))
        {

            if (linha[0] == '%')
            {
                // Atualiza a unidade corretamente
                sscanf(linha, "%% Unidade %[^\n]", unidadeAtual);
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
                  
                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
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
    printf("\n                                          MENU DE OPCOES                                          \n\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em portugues e as equivalentes em ingles.\n");
    printf("2 - Informar uma palavra em portugues e imprimir todas as palavras em ingles equivalentes.\n");
    printf("3 - Informar uma palavra em ingles e a unidade, remove-la da arvore binaria e da arvore 2-3.\n");
    printf("4 - Informar uma palavra em portugues e a unidade, remove-la da arvore binaria e da arvore 2-3.\n");
    printf("5 - Imprimir a arvore completa\n");
    printf("6 - Imprimir a arvore 2-3 pre-ordem, em ordem e pos-ordem\n");
    printf("7 - Executar os casos de teste\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

int main()
{
    Portugues23 *raiz = NULL;

    char palavra[50];
    char unidade[50];
    int removido;
    carregarArquivo("../text.txt", &raiz);
    int op;
    
    do
    {
        menu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja imprimir as palavras: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", unidade);
            imprimirPalavrasUnidade(raiz, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja imprimir as palavras em ingles: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", palavra);
            exibir_traducao_Portugues(&raiz, palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em ingles que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", unidade);
            removido = Remove_palavra_ingles_unidade(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", unidade);
            removido = Remove_palavra_portugues_unidade(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nArvore 2-3 e binaria:\n");
            exibir_tree23(raiz);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 6:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nArvore 2-3 pre-ordem:\n");
            arvore23_exibir_pre(raiz);
            printf("\n\nArvore 2-3 em ordem:\n");
            arvore23_exibir_ordem(raiz);
            printf("\n\nArvore 2-3 pos-ordem:\n");
            arvore23_exibir_pos(raiz);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 7:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nExecutando os casos de teste...\n");
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 0:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nSaindo do programa...\n");
            printf("\n--------------------------------------------------------------- \n");
            break;
        default:
            printf("Insira um valor valido!\n");
            break;
        }
    } while (op != 0);

    freeTree(&raiz);

    return 0;
}