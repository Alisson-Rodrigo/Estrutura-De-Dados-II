#include <stdio.h>
#include <stdlib.h>
#include "23.c"
#include "remocao.c"
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
    TreeNode23 *treeRoot = NULL;

    // Define o tamanho máximo da memória
    int maximumMemorySize = MEMORY_SIZE;

    printf("---------------------------------\n");
    printf("Alocacao de Memoria - Arvore 2-3\n");
    printf("---------------------------------\n");

    printf("Tamanho máximo da memória configurado como %d blocos.\n", MEMORY_SIZE - 1);

    int minimumMemoryBlock = initialize_memory_blocks(&treeRoot, maximumMemorySize);

    int op, node_count;
    do {
        op = menu();
        switch (op) {
            case 1:
                do {
                    printf("\nQuantidade de nós a serem alocados: ");
                    scanf("%d", &node_count);
                    while (getchar() != '\n'); 
                    if (node_count < minimumMemoryBlock || node_count > maximumMemorySize) {
                        printf("\nDigite um número entre %d e %d\n", minimumMemoryBlock, maximumMemorySize);
                    }
                } while (node_count < minimumMemoryBlock || node_count > maximumMemorySize);

                manage_memory_block(&treeRoot, node_count, ALLOCATED_STATUS_FREE);
                break;

            case 2:
                do {
                    printf("\nQuantidade de nós a serem liberados: ");
                    scanf("%d", &node_count);
                    while (getchar() != '\n'); 
                    if (node_count < minimumMemoryBlock || node_count > maximumMemorySize) {
                        printf("\nDigite um número entre %d e %d\n", minimumMemoryBlock, maximumMemorySize);
                    }
                } while (node_count < minimumMemoryBlock || node_count > maximumMemorySize);

                manage_memory_block(&treeRoot, node_count, STATUS_OCCUPIED);
                break;
            case 3:
                printf("\nExibindo Enderecos\n");
                DisplayInfos(treeRoot);
                break;
            case 4:
                printf("\nExibindo árvore\n");
                TreeNode23_print_in_order(treeRoot);
                break;
            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpção inválida\n");
        }
    } while (op != 0);

    TreeNode23_cleanup(&treeRoot);
    return 0;
}

