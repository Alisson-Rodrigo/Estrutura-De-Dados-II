#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.h"
#include "arvrb.h"
#include "unidade.h"
#define RED 1
#define BLACK 0

NodeData createInfo(char *portugueseWord, char *englishWord, int unit)
{
    NodeData info;

    // Aloca e copia a palavra em português
    info.portugueseWord = malloc(strlen(portugueseWord) + 1);
    if (info.portugueseWord == NULL)
    {
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

int insertPortugueseWord(RedBlackTreePT **redBlackTree, char *portugueseWord, char *englishWord, int unit)
{
    int isInserted = 0;
    RedBlackTreePT *nonExistentNode = NULL;

    nonExistentNode = SearchWordInTree(redBlackTree, portugueseWord);

    if (nonExistentNode != NULL)
    {
        addEnglishTranslation(nonExistentNode, englishWord, unit);
        isInserted = 1;
    }
    else
    {
        NodeData newNodeData = createInfo(portugueseWord, englishWord, unit);
        if (insertRedBlackTree(redBlackTree, &newNodeData))
        {
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
            }
            else
            {
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

RedBlackTreePT *SearchWordInTree(RedBlackTreePT **redBlackTree, char *portugueseWord)
{
    RedBlackTreePT *currentNode = NULL;

    if (*redBlackTree != NULL)
    {
        if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) == 0)
        {
            currentNode = *redBlackTree;
        }
        else if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) < 0)
        {
            currentNode = SearchWordInTree(&(*redBlackTree)->left, portugueseWord);
        }
        else
        {
            currentNode = SearchWordInTree(&(*redBlackTree)->right, portugueseWord);
        }
    }

    return currentNode;
}



void printWordsByUnit(RedBlackTreePT *redBlackTree, int unit)
{
    static int printedUnit = 0; // Controla se a unidade já foi impressa

    if (redBlackTree)
    {
        // Percorre a subárvore esquerda
        printWordsByUnit(redBlackTree->left, unit);

        // Processa o nó atual
        BinaryTreeNode *englishWordNode = redBlackTree->info.englishWordNode;

        // Percorre a árvore binária associada
        while (englishWordNode != NULL)
        {
            Unit *currentUnit = englishWordNode->unitValues;

            // Verifica todas as unidades associadas à palavra
            while (currentUnit != NULL)
            {
                if (currentUnit->unitValue == unit)
                {
                    // Imprime a unidade uma única vez
                    if (!printedUnit)
                    {
                        printf("%% Unidade %d\n", unit);
                        printedUnit = 1; // Marca que a unidade foi impressa
                    }

                    // Imprime a palavra
                    printf("%s: %s;\n", redBlackTree->info.portugueseWord, englishWordNode->englishWord);
                    break; // Sai do loop interno se a unidade for encontrada
                }
                currentUnit = currentUnit->nextNode;
            }

            // Movimenta para o próximo nó da árvore binária
            if (englishWordNode->left != NULL)
            {
                englishWordNode = englishWordNode->left;
            }
            else if (englishWordNode->right != NULL)
            {
                englishWordNode = englishWordNode->right;
            }
            else
            {
                englishWordNode = NULL;
            }
        }

        // Percorre a subárvore direita
        printWordsByUnit(redBlackTree->right, unit);
    }

    // Reseta o estado da variável estática para a próxima chamada
    if (redBlackTree == NULL)
    {
        printedUnit = 0;
    }
}


void printTranslations(BinaryTreeNode *currentNode, int unit, char *portugueseWord)
{
    if (currentNode)
    {
        Unit *currentUnit = currentNode->unitValues;
        int foundUnit = 0;

        while (currentUnit != NULL)
        {
            if (currentUnit->unitValue == unit)
            {
                foundUnit = 1;
                break;
            }
            currentUnit = currentUnit->nextNode;
        }

        if (foundUnit)
        {
            printf("Palavra em Português: %s\n", portugueseWord);
            printf("Palavra em inglês: %s\n", currentNode->englishWord);
        }

        printTranslations(currentNode->left, unit, portugueseWord);
        printTranslations(currentNode->right, unit, portugueseWord);
    }
}

void showPortugueseTranslation(RedBlackTreePT **rootNode, char *portugueseWord)
{
    if (*rootNode != NULL)
    {
        // Busca a palavra na arvore rubro-negra
        RedBlackTreePT *resultado = SearchWordInTree(rootNode, portugueseWord);

        if (resultado != NULL)
        {
            printf("\n============================\n");
            printf(" Traducao da palavra: %s\n", portugueseWord);
            printf("============================\n");

            // Percorre a arvore binaria associada e imprime as traducoes
            BinaryTreeNode *node = resultado->info.englishWordNode;
            if (node == NULL)
            {
                printf(" Nenhuma traducao encontrada.\n");
            }
            else
            {
                printf(" Traducoes encontradas:\n");
                printBinaryTree(node);
            }
            printf("============================\n");
        }
        else
        {
            printf("\n============================\n");
            printf(" Nenhuma traducao encontrada para: %s\n", portugueseWord);
            printf("============================\n");
        }
    }
    else
    {
        printf("\n============================\n");
        printf(" A arvore rubro-negra esta vazia.\n");
        printf("============================\n");
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

RedBlackTreePT *SearchEnglishWordInRBTree(RedBlackTreePT *rootNode, char *englishWord, int unit)
{
    RedBlackTreePT *rbTreeOutput = NULL; 

    if (rootNode != NULL)
    {
        BinaryTreeNode *currentNode = rootNode->info.englishWordNode;
        while (currentNode != NULL)
        {
            printf("Verificando palavra: '%s' na unidade %d\n", currentNode->englishWord, unit);

            Unit *currentUnit = currentNode->unitValues;
            int unitFound = 0;

            while (currentUnit != NULL)
            {
                if (currentUnit->unitValue == unit)
                {
                    unitFound = 1;
                    break;
                }
                currentUnit = currentUnit->nextNode;
            }

            if (unitFound && strcmp(currentNode->englishWord, englishWord) == 0)
            {
                printf("Palavra encontrada na árvore binária associada ao nó português: '%s'\n", rootNode->info.portugueseWord);
                rbTreeOutput = rootNode;
                break;
            }

            if (strcmp(englishWord, currentNode->englishWord) < 0)
            {
                currentNode = currentNode->left;
            }
            else
            {
                currentNode = currentNode->right;
            }
        }

        if (rbTreeOutput == NULL)
        {
            rbTreeOutput = SearchEnglishWordInRBTree(rootNode->left, englishWord, unit);
        }


        if (rbTreeOutput == NULL)
        {
            rbTreeOutput = SearchEnglishWordInRBTree(rootNode->right, englishWord, unit);
        }
    }

    return rbTreeOutput; 
}
void removeWordFromTree(RedBlackTreePT **node, char *wordToRemove, int *totalRemovido, RedBlackTreePT **rootNode)
{
    if (*node != NULL)
    {
        // Percorre à esquerda
        removeWordFromTree(&(*node)->left, wordToRemove, totalRemovido, rootNode);

        // Remove a palavra da árvore binária associada
        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        int removed = removeEnglishWord(&binaryRoot, wordToRemove);
        if (removed)
        {
            (*totalRemovido)++;
        }

        // Verifica se a árvore binária ficou vazia após a remoção
        if (binaryRoot == NULL)
        {
            removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
        }
        else
        {
            (*node)->info.englishWordNode = binaryRoot;
        }

        // Percorre à direita
        removeWordFromTree(&(*node)->right, wordToRemove, totalRemovido, rootNode);
    }
}


void removeWordByUnit(RedBlackTreePT **node, char *wordToRemove, int unit, int *removidos, RedBlackTreePT **rootNode)
{
    if (*node != NULL)
    {
        // Remove da subárvore esquerda
        removeWordByUnit(&(*node)->left, wordToRemove, unit, removidos, rootNode);

        // Verifica e remove a palavra na árvore binária associada
        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        if (binaryRoot != NULL)
        {
            BinaryTreeNode *currentNode = binaryRoot;
            int removed = 0;
            int encontrou = 0; // Variável para indicar se a palavra foi tratada

            // Busca e remove a palavra associada à unidade
            while (currentNode != NULL && !encontrou)
            {
                if (strcmp(currentNode->englishWord, wordToRemove) == 0)
                {
                    Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                    currentNode->unitValues = novaLista;

                    // Remove o nó se não houver mais unidades associadas
                    if (currentNode->unitValues == NULL)
                    {
                        removed = removeEnglishWord(&binaryRoot, wordToRemove);
                        if (removed)
                            (*removidos)++;
                    }
                    encontrou = 1; // Marca que a palavra foi tratada
                }
                else if (strcmp(wordToRemove, currentNode->englishWord) < 0)
                {
                    currentNode = currentNode->left; // Continua para a esquerda
                }
                else
                {
                    currentNode = currentNode->right; // Continua para a direita
                }
            }

            (*node)->info.englishWordNode = binaryRoot;

            // Remove o nó da Árvore Rubro-Negra se a árvore binária estiver vazia
            if (binaryRoot == NULL)
            {
                removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
            }
        }

        // Remove da subárvore direita
        removeWordByUnit(&(*node)->right, wordToRemove, unit, removidos, rootNode);
    }
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

void removerPalavraPortuguesaPorUnidade(RedBlackTreePT **redBlackTreeRoot, char *portugueseWord, int unidade)
{
    int isFound = 0;       // Variável para armazenar o estado da busca
    int isNodeRemoved = 0; // Variável para acompanhar a remoção na BST

    if (*redBlackTreeRoot == NULL)
    {
        printf("A árvore está vazia.\n");
        isFound = 0;
    }
    else
    {
        // Localiza o nó correspondente à palavra em português na RBT
        RedBlackTreePT *noRBT = SearchWordInTree(redBlackTreeRoot, portugueseWord);
        if (noRBT == NULL)
        {
            isFound = 0;
        }
        else
        {
            isFound = 1;

            // Remove a palavra em inglês associada à unidade na BST
            BinaryTreeNode *raizBST = noRBT->info.englishWordNode;
            if (raizBST != NULL)
            {
                BinaryTreeNode *currentNode = raizBST;
                int encontrou = 0; // Controle para saída do loop

                while (currentNode != NULL && !encontrou)
                {
                    Unit *novaLista = remove_unit(currentNode->unitValues, unidade);
                    currentNode->unitValues = novaLista;

                    // Se a lista de unidades ficar vazia, remova a palavra
                    if (currentNode->unitValues == NULL)
                    {
                        isNodeRemoved = removeEnglishWord(&raizBST, currentNode->englishWord);
                        encontrou = 1; // Define a saída do loop
                    }
                    else if (strcmp(portugueseWord, currentNode->englishWord) < 0)
                    {
                        currentNode = currentNode->left; // Navega para a esquerda
                    }
                    else
                    {
                        currentNode = currentNode->right; // Navega para a direita
                    }
                }

                // Atualiza a raiz da BST no nó da RBT
                noRBT->info.englishWordNode = raizBST;

                // Se a BST ficar vazia após a remoção, remova o nó correspondente na RBT
                if (raizBST == NULL)
                {
                    removeRBTreeNode(redBlackTreeRoot, portugueseWord);
                }
            }
        }
    }

    // Retorno unificado com mensagens baseadas nos estados
    if (!isFound)
    {
        printf("Operação finalizada: Palavra não encontrada.\n");
    }
    else if (isNodeRemoved)
    {
        printf("Operação finalizada: Palavra e unidade removidas com sucesso.\n");
    }
    else
    {
        printf("Operação finalizada: Nenhuma palavra removida.\n");
    }
}
