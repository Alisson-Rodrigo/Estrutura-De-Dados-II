#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.c"
#include "arvrb.c"
#include "remocao.c"

void carregarArquivo(const char *nomeArquivo, PortuguesRB **arvore)
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

                    if(!inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual))
                        perror("Erro ao inserir na arvore vermelho preto");

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

    PortuguesRB *raiz = NULL;

    carregarArquivo("C:/Users/PurooLight/Documents/GitHub/Estrutura-De-Dados-II/trab2_ed2_final/trabalhoEd2.txt", &raiz);

    int op, res;
    char palavra[50];
    char unidade[50];
    int removido;
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
            scanf("%[^\n]", unidade);
            imprimirPalavrasUnidade(raiz, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja imprimir as palavras em ingles: ");
            scanf("%s", palavra);
            exibir_traducao_Portugues(&raiz, palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em ingles que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", unidade);
            Remove_palavra_ingles_unidade(&raiz, palavra, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", unidade);
            removido = Remove_palavra_portugues_unidade(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            exibirArvore(raiz);
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