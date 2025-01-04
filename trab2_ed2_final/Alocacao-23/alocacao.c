#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int initialize_memory_blocks(TreeNode23 **estrutura_memoria, int capacidade_maxima) {
    int current_state;

    // Solicitação do estado inicial
    do {
        printf("\n=== Configuração do Bloco Inicial ===\n");
        printf("Escolha o estado do infoBlock inicial:\n");
        printf("  [%d] -> Disponível\n", ALLOCATED_STATUS_FREE);
        printf("  [%d] -> Ocupado\n", STATUS_OCCUPIED);
        printf("Digite sua escolha: ");
        scanf("%d", &current_state);
        while (getchar() != '\n'); // Limpeza do buffer
        if (current_state != ALLOCATED_STATUS_FREE && current_state != STATUS_OCCUPIED) {
            printf("\n[Erro] Escolha inválida. Por favor, tente novamente.\n");
        }
    } while (current_state != ALLOCATED_STATUS_FREE && current_state != STATUS_OCCUPIED);

    Info infoBlock;
    // Solicitação do endereço inicial
    do {
        printf("\n>>> Insira o endereço inicial do infoBlock: ");
        scanf("%d", &infoBlock.num_start);
        while (getchar() != '\n'); // Limpeza do buffer
        if (infoBlock.num_start < 0 || infoBlock.num_start >= capacidade_maxima) {
            printf("\n[Erro] O endereço inicial deve estar entre 0 e %d.\n", capacidade_maxima - 1);
        }
    } while (infoBlock.num_start < 0 || infoBlock.num_start >= capacidade_maxima);

    int endereco_minimo = infoBlock.num_start;

    // Configuração dos blocos subsequentes
    do {
        infoBlock.status = current_state;

        do {
            printf("\n>>> Insira o endereço final do infoBlock (deve ser >= %d): ", infoBlock.num_start);
            scanf("%d", &infoBlock.num_end);
            while (getchar() != '\n'); // Limpeza do buffer
            if (infoBlock.num_end < infoBlock.num_start || infoBlock.num_end >= capacidade_maxima) {
                printf("\n[Erro] O endereço final deve estar entre %d e %d.\n", infoBlock.num_start, capacidade_maxima - 1);
            }
        } while (infoBlock.num_end < infoBlock.num_start || infoBlock.num_end >= capacidade_maxima);

        // Inserção do infoBlock na estrutura
        TreeNode23_insert(estrutura_memoria, infoBlock);

        // Atualização do próximo infoBlock
        infoBlock.num_start = infoBlock.num_end + 1;
        current_state = !current_state; // Alterna entre Livre e Ocupado
    } while (infoBlock.num_end < (capacidade_maxima - 1));

    printf("\n[Sucesso] Blocos inicializados com sucesso!\n");
    return endereco_minimo;
}


void merge_no(TreeNode23 **root, int *num_end, int limite, int valor_remover) {
    *num_end = limite;
    TreeNode23_remove(root, valor_remover);
}

TreeNode23 *find_smallest_block(TreeNode23 **estrutura, TreeNode23 *nodo, Info *info_dados, Info **menor_valor) {
    TreeNode23 *smallest_block, *pai_temporario;
    *menor_valor = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->info1.num_start != info_dados->num_start) {
            smallest_block = nodo;
        } else {
            smallest_block = Find_smallest_parent_node(*estrutura, info_dados->num_start);
        }

        if (smallest_block != NULL) {
            if (smallest_block->n_infos == 2 && smallest_block->info2.num_start < info_dados->num_start) {
                *menor_valor = &(smallest_block->info2);
            } else {
                *menor_valor = &(smallest_block->info1);
            }
        }
    } else if (nodo->info1.num_start == info_dados->num_start) {
        smallest_block = Find_largest_child_node(nodo->left, &pai_temporario, menor_valor);
    } else {
        smallest_block = Find_largest_child_node(nodo->center, &pai_temporario, menor_valor);
    }

    return smallest_block;
}

TreeNode23 *find_largest_block(TreeNode23 **estrutura, TreeNode23 *nodo, Info *info_dados, Info **maior_valor) {
    TreeNode23 *largest_block, *pai_temporario;
    *maior_valor = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->n_infos == 2 && nodo->info1.num_start == info_dados->num_start) {
            largest_block = nodo;
        } else {
            largest_block = Find_max_parent_node(*estrutura, info_dados->num_start);
        }

        if (largest_block != NULL) {
            if (largest_block->info1.num_start > info_dados->num_start) {
                *maior_valor = &(largest_block->info1);
            } else {
                *maior_valor = &(largest_block->info2);
            }
        }
    } else {
        if (nodo->info1.num_start == info_dados->num_start) {
            largest_block = Find_min_child_node(nodo->center, &pai_temporario);
        } else {
            largest_block = Find_min_child_node(nodo->right, &pai_temporario);
        }

        if (largest_block != NULL) {
            *maior_valor = &(largest_block->info1);
        }
    }

    return largest_block;
}

void adjust_node_in_tree(TreeNode23 **estrutura, TreeNode23 *nodo_atual, Info *info_atual, int tamanho) {
    TreeNode23 *smallest_block;
    Info *smallest_data = NULL;

    smallest_block = find_smallest_block(estrutura, nodo_atual, info_atual, &smallest_data);

    if (tamanho < (info_atual->num_end - info_atual->num_start + 1)) {
        if (smallest_block == NULL) {
            Info novo_dado;
            novo_dado.num_start = info_atual->num_start;
            novo_dado.num_end = info_atual->num_start + tamanho - 1;
            novo_dado.status = !(info_atual->status);

            info_atual->num_start += tamanho;
            TreeNode23_insert(estrutura, novo_dado);
        } else {
            smallest_data->num_end += tamanho;
            info_atual->num_start += tamanho;
        }
    } else {
        TreeNode23 *largest_block;
        Info *largest_item = NULL;

        largest_block = find_largest_block(estrutura, nodo_atual, info_atual, &largest_item);

        if (smallest_block == NULL && largest_block == NULL) {
            info_atual->status = !(info_atual->status);
        } else {
            if (smallest_block == NULL) {
                info_atual->status = !(info_atual->status);
                merge_no(estrutura, &(info_atual->num_end), largest_item->num_end, largest_item->num_start);
            } else if (largest_block == NULL) {
                merge_no(estrutura, &(smallest_data->num_end), info_atual->num_end, info_atual->num_start);
            } else {
                int remover_endereco = largest_item->num_start;
                merge_no(estrutura, &(smallest_data->num_end), largest_item->num_end, info_atual->num_start);
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
    Info *target_block = NULL;
    TreeNode23 *nodo_alvo = search_node(estrutura, tamanho, estado_atual, &target_block);

    if (target_block != NULL) {
        printf("\nDetalhes do infoBlock selecionado:\n");
        showNodeInfo23(*target_block);
        adjust_node_in_tree(estrutura, nodo_alvo, target_block, tamanho);
    } else {
        printf("\nNão foram encontrados blocos disponíveis com o critério especificado.\n");
    }

    return 1;
}


