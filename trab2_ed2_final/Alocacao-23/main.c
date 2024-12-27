#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "arv-23.c" // Inclui a implementacao da arvore 2-3

#define MEMORY_SIZE (1024 * 1024) // Tamanho total da memoria (1MB por bloco)

void cadastrar_nos(Memory **root) {
    int start = 0, end, status;

    printf("Informe o status do primeiro no (1 para Livre, 0 para Ocupado): ");
    scanf("%d", &status);

    printf("Informe o endereco inicial do primeiro no: ");
    scanf("%d", &start);

    printf("Informe o endereco final do primeiro no (maximo %d): ", MEMORY_SIZE);
    scanf("%d", &end);

    if (end < start || end > MEMORY_SIZE) {
        printf("Endereco final invalido.\n");
        return;
    }

    Info *info = CreateInfo(start, end, status);
    *root = createNode(info, NULL, NULL);

    while (end < MEMORY_SIZE) {
        start = end + 1;
        printf("Informe o endereco final do proximo no (maximo %d para terminar): ", MEMORY_SIZE);
        scanf("%d", &end);

        if (end <= start || end > MEMORY_SIZE) {
            printf("Endereco final invalido. Tente novamente.\n");
            continue;
        }

        // Alternar status automaticamente
        status = (status == FREE) ? OCCUPIED : FREE;

        Info *newInfo = CreateInfo(start, end, status);
        int flag = 0;
        Insert23(root, NULL, NULL, start, end, status, &flag);

        if (end == MEMORY_SIZE) {
            printf("Cadastro concluido.\n");
            break;
        }
    }

    // Concatenar nos adjacentes, se necessario
    concatenar_nos(root);
}



void concatenar_nos(Memory **root) {
    if (!root || !(*root)) return;

    Memory *node = *root;

    if (isLeaf(node)) {
        if (node->numKeys == 2 && node->info1->status == node->info2->status) {
            // Unir os dois blocos
            node->info1->end = node->info2->end;
            free(node->info2);
            node->info2 = NULL;
            node->numKeys = 1;
        }
    } else {
        concatenar_nos(&node->left);
        concatenar_nos(&node->center);
        if (node->numKeys == 2) {
            concatenar_nos(&node->right);
        }
    }
}


void alocar_blocos(Memory **root, int quantidade_blocos) {
    if (!root || !(*root)) {
        printf("Erro: Nenhuma memoria cadastrada.\n");
        return;
    }

    // Encontrar um nó com espaço suficiente
    Memory *node = FindSpace(*root, quantidade_blocos);
    if (!node) {
        printf("Erro: Nao ha blocos livres suficientes para alocar.\n");
        return;
    }

    Info *targetInfo = NULL;

    // Identificar qual bloco contém espaço suficiente
    if (node->info1->status == FREE && (node->info1->end - node->info1->start + 1) >= quantidade_blocos) {
        targetInfo = node->info1;
    } else if (node->numKeys == 2 && node->info2->status == FREE &&
               (node->info2->end - node->info2->start + 1) >= quantidade_blocos) {
        targetInfo = node->info2;
    }

    if (!targetInfo) {
        printf("Erro interno: Bloco adequado nao encontrado.\n");
        return;
    }

    int espacoDisponivel = targetInfo->end - targetInfo->start + 1;
    int inicio = targetInfo->start;

    if (espacoDisponivel > quantidade_blocos) {
        // Dividir o bloco
        int novoInicio = inicio + quantidade_blocos;

        // Atualizar o bloco original com os blocos restantes
        targetInfo->start = novoInicio;

        // Criar novo bloco para os blocos alocados
        Info *alocadoInfo = CreateInfo(inicio, novoInicio - 1, OCCUPIED);
        int flag = 0;
        Insert23(root, NULL, NULL, alocadoInfo->start, alocadoInfo->end, OCCUPIED, &flag);

        if (!flag) {
            printf("Erro ao inserir bloco alocado na arvore.\n");
            free(alocadoInfo);
            return;
        }
    } else {
        // Marcar todo o bloco como ocupado
        targetInfo->status = OCCUPIED;
    }

    // Concatenar nós adjacentes após a alocação
    concatenar_nos(root);

    printf("Blocos alocados com sucesso: Inicio = %d, Quantidade = %d\n", inicio, quantidade_blocos);
}


void liberar_blocos(Memory **root, int inicio, int quantidade_blocos) {
    if (!root || !(*root)) {
        printf("Erro: Nenhuma memoria cadastrada.\n");
        return;
    }

    Memory *node = *root;
    Info *targetInfo = NULL;

    // Encontrar o bloco que contém o intervalo solicitado
    while (node) {
        if (isLeaf(node)) {
            if (inicio >= node->info1->start && inicio <= node->info1->end) {
                targetInfo = node->info1;
                break;
            } else if (node->numKeys == 2 && inicio >= node->info2->start && inicio <= node->info2->end) {
                targetInfo = node->info2;
                break;
            }
        } else {
            if (inicio < node->info1->start) {
                node = node->left;
            } else if (node->numKeys == 1 || inicio < node->info2->start) {
                node = node->center;
            } else {
                node = node->right;
            }
        }
    }

    if (!targetInfo) {
        printf("Erro: Bloco nao encontrado.\n");
        return;
    }

    int espacoDisponivel = targetInfo->end - targetInfo->start + 1;

    if (quantidade_blocos > espacoDisponivel) {
        printf("Erro: Quantidade de blocos a liberar excede o tamanho do bloco.\n");
        return;
    }

    if (quantidade_blocos == espacoDisponivel) {
        // Liberar o bloco inteiro
        targetInfo->status = FREE;
    } else {
        // Dividir o bloco
        int novoInicio = inicio + quantidade_blocos;
        Info *restante = CreateInfo(novoInicio, targetInfo->end, targetInfo->status);

        targetInfo->end = inicio - 1;
        targetInfo->status = FREE;

        // Inserir o bloco restante
        int flag = 0;
        Insert23(root, NULL, NULL, restante->start, restante->end, restante->status, &flag);
    }

    // Concatenar nós adjacentes após a liberação
    concatenar_nos(root);

    printf("Blocos liberados com sucesso: Inicio = %d, Quantidade = %d\n", inicio, quantidade_blocos);
}



// Funcao principal com menu interativo
int main()
{
    setlocale(LC_ALL, "PT-BR"); // Permite usar caracteres regionais
    Memory *root = NULL;   // Raiz da arvore
    int opcao;
    int quantidade_blocos;
    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar nos\n");
        printf("2. Exibir estado da memoria\n");
        printf("3. Alocar blocos\n");
        printf("4. Liberar blocos\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cadastrar_nos(&root);
            break;
        case 2:
            printf("\nEstado atual da memoria:\n");
            if (root)
            {
                DisplayInfos(root);
            }
            else
            {
                printf("Nenhum no cadastrado.\n");
            }
            break;
        case 3:
            printf("Informe a quantidade de blocos a alocar: ");
            scanf("%d", &quantidade_blocos);
            alocar_blocos(&root, quantidade_blocos);
            break;
        case 4:
            printf("Informe o endereco inicial do bloco a ser liberado: ");
            int inicio;
            scanf("%d", &inicio);
            printf("Informe a quantidade de blocos a liberar: ");
            
            scanf("%d", &quantidade_blocos);
            liberar_blocos(&root, inicio, quantidade_blocos);
            break;

        case 5:
            printf("Saindo do programa...\n");
            return 0;
        default:
            printf("Opcao invalida, tente novamente.\n");
        }
    }

    return 0;
}
