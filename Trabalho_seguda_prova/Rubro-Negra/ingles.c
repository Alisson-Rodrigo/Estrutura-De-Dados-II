#include "ingles.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portugues.h"
#include "unidade.h"

BinaryTreeNode *initializeBinaryTreeNode(char *englishWord, int unit) {
    BinaryTreeNode *newNode = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o nó da árvore binária");
        exit(EXIT_FAILURE);
    }

 
    strcpy(newNode->englishWord, englishWord);
    newNode->unitValues = create_unit(unit); 
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

BinaryTreeNode *insertEnglishWord(BinaryTreeNode *rootNode, char *englishWord, int unit) {
    BinaryTreeNode *result = rootNode; 

    if (rootNode == NULL) {
       
        BinaryTreeNode *newTreeNode = initializeBinaryTreeNode(englishWord, unit);
        if (newTreeNode != NULL) {
            result = newTreeNode;
        } else {
            perror("Erro ao alocar memória para o nó da árvore binária.");
        }
    } else {
        int cmp = strcmp(englishWord, rootNode->englishWord);
        if (cmp < 0) {
            rootNode->left = insertEnglishWord(rootNode->left, englishWord, unit);
        } else if (cmp > 0) {
            rootNode->right = insertEnglishWord(rootNode->right, englishWord, unit);
        } else {
            
            Unit *newUnit = create_unit(unit);
            if (newUnit != NULL) {
                rootNode->unitValues = insert_unit_sorted(rootNode->unitValues, newUnit);
            } else {
                perror("Erro ao alocar memória para a unidade.");
            }
        }
    }

    return result; 
}

void addEnglishTranslation(RedBlackTreePT *rootNode, char *englishWord, int unit) {
    if (rootNode != NULL) {
        BinaryTreeNode *novoRoot = insertEnglishWord(rootNode->info.englishWordNode, englishWord, unit);
        rootNode->info.englishWordNode = novoRoot;
    } else {
        perror("Erro: No da árvore vermelho-preto e nulo.");
    }
}

void printBinaryTree(BinaryTreeNode *root) {
    if (root != NULL) {
        printBinaryTree(root->left);


        printf("Palavra em ingles: %s\n", root->englishWord);

        Unit *currentUnit = root->unitValues;
        printf("Unidades: ");
        while (currentUnit != NULL) {
            printf("%d ", currentUnit->unitValue);
            currentUnit = currentUnit->nextNode;
        }
        printf("\n");

        printBinaryTree(root->right);
    }
}



int isLeafNodes(BinaryTreeNode *rootNode)
{
    return (rootNode->left == NULL && rootNode->right == NULL);
}

BinaryTreeNode *singleChildNode(BinaryTreeNode *rootNode)
{
    BinaryTreeNode *aux;
    aux = NULL;

    if (rootNode->right == NULL)
    {
        aux = rootNode->left;
    }
    else if (rootNode->left == NULL)
    {
        aux = rootNode->right;
    }

    return aux;
}

BinaryTreeNode *minimumChildNode(BinaryTreeNode *rootNode)
{
    BinaryTreeNode *aux;
    aux = rootNode;

    if (rootNode)
    {
        if (rootNode->left)
            aux = minimumChildNode(rootNode->left);
    }

    return aux;
}

int removeEnglishWord(BinaryTreeNode **rootNode, const char *wordToRemove, int unit) {
    int isFound = 0; 

    if (rootNode != NULL && *rootNode != NULL && wordToRemove != NULL) {
        if (strcmp(wordToRemove, (*rootNode)->englishWord) == 0) {
           
            isFound = 1;
            Unit *newUnitList = remove_unit((*rootNode)->unitValues, unit);
            (*rootNode)->unitValues = newUnitList;

            if (newUnitList == NULL) { 
                BinaryTreeNode *nodeToRemove = *rootNode;

                if (isLeafNodes(*rootNode)) {
                   
                    free(nodeToRemove);
                    *rootNode = NULL;
                } else if ((nodeToRemove = singleChildNode(*rootNode)) != NULL) {
                
                    free(*rootNode);
                    *rootNode = nodeToRemove;
                } else {
                
                    BinaryTreeNode *successor = minimumChildNode((*rootNode)->right);
                    strcpy((*rootNode)->englishWord, successor->englishWord);
                    (*rootNode)->unitValues = successor->unitValues;
                    removeEnglishWord(&(*rootNode)->right, successor->englishWord, unit);
                }
            }
        } else if (strcmp(wordToRemove, (*rootNode)->englishWord) < 0) {
            
            isFound = removeEnglishWord(&(*rootNode)->left, wordToRemove, unit);
        } else {
            
            isFound = removeEnglishWord(&(*rootNode)->right, wordToRemove, unit);
        }
    }

    return isFound;
}




void RemoveUnitFromEnglishTerm(RedBlackTreePT **rootNode, char *englishTerm, int unit) {
    if (*rootNode != NULL) {
        RemoveUnitFromEnglishTerm(&(*rootNode)->left, englishTerm, unit);

        if ((*rootNode)->info.englishWordNode != NULL) {
            BinaryTreeNode *currentNode = (*rootNode)->info.englishWordNode;
            int found = 0; 

            
            while (currentNode != NULL && !found) {
                if (strcmp(currentNode->englishWord, englishTerm) == 0) {
                    Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                    currentNode->unitValues = novaLista;

                   
                    if (currentNode->unitValues == NULL) {
                        removeEnglishWord(&(*rootNode)->info.englishWordNode, englishTerm, unit);
                    }

                    found = 1; 
                } else if (strcmp(englishTerm, currentNode->englishWord) < 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }

        RemoveUnitFromEnglishTerm(&(*rootNode)->right, englishTerm, unit);
    }
}


void exibir_arvorebinaria(BinaryTreeNode *rootNode) {
    if (rootNode != NULL) {
        exibir_arvorebinaria(rootNode->left);

  
        printf("Palavra em ingles: %s\n", rootNode->englishWord);

    
        Unit *currentUnit = rootNode->unitValues;
        printf("Unidades: ");
        while (currentUnit != NULL) {
            printf("%d ", currentUnit->unitValue);
            currentUnit = currentUnit->nextNode;
        }
        printf("\n");

        exibir_arvorebinaria(rootNode->right);
    }
}