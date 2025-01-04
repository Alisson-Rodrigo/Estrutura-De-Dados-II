#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int initialize_memory_blocks(TreeNode23 **estrutura_memoria, int capacidade_maxima) {
    int estado_inicial;

    // Solicitação do estado inicial
    do {
        printf("\n=== Configuração do Bloco Inicial ===\n");
        printf("Escolha o estado do bloco inicial:\n");
        printf("  [%d] -> Disponível\n", ALLOCATED_STATUS_FREE);
        printf("  [%d] -> Ocupado\n", STATUS_OCCUPIED);
        printf("Digite sua escolha: ");
        scanf("%d", &estado_inicial);
        while (getchar() != '\n'); // Limpeza do buffer
        if (estado_inicial != ALLOCATED_STATUS_FREE && estado_inicial != STATUS_OCCUPIED) {
            printf("\n[Erro] Escolha inválida. Por favor, tente novamente.\n");
        }
    } while (estado_inicial != ALLOCATED_STATUS_FREE && estado_inicial != STATUS_OCCUPIED);

    Info bloco;
    // Solicitação do endereço inicial
    do {
        printf("\n>>> Insira o endereço inicial do bloco: ");
        scanf("%d", &bloco.num_start);
        while (getchar() != '\n'); // Limpeza do buffer
        if (bloco.num_start < 0 || bloco.num_start >= capacidade_maxima) {
            printf("\n[Erro] O endereço inicial deve estar entre 0 e %d.\n", capacidade_maxima - 1);
        }
    } while (bloco.num_start < 0 || bloco.num_start >= capacidade_maxima);

    int endereco_minimo = bloco.num_start;

    // Configuração dos blocos subsequentes
    do {
        bloco.status = estado_inicial;

        do {
            printf("\n>>> Insira o endereço final do bloco (deve ser >= %d): ", bloco.num_start);
            scanf("%d", &bloco.num_end);
            while (getchar() != '\n'); // Limpeza do buffer
            if (bloco.num_end < bloco.num_start || bloco.num_end >= capacidade_maxima) {
                printf("\n[Erro] O endereço final deve estar entre %d e %d.\n", bloco.num_start, capacidade_maxima - 1);
            }
        } while (bloco.num_end < bloco.num_start || bloco.num_end >= capacidade_maxima);

        // Inserção do bloco na estrutura
        TreeNode23_insert(estrutura_memoria, bloco);

        // Atualização do próximo bloco
        bloco.num_start = bloco.num_end + 1;
        estado_inicial = !estado_inicial; // Alterna entre Livre e Ocupado
    } while (bloco.num_end < (capacidade_maxima - 1));

    printf("\n[Sucesso] Blocos inicializados com sucesso!\n");
    return endereco_minimo;
}


void merge_no(TreeNode23 **root, int *num_end, int limite, int valor_remover) {
    *num_end = limite;
    TreeNode23_remove(root, valor_remover);
}

TreeNode23 *find_smallest_block(TreeNode23 **estrutura, TreeNode23 *nodo, Info *info_dados, Info **menor_valor) {
    TreeNode23 *menor_bloco, *pai_temporario;
    *menor_valor = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->info1.num_start != info_dados->num_start) {
            menor_bloco = nodo;
        } else {
            menor_bloco = Find_smallest_parent_node(*estrutura, info_dados->num_start);
        }

        if (menor_bloco != NULL) {
            if (menor_bloco->n_infos == 2 && menor_bloco->info2.num_start < info_dados->num_start) {
                *menor_valor = &(menor_bloco->info2);
            } else {
                *menor_valor = &(menor_bloco->info1);
            }
        }
    } else if (nodo->info1.num_start == info_dados->num_start) {
        menor_bloco = Find_largest_child_node(nodo->left, &pai_temporario, menor_valor);
    } else {
        menor_bloco = Find_largest_child_node(nodo->center, &pai_temporario, menor_valor);
    }

    return menor_bloco;
}

TreeNode23 *find_largest_block(TreeNode23 **estrutura, TreeNode23 *nodo, Info *info_dados, Info **maior_valor) {
    TreeNode23 *maior_bloco, *pai_temporario;
    *maior_valor = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->n_infos == 2 && nodo->info1.num_start == info_dados->num_start) {
            maior_bloco = nodo;
        } else {
            maior_bloco = Find_max_parent_node(*estrutura, info_dados->num_start);
        }

        if (maior_bloco != NULL) {
            if (maior_bloco->info1.num_start > info_dados->num_start) {
                *maior_valor = &(maior_bloco->info1);
            } else {
                *maior_valor = &(maior_bloco->info2);
            }
        }
    } else {
        if (nodo->info1.num_start == info_dados->num_start) {
            maior_bloco = Find_min_child_node(nodo->center, &pai_temporario);
        } else {
            maior_bloco = Find_min_child_node(nodo->right, &pai_temporario);
        }

        if (maior_bloco != NULL) {
            *maior_valor = &(maior_bloco->info1);
        }
    }

    return maior_bloco;
}

void adjust_node_in_tree(TreeNode23 **estrutura, TreeNode23 *nodo_atual, Info *info_atual, int tamanho) {
    TreeNode23 *menor_bloco;
    Info *menor_dado = NULL;

    menor_bloco = find_smallest_block(estrutura, nodo_atual, info_atual, &menor_dado);

    if (tamanho < (info_atual->num_end - info_atual->num_start + 1)) {
        if (menor_bloco == NULL) {
            Info novo_dado;
            novo_dado.num_start = info_atual->num_start;
            novo_dado.num_end = info_atual->num_start + tamanho - 1;
            novo_dado.status = !(info_atual->status);

            info_atual->num_start += tamanho;
            TreeNode23_insert(estrutura, novo_dado);
        } else {
            menor_dado->num_end += tamanho;
            info_atual->num_start += tamanho;
        }
    } else {
        TreeNode23 *maior_bloco;
        Info *maior_dado = NULL;

        maior_bloco = find_largest_block(estrutura, nodo_atual, info_atual, &maior_dado);

        if (menor_bloco == NULL && maior_bloco == NULL) {
            info_atual->status = !(info_atual->status);
        } else {
            if (menor_bloco == NULL) {
                info_atual->status = !(info_atual->status);
                merge_no(estrutura, &(info_atual->num_end), maior_dado->num_end, maior_dado->num_start);
            } else if (maior_bloco == NULL) {
                merge_no(estrutura, &(menor_dado->num_end), info_atual->num_end, info_atual->num_start);
            } else {
                int remover_endereco = maior_dado->num_start;
                merge_no(estrutura, &(menor_dado->num_end), maior_dado->num_end, info_atual->num_start);
                TreeNode23_remove(estrutura, remover_endereco);
            }
        }
    }
}

TreeNode23 *search_node(TreeNode23 **arvore, int quant, int status, Info **info_escolhido) {
    TreeNode23 *node;
    if (*arvore != NULL) {
        node = search_node(&((*arvore)->left), quant, status, info_escolhido);

        if (*info_escolhido == NULL) {
            if ((*arvore)->info1.status == status &&
                ((*arvore)->info1.num_end - (*arvore)->info1.num_start + 1) >= quant) {
                *info_escolhido = &((*arvore)->info1);
                node = *arvore;
            } else {
                node = search_node(&((*arvore)->center), quant, status, info_escolhido);
                if ((*arvore)->n_infos == 2) {
                    if ((*arvore)->info2.status == status &&
                        ((*arvore)->info2.num_end - (*arvore)->info2.num_start + 1) >= quant) {
                        *info_escolhido = &((*arvore)->info2);
                        node = *arvore;
                    } else if (*info_escolhido == NULL)
                        node = search_node(&((*arvore)->right), quant, status, info_escolhido);
                }
            }
        }
    } else
        *info_escolhido = NULL;

    return node;
}

int manage_memory_block(TreeNode23 **estrutura, int tamanho, int estado_atual) {
    Info *bloco_alvo = NULL;
    TreeNode23 *nodo_alvo = search_node(estrutura, tamanho, estado_atual, &bloco_alvo);

    if (bloco_alvo != NULL) {
        printf("\nDetalhes do bloco selecionado:\n");
        showNodeInfo23(*bloco_alvo);
        adjust_node_in_tree(estrutura, nodo_alvo, bloco_alvo, tamanho);
    } else {
        printf("\nNão foram encontrados blocos disponíveis com o critério especificado.\n");
    }

    return 1;
}


