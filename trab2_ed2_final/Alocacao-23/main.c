#include "arv-23.c"  // Assume que todas as funções e estruturas necessárias estão definidas aqui
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>  // Para suporte a caracteres especiais e acentuação

int main() {
    setlocale(LC_ALL, "");  // Configura o ambiente para suportar a língua local (acentuação)
    Memory *root = NULL;  // Raiz da árvore
    int option;
    int size, start;

    while (1) {
        printf("\nMenu de Gerenciamento de Memória:\n");
        printf("1. Cadastrar Nós\n");
        printf("2. Exibir Estado da Memória\n");
        printf("3. Alocar Blocos\n");
        printf("4. Liberar Blocos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                cadastrar_nos(&root);
                break;
            case 2:
                printf("\nEstado atual da memória:\n");
                if (root) {
                    DisplayInfos(root);
                } else {
                    printf("Nenhum nó cadastrado.\n");
                }
                break;
            case 3:
                printf("Informe a quantidade de blocos a alocar: ");
                scanf("%d", &size);
                alocar_blocos(&root, size);
                break;
            case 4:
                printf("Informe o endereço inicial do bloco a ser liberado: ");
                scanf("%d", &start);
                printf("Informe a quantidade de blocos a liberar: ");
                scanf("%d", &size);
                liberar_blocos(&root, start, size);
                break;
            case 5:
                printf("Saindo do programa...\n");
                // Liberar toda a memória alocada, se necessário
                // free_memory(&root);  // Supõe que existe uma função para liberar toda a árvore
                return 0;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }
    return 0;
}
