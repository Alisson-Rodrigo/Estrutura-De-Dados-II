#include "arv-23.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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



Info *CreateInfo(int start, int end, int status)
{
    Info *info = (Info *)malloc(sizeof(Info));

    if (info != NULL)
    {
        info->start = start;
        info->end = end;
        info->status = status;
    }
    return info;
}

Memory *createNode(Info *information, Memory *leftChild, Memory *centerChild)
{
    Memory *node = (Memory *)malloc(sizeof(Memory));

    if (node != NULL)
    {
        node->info1 = information;
        node->left = leftChild;
        node->center = centerChild;
        node->numKeys = 1;
    }
    else
    {
        printf("Falha ao criar o Nó!\n");
    }

    return node;
}

int isLeaf(Memory *node)
{
    return node->left == NULL;
}

void AddInfo(Memory **node, Info *info, Memory *child)
{
    if ((*node)->numKeys == 1)
    {
        if (info->start > (*node)->info1->start)
        {
            (*node)->info2 = info;
            (*node)->right = child;
        }
        else
        {
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            (*node)->right = (*node)->center;
            (*node)->center = child;
        }
        (*node)->numKeys = 2;
    }
    else if ((*node)->numKeys == 2)
    {
        Memory *newNode = (Memory *)malloc(sizeof(Memory));
        newNode->numKeys = 1;

        if (info->start > (*node)->info2->start)
        {
            newNode->info1 = info;
            newNode->left = (*node)->right;
            newNode->center = child;
        }
        else if (info->start > (*node)->info1->start)
        {
            newNode->info1 = (*node)->info2;
            newNode->left = child;
            newNode->center = (*node)->right;
        }
        else
        {
            newNode->info1 = (*node)->info2;
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            newNode->left = (*node)->center;
            newNode->center = (*node)->right;
            (*node)->center = child;
        }
        (*node)->numKeys = 1;
    }
}

Split SplitNode(Memory **root, Info *info, Memory *child)
{
    Memory *largestNode = (Memory *)malloc(sizeof(Memory));
    Info *promote = NULL;
    Split itBroke;

    largestNode->numKeys = 1;

    if (info->start > (*root)->info2->start)
    {
        promote = (*root)->info2;
        (*root)->numKeys = 1;

        largestNode->info1 = info;
        largestNode->left = (*root)->center;
        largestNode->center = child;
    }
    else if (info->start > (*root)->info1->start)
    {
        promote = info;
        largestNode->info1 = (*root)->info2;
        largestNode->left = child;
        largestNode->center = (*root)->right;

        (*root)->numKeys = 1;
    }
    else
    {
        promote = (*root)->info1;
        (*root)->info1 = info;

        largestNode->info1 = (*root)->info2;
        largestNode->left = (*root)->center;
        largestNode->center = (*root)->right;

        (*root)->numKeys = 1;
    }

    itBroke.largestNode = largestNode;
    itBroke.promote = promote;

    return itBroke;
}

void Insert23(Memory **root, Memory *parent, Info **promote, int start, int end, int status, int *flag)
{
    if (*root == NULL)
    {
        Info *nova_info = CreateInfo(start, end, status);
        *root = createNode(nova_info, NULL, NULL);
        *flag = 1;
    }
    else if (isLeaf(*root))
    {
        *flag = 1;
        if ((*root)->numKeys < 2)
        {
            Info *nova_info = CreateInfo(start, end, status);
            AddInfo(root, nova_info, NULL);
        }
        else
        {
            Info *nova_info = CreateInfo(start, end, status);
            Split resultado = SplitNode(root, nova_info, NULL);

            if (!parent)
                *root = createNode(resultado.promote, *root, resultado.largestNode);
            else if (parent->numKeys < 2)
                AddInfo(&parent, resultado.promote, resultado.largestNode);
            else
            {
                resultado = SplitNode(&parent, resultado.promote, resultado.largestNode);
                parent = createNode(resultado.promote, parent, resultado.largestNode);
            }
        }
    }
    else
    {
        if (start < (*root)->info1->start)
            Insert23(&(*root)->left, *root, promote, start, end, status, flag);
        else if ((*root)->numKeys == 1 || start < (*root)->info2->start)
            Insert23(&(*root)->center, *root, promote, start, end, status, flag);
        else
            Insert23(&(*root)->right, *root, promote, start, end, status, flag);
    }
}

Memory *FindSpace(Memory *root, int requiredSpace)
{
    Memory *result = NULL;

    if (root != NULL)
    {
        if (isLeaf(root))
        {
            // Calcula o espaço disponível para os blocos
            int espacoDisponivel1 = root->info1->end - root->info1->start + 1;
            int espacoDisponivel2 = (root->numKeys == 2) ? root->info2->end - root->info2->start + 1 : 0;

            // Verifica se o primeiro bloco atende à necessidade
            if (root->info1->status == FREE && espacoDisponivel1 >= requiredSpace)
            {
                result = root;
            }
            // Verifica se o segundo bloco (se existir) atende à necessidade
            else if (root->numKeys == 2 && root->info2->status == FREE && espacoDisponivel2 >= requiredSpace)
            {
                result = root;
            }
        }
        else
        {
            // Busca recursivamente nos filhos
            result = FindSpace(root->left, requiredSpace);
            if (!result)
                result = FindSpace(root->center, requiredSpace);
            if (!result && root->numKeys == 2)
                result = FindSpace(root->right, requiredSpace);
        }
    }

    return result;
}


Memory *SourceSpace(Memory *root, int requiredSpace)
{
    return FindSpace(root, requiredSpace);
}

void DisplayInfos(Memory *root) {
    if (root == NULL) {
        return;  // Se o nó é nulo, retorna sem fazer nada.
    }

    // Percorre a subárvore esquerda primeiro.
    if (root->left) {
        DisplayInfos(root->left);
    }

    // Mostra as informações do nó atual.
    printf("Começo: %d | Fim: %d | Status: %d\n", root->info1->start, root->info1->end, root->info1->status);

    // Percorre a subárvore central.
    if (root->center) {
        DisplayInfos(root->center);
    }

    // Se houver uma segunda informação, mostra e percorre a subárvore direita.
    if (root->numKeys == 2) {
        printf("Começo: %d | Fim: %d | Status: %d\n", root->info2->start, root->info2->end, root->info2->status);
        if (root->right) {
            DisplayInfos(root->right);
        }
    }
}


void RemoveInfo(Memory **root, int start) {
    if (*root == NULL) {
        printf("Árvore vazia ou elemento não encontrado.\n");
        return;
    }

    // Verifica se o elemento está no nó atual
    if ((*root)->info1->start == start || ((*root)->numKeys == 2 && (*root)->info2->start == start)) {
        // Caso o nó seja uma folha
        if (isLeaf(*root)) {
            if ((*root)->info1->start == start) {
                // Remove info1
                if ((*root)->numKeys == 2) {
                    (*root)->info1 = (*root)->info2;
                    (*root)->numKeys = 1;
                } else {
                    free((*root)->info1);
                    free(*root);
                    *root = NULL;
                }
            } else if ((*root)->numKeys == 2 && (*root)->info2->start == start) {
                // Remove info2
                free((*root)->info2);
                (*root)->numKeys = 1;
            }
        } else {
            printf("Remoção em nós internos não implementada.\n");
        }
    } else {
        // Recursivamente busca e remove nos filhos
        if (start < (*root)->info1->start) {
            RemoveInfo(&(*root)->left, start);
        } else if ((*root)->numKeys == 1 || start < (*root)->info2->start) {
            RemoveInfo(&(*root)->center, start);
        } else {
            RemoveInfo(&(*root)->right, start);
        }
    }
}

void RebalanceTree(Memory **root, Memory *parent) {
    if (*root == NULL || (*root)->numKeys > 0) return;

    if (parent) {
        if (parent->left == *root) {
            if (parent->center->numKeys == 2) {
                (*root)->info1 = parent->info1;
                (*root)->center = parent->center->left;
                parent->info1 = parent->center->info1;
                parent->center->info1 = parent->center->info2;
                parent->center->left = parent->center->center;
                parent->center->numKeys = 1;
            } else {
                // Implementa fusão dos nós
            }
        }
        // Casos similares para centro e direito
    }
}

void AllocateBlocks(Memory **root, int size, int status) {
    Memory *space = SourceSpace(*root, size);
    if (space == NULL) {
        printf("Espaço não disponível.\n");
        return;
    }

    int remainingSize = space->info1->end - space->info1->start + 1 - size;
    if (remainingSize > 0) {
        Info *newInfo = CreateInfo(space->info1->start + size, space->info1->end, FREE);
        space->info1->end = space->info1->start + size - 1;
        AddInfo(&space, newInfo, NULL);
    }

    space->info1->status = status;
}

void FreeBlocks(Memory **root, int size) {
    AllocateBlocks(root, size, FREE);
}

void OccupyBlocks(Memory **root, int size) {
    AllocateBlocks(root, size, OCCUPIED);
}

