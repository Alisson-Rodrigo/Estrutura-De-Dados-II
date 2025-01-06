#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.h"
#include "arvrb.h"
#include "unidade.h"
#define RED 1
#define BLACK 0

NodeData createInfo(char *portugueseWord, char *englishWord, int unit) {
    NodeData info;

    // Aloca e copia a palavra em português
    info.portugueseWord = malloc(strlen(portugueseWord) + 1);
    if (info.portugueseWord == NULL) {
        perror("Erro ao alocar memória para a palavra em português.");
        exit(EXIT_FAILURE);
    }
    strcpy(info.portugueseWord, portugueseWord);

    // Inicializa a árvore binária de palavras em inglês
    info.englishWordNode = NULL;

    // Insere a palavra em inglês e a unidade na árvore binária
    info.englishWordNode = insertEnglishWord(info.englishWordNode, englishWord, unit);

    return info;
}

int insertPortugueseWord(RedBlackTreePT **redBlackTree, char *portugueseWord, char *englishWord, int unit) {
    int isInserted = 0; 
    RedBlackTreePT *nonExistentNode = NULL;

    nonExistentNode = SearchWordInTree(redBlackTree, portugueseWord);

    if (nonExistentNode != NULL) {
        addEnglishTranslation(nonExistentNode, englishWord, unit);
        isInserted = 1;
    } else {
        NodeData newNodeData = createInfo(portugueseWord, englishWord, unit);
        if (insertRedBlackTree(redBlackTree, &newNodeData)) {
            isInserted = 1;
        }
    }

    return isInserted; 
}

RedBlackTreePT *createNode(NodeData *nodeData)
{
    RedBlackTreePT *newRedBlackTree = (RedBlackTreePT *)malloc(sizeof(RedBlackTreePT));
    newRedBlackTree->info = *nodeData;
    newRedBlackTree->color = 1;
    newRedBlackTree->left = NULL;
    newRedBlackTree->right = NULL;
    return newRedBlackTree;
}
int getNodeColor(RedBlackTreePT *rootNode)
{
    int color;

    if (rootNode == NULL)
        color = BLACK;
    else
        color = rootNode->color;

    return color;
}

void switch_node_color(RedBlackTreePT **rootNode)
{
    (*rootNode)->color = !(*rootNode)->color;
    if ((*rootNode)->left)
        (*rootNode)->left->color = !(*rootNode)->left->color;
    if ((*rootNode)->right)
        (*rootNode)->right->color = !(*rootNode)->right->color;
}

void rotate_right(RedBlackTreePT **rootNode)
{
    RedBlackTreePT *aux = (*rootNode)->left;
    (*rootNode)->left = aux->right;
    aux->right = *rootNode;
    aux->color = (*rootNode)->color;
    (*rootNode)->color = RED;
    (*rootNode) = aux;
}

void left_rotate(RedBlackTreePT **treeRoot)
{
    RedBlackTreePT *aux = (*treeRoot)->right;
    (*treeRoot)->right = aux->left;
    aux->left = *treeRoot;
    aux->color = (*treeRoot)->color;
    (*treeRoot)->color = RED;
    (*treeRoot) = aux;
}

void balanceTree(RedBlackTreePT **treeRoot)
{
    if (*treeRoot)
    {
        if (getNodeColor((*treeRoot)->right) == RED && getNodeColor((*treeRoot)->left) == BLACK)
            left_rotate(treeRoot);
        if (getNodeColor((*treeRoot)->left) == RED && getNodeColor((*treeRoot)->left->left) == RED)
            rotate_right(treeRoot);
        if (getNodeColor((*treeRoot)->left) == RED && getNodeColor((*treeRoot)->right) == RED)
            switch_node_color(treeRoot);
    }
}

int insertRedBlackNode(RedBlackTreePT **rootNode, NodeData *nodeData)
{

    int isInserted = 1;
    if (*rootNode == NULL)
    {
        *rootNode = createNode(nodeData);
    }
    else
    {
        if (strcmp(nodeData->portugueseWord, (*rootNode)->info.portugueseWord) < 0)
        {
            isInserted = insertRedBlackNode(&(*rootNode)->left, nodeData);
        }
        else
        {
            isInserted = insertRedBlackNode(&(*rootNode)->right, nodeData);
        }
        balanceTree(&(*rootNode));
    }

    return isInserted;
}

int insertRedBlackTree(RedBlackTreePT **rootNode, NodeData *nodeData)
{
    int isInserted = insertRedBlackNode(rootNode, nodeData);
    if (isInserted)
    {
        (*rootNode)->color = BLACK;
    }
    return isInserted;
}

void shiftLeftRed(RedBlackTreePT **rootNode)
{
    switch_node_color(rootNode);

    if ((*rootNode)->right && getNodeColor((*rootNode)->right->left) == RED)
    {
        rotate_right(&(*rootNode)->right);
        left_rotate((rootNode));
        switch_node_color(rootNode);
    }
}

void rotateRedRight(RedBlackTreePT **rootNode)
{
    switch_node_color(rootNode);

    if ((*rootNode)->left && getNodeColor((*rootNode)->left->left) == RED)
    {
        rotate_right(rootNode);
        switch_node_color(rootNode);
    }
}

void removeMinimum(RedBlackTreePT **rootNode)
{
    if (!((*rootNode)->left))
    {
        free(*rootNode);
        *rootNode = NULL;
    }
    else
    {
        if (getNodeColor((*rootNode)->left) == BLACK && getNodeColor((*rootNode)->left->left) == BLACK)
            shiftLeftRed(rootNode);

        removeMinimum(&(*rootNode)->left);
        balanceTree(rootNode);
    }
}

RedBlackTreePT *findMinimum(RedBlackTreePT *rootNode)
{
    RedBlackTreePT *minimumNode;
    minimumNode = rootNode;

    if (rootNode)
        if (rootNode->left)
            minimumNode = findMinimum(rootNode->left);

    return minimumNode;
}

int removeNodeFromRBTree(RedBlackTreePT **treeRoot, char *nodeValue)
{
    int isExist = 0;

    if (*treeRoot)
    {
        if (strcmp(nodeValue, (*treeRoot)->info.portugueseWord) < 0)
        {
            if ((*treeRoot)->left && getNodeColor((*treeRoot)->left) == BLACK && getNodeColor((*treeRoot)->left->left) == BLACK)
                shiftLeftRed(treeRoot);

            isExist = removeNodeFromRBTree(&(*treeRoot)->left, nodeValue);
        }
        else
        {
            if (getNodeColor((*treeRoot)->left) == RED)
                rotate_right(treeRoot);

            if (strcmp(nodeValue, (*treeRoot)->info.portugueseWord) == 0 && (*treeRoot)->right == NULL)
            {
                free(*treeRoot);
                *treeRoot = NULL;

                isExist = 1;
            }else{
                if ((*treeRoot)->right && getNodeColor((*treeRoot)->right) == BLACK && getNodeColor((*treeRoot)->right->left) == BLACK)
                rotateRedRight(treeRoot);

                if (strcmp(nodeValue, (*treeRoot)->info.portugueseWord) == 0)
                {
                    RedBlackTreePT *aux;
                    aux = findMinimum((*treeRoot)->right);
                    (*treeRoot)->info = aux->info;
                    removeMinimum(&(*treeRoot)->right);

                    isExist = 1;
                }
                else
                {
                    isExist = removeNodeFromRBTree(&(*treeRoot)->right, nodeValue);
                }
            }
        }
    }
    balanceTree(treeRoot);
    return isExist;
}

int removeRBTreeNode(RedBlackTreePT **rootNode, char *nodeValue)
{
    int isNodeRemoved = removeNodeFromRBTree(rootNode, nodeValue);
    if (isNodeRemoved)
    {
        (*rootNode)->color = BLACK;
    }
    return isNodeRemoved;
}


RedBlackTreePT *SearchWordInTree(RedBlackTreePT **redBlackTree, char *portugueseWord) {
    RedBlackTreePT *currentNode = NULL;

    if (*redBlackTree != NULL) {
        // Exibe a palavra com a qual está comparando
        printf("[DEBUG] Comparando '%s' com '%s'\n", portugueseWord, (*redBlackTree)->info.portugueseWord);

        if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) == 0) {
            printf("[DEBUG] Palavra encontrada: '%s'\n", (*redBlackTree)->info.portugueseWord);
            currentNode = *redBlackTree;
        } else if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) < 0) {
            printf("[DEBUG] Indo para a esquerda.\n");
            currentNode = SearchWordInTree(&(*redBlackTree)->left, portugueseWord);
        } else {
            printf("[DEBUG] Indo para a direita.\n");
            currentNode = SearchWordInTree(&(*redBlackTree)->right, portugueseWord);
        }
    } else {
        printf("[DEBUG] Chegou a um nó NULL.\n");
    }

    return currentNode;
}


void printWordsByUnit(RedBlackTreePT *redBlackTree, int unit) {

    if (redBlackTree) {
        // Percorre a subárvore esquerda
        printWordsByUnit(redBlackTree->left, unit);

        // Processa o nó atual
        BinaryTreeNode *englishWordNode = redBlackTree->info.englishWordNode;

        // Percorre a árvore binária associada
        while (englishWordNode != NULL) {
            Unit *currentUnit = englishWordNode->unitValues;

            // Verifica todas as unidades associadas à palavra
            while (currentUnit != NULL) {
                if (currentUnit->unitValue == unit) {
                    printf("%s: %s;\n", englishWordNode->englishWord, redBlackTree->info.portugueseWord); // Palavra e significado
                    break; // Sai da lista de unidades ao encontrar a unidade desejada
                }
                currentUnit = currentUnit->nextNode;
            }

            // Movimenta para o próximo nó da árvore binária
            if (englishWordNode->left != NULL) {
                englishWordNode = englishWordNode->left;
            } else if (englishWordNode->right != NULL) {
                englishWordNode = englishWordNode->right;
            } else {
                englishWordNode = NULL;
            }
        }

        // Percorre a subárvore direita
        printWordsByUnit(redBlackTree->right, unit);
    }


}




void printTranslations(BinaryTreeNode *currentNode, int unit, char *portugueseWord) {
    if (currentNode) {
        // Verifica se a unidade fornecida está presente na lista de unidades
        Unit *currentUnit = currentNode->unitValues;
        int foundUnit = 0;

        while (currentUnit != NULL) {
            if (currentUnit->unitValue == unit) {
                foundUnit = 1;
                break;
            }
            currentUnit = currentUnit->nextNode;
        }

        // Se a unidade foi encontrada, imprime a tradução
        if (foundUnit) {
            printf("Palavra em Português: %s\n", portugueseWord);
            printf("Palavra em inglês: %s\n", currentNode->englishWord);
        }

        // Percorre os subnós da árvore binária
        printTranslations(currentNode->left, unit, portugueseWord);
        printTranslations(currentNode->right, unit, portugueseWord);
    }
}

void showPortugueseTranslation(RedBlackTreePT **rootNode, char *portugueseWord) {
    if (*rootNode != NULL) {
        // Busca a palavra na árvore rubro-negra
        printf("[DEBUG] Buscando a palavra em português: '%s'\n", portugueseWord);
        RedBlackTreePT *resultado = SearchWordInTree(rootNode, portugueseWord);

        if (resultado != NULL) {
            printf("[DEBUG] Palavra encontrada na árvore rubro-negra.\n");
            printf("Traduções em inglês para a palavra '%s':\n", portugueseWord);

            // Percorre a árvore binária associada e imprime as traduções
            BinaryTreeNode *node = resultado->info.englishWordNode;
            if (node == NULL) {
                printf("Nenhuma tradução encontrada.\n");
            } else {
                printBinaryTree(node);
            }
        } else {
            printf("[DEBUG] Palavra '%s' não encontrada na árvore rubro-negra.\n", portugueseWord);
            printf("Nenhuma tradução encontrada para '%s'.\n", portugueseWord);
        }
    } else {
        printf("[DEBUG] A árvore rubro-negra está vazia.\n");
    }
}


void showRedBlackTree(RedBlackTreePT *rootNode)
{
    if (rootNode)
    {
        showRedBlackTree(rootNode->left);
        printf("Cor - %d\n", rootNode->color);
        printf("Palavra em Português - %s\n", rootNode->info.portugueseWord);
        printBinaryTree(rootNode->info.englishWordNode);
        printf("\n");
        showRedBlackTree(rootNode->right);
    }
}

RedBlackTreePT *SearchEnglishWordInRBTree(RedBlackTreePT *rootNode, char *englishWord, int unit) {
    RedBlackTreePT *rbTreeOutput = NULL; // Variável para armazenar o retorno

    if (rootNode != NULL) {
        // Verifica o nó atual da árvore vermelho-preto
        BinaryTreeNode *currentNode = rootNode->info.englishWordNode;
        while (currentNode != NULL) {
            printf("Verificando palavra: '%s' na unidade %d\n", currentNode->englishWord, unit);

            Unit *currentUnit = currentNode->unitValues;
            int unitFound = 0;

            // Verifica se a unidade existe na lista
            while (currentUnit != NULL) {
                if (currentUnit->unitValue == unit) {
                    unitFound = 1;
                    break;
                }
                currentUnit = currentUnit->nextNode;
            }

            if (unitFound && strcmp(currentNode->englishWord, englishWord) == 0) {
                printf("Palavra encontrada na árvore binária associada ao nó português: '%s'\n", rootNode->info.portugueseWord);
                rbTreeOutput = rootNode;
                break;
            }

            if (strcmp(englishWord, currentNode->englishWord) < 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        // Se não encontrou no nó atual, verifica à esquerda
        if (rbTreeOutput == NULL) {
            rbTreeOutput = SearchEnglishWordInRBTree(rootNode->left, englishWord, unit);
        }

        // Se não encontrou na esquerda, verifica à direita
        if (rbTreeOutput == NULL) {
            rbTreeOutput = SearchEnglishWordInRBTree(rootNode->right, englishWord, unit);
        }
    }

    return rbTreeOutput; // Retorna o resultado armazenado
}
void removeWordFromTree(RedBlackTreePT **node, char *wordToRemove, int *totalRemovido, RedBlackTreePT **rootNode) {
    if (*node != NULL) {
        // Percorre à esquerda
        removeWordFromTree(&(*node)->left, wordToRemove, totalRemovido, rootNode);

        // Remove a palavra da árvore binária associada
        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        int removed = removeEnglishWord(&binaryRoot, wordToRemove);
        if (removed) {
            (*totalRemovido)++;
            printf("[DEBUG] Palavra '%s' removida da árvore binária associada ao nó com palavra portuguesa: '%s'.\n",
                   wordToRemove, (*node)->info.portugueseWord);
        }

        // Verifica se a árvore binária ficou vazia após a remoção
        if (binaryRoot == NULL) {
            printf("[DEBUG] Árvore binária associada ao nó '%s' ficou vazia. Removendo nó da Árvore Rubro-Negra.\n",
                   (*node)->info.portugueseWord);
            removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
        } else {
            (*node)->info.englishWordNode = binaryRoot;
        }

        // Percorre à direita
        removeWordFromTree(&(*node)->right, wordToRemove, totalRemovido, rootNode);
    }
}
void removeWordByUnit(RedBlackTreePT **node, char *wordToRemove, int unit, int *removidos, RedBlackTreePT **rootNode) {
    if (*node == NULL) return; // Retorna se o nó for nulo

    // Remove da subárvore esquerda
    removeWordByUnit(&(*node)->left, wordToRemove, unit, removidos, rootNode);

    // Verifica e remove a palavra na árvore binária associada
    BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
    if (binaryRoot != NULL) {
        BinaryTreeNode *currentNode = binaryRoot;
        int removed = 0;

        // Busca e remove a palavra associada à unidade
        while (currentNode != NULL) {
            if (strcmp(currentNode->englishWord, wordToRemove) == 0) {
                Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                currentNode->unitValues = novaLista;

                // Remove o nó se não houver mais unidades associadas
                if (currentNode->unitValues == NULL) {
                    printf("[DEBUG] Palavra '%s' ficou sem unidades. Removendo nó.\n", wordToRemove);
                    removed = removeEnglishWord(&binaryRoot, wordToRemove);
                    if (removed) (*removidos)++;
                }
                break; // Sai do loop após tratar a palavra
            }

            // Continua percorrendo a árvore binária
            if (strcmp(wordToRemove, currentNode->englishWord) < 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        (*node)->info.englishWordNode = binaryRoot;

        // Remove o nó da Árvore Rubro-Negra se a árvore binária estiver vazia
        if (binaryRoot == NULL) {
            printf("[DEBUG] Árvore binária associada ao nó '%s' ficou vazia. Removendo nó da Árvore Rubro-Negra.\n",
                   (*node)->info.portugueseWord);
            removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
            return; // Sai após remover o nó
        }
    }

    // Remove da subárvore direita
    removeWordByUnit(&(*node)->right, wordToRemove, unit, removidos, rootNode);
}

void exibir_arvorebianria_dada_palavra_portuguesa(RedBlackTreePT *rootNode, char *portugueseWord)
{
    if (rootNode)
    {
        exibir_arvorebianria_dada_palavra_portuguesa(rootNode->left, portugueseWord);
        if (strcmp(rootNode->info.portugueseWord, portugueseWord) == 0)
        {
            exibir_arvorebinaria(rootNode->info.englishWordNode);
        }
        exibir_arvorebianria_dada_palavra_portuguesa(rootNode->right, portugueseWord);
    }
}

void removerPalavraPortuguesaPorUnidade(RedBlackTreePT **redBlackTreeRoot, char *portugueseWord, int unidade) {
    int isFound = 0; // Variável para armazenar o estado da busca
    int isNodeRemoved = 0; // Variável para acompanhar a remoção na BST

    if (*redBlackTreeRoot == NULL) {
        printf("A árvore está vazia.\n");
        isFound = 0;
    } else {
        printf("[DEBUG] Iniciando remoção da palavra '%s' na unidade %d.\n", portugueseWord, unidade);

        // Localiza o nó correspondente à palavra em português na RBT
        RedBlackTreePT *noRBT = SearchWordInTree(redBlackTreeRoot, portugueseWord);
        if (noRBT == NULL) {
            printf("Palavra '%s' não encontrada na árvore rubro-negra.\n", portugueseWord);
            isFound = 0;
        } else {
            isFound = 1;
            printf("[DEBUG] Palavra '%s' encontrada na RBT. Iniciando remoção na BST.\n", portugueseWord);

            // Remove a palavra em inglês associada à unidade na BST
            BinaryTreeNode *raizBST = noRBT->info.englishWordNode;
            if (raizBST != NULL) {
                BinaryTreeNode *currentNode = raizBST;

                while (currentNode != NULL) {
                    Unit *novaLista = remove_unit(currentNode->unitValues, unidade);
                    currentNode->unitValues = novaLista;

                    // Se a lista de unidades ficar vazia, remova a palavra
                    if (currentNode->unitValues == NULL) {
                        printf("[DEBUG] Palavra '%s' ficou sem unidades. Removendo da BST.\n", currentNode->englishWord);
                        isNodeRemoved = removeEnglishWord(&raizBST, currentNode->englishWord);
                        break; // Palavra tratada, sair do loop
                    }

                    // Navega na BST
                    if (strcmp(portugueseWord, currentNode->englishWord) < 0) {
                        currentNode = currentNode->left;
                    } else {
                        currentNode = currentNode->right;
                    }
                }

                // Atualiza a raiz da BST no nó da RBT
                noRBT->info.englishWordNode = raizBST;

                // Se a BST ficar vazia após a remoção, remova o nó correspondente na RBT
                if (raizBST == NULL) {
                    printf("[DEBUG] A BST ficou vazia. Removendo a palavra '%s' da RBT.\n", portugueseWord);
                    removeRBTreeNode(redBlackTreeRoot, portugueseWord);
                } else if (!isNodeRemoved) {
                    printf("[DEBUG] Nenhuma palavra em inglês encontrada na BST para unidade %d.\n", unidade);
                }
            } else {
                printf("[DEBUG] A BST associada à palavra '%s' está vazia.\n", portugueseWord);
            }
        }
    }

    // Retorno unificado com mensagens baseadas nos estados
    if (!isFound) {
        printf("[DEBUG] Operação finalizada: Palavra não encontrada na RBT.\n");
    } else if (isNodeRemoved) {
        printf("[DEBUG] Operação finalizada: Palavra e unidade removidas com sucesso.\n");
    } else {
        printf("[DEBUG] Operação finalizada: Nenhuma palavra removida.\n");
    }
}
