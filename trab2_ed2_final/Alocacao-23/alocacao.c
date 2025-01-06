#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int initialize_memory_blocks(TreeNode23 **memory_structure, int max_capacity) {
    int current_state;

    do {
        printf("\n=== Configuração do Bloco Inicial ===\n");
        printf("Escolha o estado do infoBlock inicial:\n");
        printf("%d -> Disponível\n", ALLOCATED_STATUS_FREE);
        printf("%d -> Ocupado\n", STATUS_OCCUPIED);
        printf("Digite sua escolha: ");
        scanf("%d", &current_state);
        while (getchar() != '\n'); 
        if (current_state != ALLOCATED_STATUS_FREE && current_state != STATUS_OCCUPIED) {
            printf("\n[Erro] Escolha inválida. Por favor, tente novamente.\n");
        }
    } while (current_state != ALLOCATED_STATUS_FREE && current_state != STATUS_OCCUPIED);

    Info infoBlock;
    do {
        printf("\n>>> Insira o endereço inicial do infoBlock: ");
        scanf("%d", &infoBlock.num_start);
        while (getchar() != '\n'); 
        if (infoBlock.num_start < 0 || infoBlock.num_start >= max_capacity) {
            printf("\n[Erro] O endereço inicial deve estar entre 0 e %d.\n", max_capacity - 1);
        }
    } while (infoBlock.num_start < 0 || infoBlock.num_start >= max_capacity);

    int endereco_minimo = infoBlock.num_start;

    do {
        infoBlock.status = current_state;

        do {
            printf("\n>>> Insira o endereço final do infoBlock (deve ser >= %d): ", infoBlock.num_start);
            scanf("%d", &infoBlock.num_end);
            while (getchar() != '\n'); 
            if (infoBlock.num_end < infoBlock.num_start || infoBlock.num_end >= max_capacity) {
                printf("\n[Erro] O endereço final deve estar entre %d e %d.\n", infoBlock.num_start, max_capacity - 1);
            }
        } while (infoBlock.num_end < infoBlock.num_start || infoBlock.num_end >= max_capacity);

        TreeNode23_insert(memory_structure, infoBlock);

        infoBlock.num_start = infoBlock.num_end + 1;
        current_state = !current_state; 
    } while (infoBlock.num_end < (max_capacity - 1));

    printf("\n[Sucesso] Blocos inicializados com sucesso!\n");
    return endereco_minimo;
}


void merge_no(TreeNode23 **root, int *num_end, int limite, int valor_remover) {
    *num_end = limite;
    TreeNode23_remove(root, valor_remover);
}

TreeNode23 *find_smallest_block(TreeNode23 **treeRoot, TreeNode23 *nodo, Info *data_info, Info **minimum_value) {
    TreeNode23 *smallest_block, *temporary_parent;
    *minimum_value = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->info1.num_start != data_info->num_start) {
            smallest_block = nodo;
        } else {
            smallest_block = Find_smallest_parent_node(*treeRoot, data_info->num_start);
        }

        if (smallest_block != NULL) {
            if (smallest_block->n_infos == 2 && smallest_block->info2.num_start < data_info->num_start) {
                *minimum_value = &(smallest_block->info2);
            } else {
                *minimum_value = &(smallest_block->info1);
            }
        }
    } else if (nodo->info1.num_start == data_info->num_start) {
        smallest_block = Find_largest_child_node(nodo->left, &temporary_parent, minimum_value);
    } else {
        smallest_block = Find_largest_child_node(nodo->center, &temporary_parent, minimum_value);
    }

    return smallest_block;
}

TreeNode23 *find_largest_block(TreeNode23 **treeRoot, TreeNode23 *nodo, Info *data_info, Info **largest_value_pointer) {
    TreeNode23 *largest_block, *temporary_parent;
    *largest_value_pointer = NULL;

    if (isLeaf(*nodo)) {
        if (nodo->n_infos == 2 && nodo->info1.num_start == data_info->num_start) {
            largest_block = nodo;
        } else {
            largest_block = Find_max_parent_node(*treeRoot, data_info->num_start);
        }

        if (largest_block != NULL) {
            if (largest_block->info1.num_start > data_info->num_start) {
                *largest_value_pointer = &(largest_block->info1);
            } else {
                *largest_value_pointer = &(largest_block->info2);
            }
        }
    } else {
        if (nodo->info1.num_start == data_info->num_start) {
            largest_block = Find_min_child_node(nodo->center, &temporary_parent);
        } else {
            largest_block = Find_min_child_node(nodo->right, &temporary_parent);
        }

        if (largest_block != NULL) {
            *largest_value_pointer = &(largest_block->info1);
        }
    }

    return largest_block;
}

void adjust_node_in_tree(TreeNode23 **treeRoot, TreeNode23 *current_node, Info *current_info, int size) {
    TreeNode23 *smallest_block;
    Info *smallest_data = NULL;

    smallest_block = find_smallest_block(treeRoot, current_node, current_info, &smallest_data);

    if (size < (current_info->num_end - current_info->num_start + 1)) {
        if (smallest_block == NULL) {
            Info novo_dado;
            novo_dado.num_start = current_info->num_start;
            novo_dado.num_end = current_info->num_start + size - 1;
            novo_dado.status = !(current_info->status);

            current_info->num_start += size;
            TreeNode23_insert(treeRoot, novo_dado);
        } else {
            smallest_data->num_end += size;
            current_info->num_start += size;
        }
    } else {
        TreeNode23 *largest_block;
        Info *largest_item = NULL;

        largest_block = find_largest_block(treeRoot, current_node, current_info, &largest_item);

        if (smallest_block == NULL && largest_block == NULL) {
            current_info->status = !(current_info->status);
        } else {
            if (smallest_block == NULL) {
                current_info->status = !(current_info->status);
                merge_no(treeRoot, &(current_info->num_end), largest_item->num_end, largest_item->num_start);
            } else if (largest_block == NULL) {
                merge_no(treeRoot, &(smallest_data->num_end), current_info->num_end, current_info->num_start);
            } else {
                int remover_endereco = largest_item->num_start;
                merge_no(treeRoot, &(smallest_data->num_end), largest_item->num_end, current_info->num_start);
                TreeNode23_remove(treeRoot, remover_endereco);
            }
        }
    }
}

TreeNode23 *search_node(TreeNode23 **treeRoot, int quantity, int status, Info **selected_info) {
    TreeNode23 *node;
    if (*treeRoot != NULL) {
        node = search_node(&((*treeRoot)->left), quantity, status, selected_info);

        if (*selected_info == NULL) {
            if ((*treeRoot)->info1.status == status &&
                ((*treeRoot)->info1.num_end - (*treeRoot)->info1.num_start + 1) >= quantity) {
                *selected_info = &((*treeRoot)->info1);
                node = *treeRoot;
            } else {
                node = search_node(&((*treeRoot)->center), quantity, status, selected_info);
                if ((*treeRoot)->n_infos == 2) {
                    if ((*treeRoot)->info2.status == status &&
                        ((*treeRoot)->info2.num_end - (*treeRoot)->info2.num_start + 1) >= quantity) {
                        *selected_info = &((*treeRoot)->info2);
                        node = *treeRoot;
                    } else if (*selected_info == NULL)
                        node = search_node(&((*treeRoot)->right), quantity, status, selected_info);
                }
            }
        }
    } else
        *selected_info = NULL;

    return node;
}

int manage_memory_block(TreeNode23 **treeRoot, int size, int estado_atual) {
    Info *target_block = NULL;
    TreeNode23 *nodo_alvo = search_node(treeRoot, size, estado_atual, &target_block);

    if (target_block != NULL) {
        printf("\nDetalhes do infoBlock selecionado:\n");
        showNodeInfo23(*target_block);
        adjust_node_in_tree(treeRoot, nodo_alvo, target_block, size);
    } else {
        printf("\nNão foram encontrados blocos disponíveis com o critério especificado.\n");
    }

    return 1;
}


