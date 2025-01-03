#include <stdio.h>
#include <stdlib.h>
#include "23.c"
#include "alocacao.c"

#define MEMORY_SIZE (1024 * 1024) // Tamanho total da memória (1MB por bloco)

int menu() {
    int op;
    printf("\nMENU");
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

int main_main() {
    Arvore23 *arvore = arvore23_criar();

    // Define o tamanho máximo da memória
    int maximo = MEMORY_SIZE;
    printf("\nTamanho máximo da memória configurado como %d blocos.\n", MEMORY_SIZE - 1);

    // Cadastrar os blocos iniciais na árvore
    int minimo = inicializar_blocos(&arvore, maximo);

    int op, quant_nos;
    do {
        op = menu();
        switch (op) {
            case 1:
                // Solicitar a quantidade de nós a alocar
                do {
                    printf("\nQuantidade de nós a serem alocados: ");
                    scanf("%d", &quant_nos);
                    while (getchar() != '\n'); // Limpa o buffer
                    if (quant_nos < minimo || quant_nos > maximo) {
                        printf("\nDigite um número entre %d e %d\n", minimo, maximo);
                    }
                } while (quant_nos < minimo || quant_nos > maximo);

                gerenciar_bloco_memoria(&arvore, quant_nos, LIVRE);
                break;

            case 2:
                // Solicitar a quantidade de nós a liberar
                do {
                    printf("\nQuantidade de nós a serem liberados: ");
                    scanf("%d", &quant_nos);
                    while (getchar() != '\n'); // Limpa o buffer
                    if (quant_nos < minimo || quant_nos > maximo) {
                        printf("\nDigite um número entre %d e %d\n", minimo, maximo);
                    }
                } while (quant_nos < minimo || quant_nos > maximo);

                gerenciar_bloco_memoria(&arvore, quant_nos, OCUPADO);
                break;
            case 3:
                printf("\nExibindo árvore [Em-Ordem]\n");
                DisplayInfos(arvore);
                break;
            case 4:
                printf("\nExibindo árvore [Pré-Ordem]\n");
                arvore23_exibir_ordem(arvore);
                break;
            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpção inválida\n");
        }
    } while (op != 0);

    arvore23_desalocar(&arvore);
    return 0;
}

int main() {
    main_main();
    return 0;
}
