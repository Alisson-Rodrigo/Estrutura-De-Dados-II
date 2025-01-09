#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingles.h"
#include "portugues.h"
#include "unidade.h"
#define RED 1
#define BLACK 0

NodeData createInfo(char *portugueseWord, char *englishWord, int unit) {
    NodeData info;

  
    info.portugueseWord = malloc(strlen(portugueseWord) + 1);
    if (info.portugueseWord == NULL) {
        perror("Erro ao alocar memória para a palavra em português.");
        exit(EXIT_FAILURE);
    }
    strcpy(info.portugueseWord, portugueseWord);


    info.englishWordNode = NULL;

   
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

        if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) == 0) {
            currentNode = *redBlackTree;
        } else if (strcmp(portugueseWord, (*redBlackTree)->info.portugueseWord) < 0) {
 
            currentNode = SearchWordInTree(&(*redBlackTree)->left, portugueseWord);
        } else {

            currentNode = SearchWordInTree(&(*redBlackTree)->right, portugueseWord);
        }
    } else {

    }

    return currentNode;
}


void printWordsByUnit(RedBlackTreePT *redBlackTree, int unit) {
    if (redBlackTree) {
      
        printWordsByUnit(redBlackTree->left, unit);

  
        BinaryTreeNode *englishWordNode = redBlackTree->info.englishWordNode;

   
        while (englishWordNode != NULL) {
            Unit *currentUnit = englishWordNode->unitValues;

        
            int foundUnit = 0;
            while (currentUnit != NULL && !foundUnit) {
                if (currentUnit->unitValue == unit) {
                    printf("%s: %s;\n", redBlackTree->info.portugueseWord, englishWordNode->englishWord);
                    foundUnit = 1; 
                }
                currentUnit = currentUnit->nextNode;
            }


            if (englishWordNode->left != NULL) {
                englishWordNode = englishWordNode->left;
            } else if (englishWordNode->right != NULL) {
                englishWordNode = englishWordNode->right;
            } else {
                englishWordNode = NULL;
            }
        }


        printWordsByUnit(redBlackTree->right, unit);
    }
}





void printTranslations(BinaryTreeNode *currentNode, int unit, char *portugueseWord) {
    if (currentNode) {
        Unit *currentUnit = currentNode->unitValues;
        int foundUnit = 0;

        
        while (currentUnit != NULL && !foundUnit) {
            if (currentUnit->unitValue == unit) {
                foundUnit = 1;
            }
            currentUnit = currentUnit->nextNode;
        }

        if (foundUnit) {
            printf("Palavra em Portugues: %s\n", portugueseWord);
            printf("Palavra em ingles: %s\n", currentNode->englishWord);
        }

        
        printTranslations(currentNode->left, unit, portugueseWord);
        printTranslations(currentNode->right, unit, portugueseWord);
    }
}


void showPortugueseTranslation(RedBlackTreePT **rootNode, char *portugueseWord) {
    if (*rootNode != NULL) {
      
        RedBlackTreePT *resultado = SearchWordInTree(rootNode, portugueseWord);

        if (resultado != NULL) {
            printf("Traducoes em ingles para a palavra '%s':\n", portugueseWord);

      
            BinaryTreeNode *node = resultado->info.englishWordNode;
            if (node == NULL) {
                printf("Nenhuma traducao encontrada.\n");
            } else {
                printBinaryTree(node);
            }
        } else {
            printf("Nenhuma traducao encontrada para '%s'.\n", portugueseWord);
        }
    } else {
        printf("Arvore vazia.\n");
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
    RedBlackTreePT *rbTreeOutput = NULL;

    if (rootNode != NULL) {
        BinaryTreeNode *currentNode = rootNode->info.englishWordNode;
        int found = 0; 

   
        while (currentNode != NULL && !found) {
            printf("Verificando palavra: '%s' na unidade %d\n", currentNode->englishWord, unit);

            Unit *currentUnit = currentNode->unitValues;
            int unitFound = 0;

            while (currentUnit != NULL && !unitFound) {
                if (currentUnit->unitValue == unit) {
                    unitFound = 1;
                }
                currentUnit = currentUnit->nextNode;
            }

        
            if (unitFound && strcmp(currentNode->englishWord, englishWord) == 0) {
                printf("Palavra encontrada na arvore binária associada ao no portugues: '%s'\n", rootNode->info.portugueseWord);
                rbTreeOutput = rootNode;
                found = 1;
            } else if (strcmp(englishWord, currentNode->englishWord) < 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        if (!found) {
            RedBlackTreePT *leftResult = SearchEnglishWordInRBTree(rootNode->left, englishWord, unit);
            if (leftResult != NULL) {
                rbTreeOutput = leftResult;
            } else {
                rbTreeOutput = SearchEnglishWordInRBTree(rootNode->right, englishWord, unit);
            }
        }
    }

    return rbTreeOutput;
}

void removeWordFromTree(RedBlackTreePT **node, char *wordToRemove, int *totalRemovido, RedBlackTreePT **rootNode, int unit) {
    if (*node != NULL) {
        removeWordFromTree(&(*node)->left, wordToRemove, totalRemovido, rootNode, unit);

        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        int removed = removeEnglishWord(&binaryRoot, wordToRemove, unit);
        if (removed) {
            (*totalRemovido)++;
        }

        if (binaryRoot == NULL) {
            removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
        } else {
            (*node)->info.englishWordNode = binaryRoot;
        }

        removeWordFromTree(&(*node)->right, wordToRemove, totalRemovido, rootNode, unit);
    }
}
void removeWordByUnit(RedBlackTreePT **node, char *wordToRemove, int unit, int *removidos, RedBlackTreePT **rootNode) {
    if (node != NULL && *node != NULL && wordToRemove != NULL && rootNode != NULL) {
        // Percorre a subárvore esquerda
        removeWordByUnit(&(*node)->left, wordToRemove, unit, removidos, rootNode);

        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        if (binaryRoot != NULL) {
            BinaryTreeNode *currentNode = binaryRoot;
            int found = 0;

            while (currentNode != NULL && !found) {
                if (strcmp(currentNode->englishWord, wordToRemove) == 0) {
                    // Remove a unidade associada
                    Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                    currentNode->unitValues = novaLista;

                    // Verifica se a lista de unidades está vazia
                    if (currentNode->unitValues == NULL) {
                        // Remove a palavra da árvore binária
                        *removidos += removeEnglishWord(&binaryRoot, wordToRemove, unit);
                    }
                    found = 1;
                } else if (strcmp(wordToRemove, currentNode->englishWord) < 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }
            }

            // Atualiza a referência na árvore binária
            (*node)->info.englishWordNode = binaryRoot;

            // Remove o nó rubro-negro se a árvore binária associada ficar vazia
            if (binaryRoot == NULL) {
                removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
            }
        }

        // Percorre a subárvore direita
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
void deleteWordFromTreeByUnit(RedBlackTreePT **redBlackTreeRoot, char *portugueseWord, int unit) {
    int error = 0;

   
    if (redBlackTreeRoot == NULL || *redBlackTreeRoot == NULL || portugueseWord == NULL) {
        printf("Erro: Entrada invalida ou arvore vazia.\n");
        error = 1;
    }

    RedBlackTreePT *noRBT = NULL;
    if (!error) {

        noRBT = SearchWordInTree(redBlackTreeRoot, portugueseWord);
        if (noRBT == NULL) {
            printf("Palavra '%s' nao encontrada na arvore rubro-negra.\n", portugueseWord);
            error = 1;
        }
    }

    if (!error && noRBT != NULL) {
        BinaryTreeNode *raizBST = noRBT->info.englishWordNode;
        if (raizBST != NULL) {
            BinaryTreeNode *currentNode = raizBST;
            BinaryTreeNode *nextNode = NULL;

            while (currentNode != NULL) {
                nextNode = (currentNode->left != NULL) ? currentNode->left : currentNode->right;

                Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                currentNode->unitValues = novaLista;

                if (currentNode->unitValues == NULL) {
                    
                    removeEnglishWord(&raizBST, currentNode->englishWord, unit);
                }

                currentNode = nextNode;
            }

            noRBT->info.englishWordNode = raizBST;

        
            if (raizBST == NULL) {
                removeRBTreeNode(redBlackTreeRoot, portugueseWord);
            }
        } else {
            printf("Nao ha palavras em ingles associadas ao no '%s'.\n", portugueseWord);
        }
    }

    if (error) {
        printf("Operacao nao concluida devido a erros.\n");
    } else {
        printf("Operacao concluida com sucesso.\n");
    }
}
