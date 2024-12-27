#include <stdio.h>
#include <stdlib.h>
#include "notas.c"

int main() {
    arvore_notas *raiz = NULL;
    int opcao;
    arvore_notas *nota;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir Nota\n");
        printf("2. Remover Nota\n");
        printf("3. Buscar Nota\n");
        printf("4. Contar Nós\n");
        printf("5. Imprimir Notas\n");
        printf("6. Imprimir Notas por Semestre\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                nota = criar_nota();
                printf("Digite o código da disciplina: ");
                scanf("%d", &nota->codigo_disciplina);
                printf("Digite o semestre cursado (ex: 2024-1): ");
                scanf("%s", nota->semestre_cursado);
                printf("Digite a nota final: ");
                scanf("%f", &nota->nota_final);
                raiz = inserir_nota(raiz, nota);
                break;
            case 2:
                printf("Digite o código da disciplina a ser removido: ");
                scanf("%d", &opcao);
                raiz = remover_nota(raiz, opcao);
                break;
            case 3:
                printf("Digite o código da disciplina a ser buscado: ");
                scanf("%d", &opcao);
                nota = buscar_notas(raiz, opcao);
                if (nota != NULL) {
                    printf("Nota encontrada: %s, %.2f\n", nota->semestre_cursado, nota->nota_final);
                } else {
                    printf("Nota não encontrada.\n");
                }
                break;
            case 4:
                printf("Total de nós na árvore: %d\n", contar_nos(raiz));
                break;
            case 5:
                printf("Notas na árvore:\n");
                imprimir_notas(raiz);
                break;
            case 6:
                printf("Digite o semestre (ex: 2024-1): ");
                char semestre[10];
                scanf("%s", semestre);
                // Implemente uma função para imprimir notas por semestre, se necessário
                break;
            case 7:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 7);

    // Aqui você pode adicionar a liberação da memória da árvore antes de sair
    return 0;
}
