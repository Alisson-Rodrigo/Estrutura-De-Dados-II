#include <stdio.h>
#include <stdlib.h>
#include "23.c"
#include "alocacao.c"

#define MEMORY_SIZE (1024 * 1024) // Tamanho total da memória (1MB por bloco)

int menu() {
    int op;
    printf("\n1 - Alocar Nós");
    printf("\n2 - Liberar Nós");
    printf("\n3 - Exibir Nós Endereços)");
    printf("\n4 - Exibir Nós Em Ordem");
    printf("\n0 - Sair");
    printf("\nOpção escolhida: ");
    scanf("%d", &op);
    while (getchar() != '\n'); // Limpa o buffer
    return op;
}

int main() {
    TreeNode23 *arvore = NULL;

    // Define o tamanho máximo da memória
    int maximo = MEMORY_SIZE;

    printf("---------------------------------\n");
    printf("Alocacao de Memoria - Arvore 2-3\n");
    printf("---------------------------------\n");

    printf("Tamanho máximo da memória configurado como %d blocos.\n", MEMORY_SIZE - 1);

    int minimo = inicializar_blocos(&arvore, maximo);

    int op, quant_nos;
    do {
        op = menu();
        switch (op) {
            case 1:
                do {
                    printf("\nQuantidade de nós a serem alocados: ");
                    scanf("%d", &quant_nos);
                    while (getchar() != '\n'); 
                    if (quant_nos < minimo || quant_nos > maximo) {
                        printf("\nDigite um número entre %d e %d\n", minimo, maximo);
                    }
                } while (quant_nos < minimo || quant_nos > maximo);

                gerenciar_bloco_memoria(&arvore, quant_nos, LIVRE);
                break;

            case 2:
                do {
                    printf("\nQuantidade de nós a serem liberados: ");
                    scanf("%d", &quant_nos);
                    while (getchar() != '\n'); 
                    if (quant_nos < minimo || quant_nos > maximo) {
                        printf("\nDigite um número entre %d e %d\n", minimo, maximo);
                    }
                } while (quant_nos < minimo || quant_nos > maximo);

                gerenciar_bloco_memoria(&arvore, quant_nos, OCUPADO);
                break;
            case 3:
                printf("\nExibindo Enderecos\n");
                DisplayInfos(arvore);
                break;
            case 4:
                printf("\nExibindo árvore\n");
                TreeNode23_exibir_ordem(arvore);
                break;
            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpção inválida\n");
        }
    } while (op != 0);

    TreeNode23_desalocar(&arvore);
    return 0;
}

