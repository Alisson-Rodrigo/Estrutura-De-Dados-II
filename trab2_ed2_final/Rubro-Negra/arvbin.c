#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvrb.h"
#include "unidade.h"

BinaryTreeNode *initializeBinaryTreeNode(char *englishWord, int unit) {
    // Aloca memória para o novo nó
    BinaryTreeNode *newNode = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o nó da árvore binária");
        exit(EXIT_FAILURE);
    }

    // Inicializa os campos do nó
    strcpy(newNode->englishWord, englishWord);
    newNode->unitValues = create_unit(unit); // Cria a lista de unidades com a primeira unidade
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

BinaryTreeNode *insertEnglishWord(BinaryTreeNode *rootNode, char *englishWord, int unit) {
    BinaryTreeNode *result = rootNode; // Variável para manter o único retorno

    if (rootNode == NULL) {
        // Cria um novo nó da árvore binária
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
            // Palavra já existe, adiciona a unidade à lista ordenada
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
        perror("Erro: Nó da árvore vermelho-preto é nulo.");
    }
}

void printBinaryTree(BinaryTreeNode *root) {
    if (root != NULL) {
        printBinaryTree(root->left);

        // Imprime a palavra em inglês
        printf("Palavra em inglês: %s\n", root->englishWord);

        // Imprime as unidades associadas (caso necessário para depuração)
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

int removeEnglishWord(BinaryTreeNode **rootNode, char *wordToRemove) {
    int isFound = 0; // Variável para manter o único retorno
    BinaryTreeNode *lastChild = NULL;

    if (*rootNode) {
        if (strcmp(wordToRemove, (*rootNode)->englishWord) == 0) {
            isFound = 1;
            printf("Removendo palavra: %s\n", wordToRemove);
            BinaryTreeNode *aux = *rootNode;

            if (isLeafNodes(*rootNode)) {
                // Libera a lista de unidades associada
                free_list(aux->unitValues);
                free(aux);
                *rootNode = NULL;
            } else if ((lastChild = singleChildNode(*rootNode)) != NULL) {
                // Libera a lista de unidades associada
                free_list(aux->unitValues);
                free(aux);
                *rootNode = lastChild;
            } else {
                // Substitui pelo menor nó da subárvore direita
                lastChild = minimumChildNode((*rootNode)->right);
                strcpy((*rootNode)->englishWord, lastChild->englishWord);

                // Libera a lista antiga e copia a nova
                free_list((*rootNode)->unitValues);
                (*rootNode)->unitValues = NULL;

                // Clona a lista de unidades do nó substituto
                Unit *currentUnit = lastChild->unitValues;
                while (currentUnit != NULL) {
                    Unit *novaUnidade = create_unit(currentUnit->unitValue);
                    (*rootNode)->unitValues = insert_unit_sorted((*rootNode)->unitValues, novaUnidade);
                    currentUnit = currentUnit->nextNode;
                }

                removeEnglishWord(&(*rootNode)->right, lastChild->englishWord);
            }
        } else if (strcmp(wordToRemove, (*rootNode)->englishWord) < 0) {
            isFound = removeEnglishWord(&(*rootNode)->left, wordToRemove);
        } else {
            isFound = removeEnglishWord(&(*rootNode)->right, wordToRemove);
        }
    }

    return isFound; // Único ponto de retorno
}


void FindEnglishTerm(RedBlackTreePT **rootNode, char *englishTerm, int unit) {
    if (*rootNode != NULL) {
        // Recursão para o lado esquerdo da árvore
        FindEnglishTerm(&(*rootNode)->left, englishTerm, unit);

        // Verifica se a árvore binária de palavras em inglês não está vazia
        if ((*rootNode)->info.englishWordNode != NULL) {
            // Percorre a árvore binária para encontrar a palavra e remover a unidade
            BinaryTreeNode *currentNode = (*rootNode)->info.englishWordNode;
            while (currentNode != NULL) {
                if (strcmp(currentNode->englishWord, englishTerm) == 0) {
                    // Encontra a palavra e remove a unidade da lista
                    Unit *novaLista = remove_unit(currentNode->unitValues, unit);
                    currentNode->unitValues = novaLista;

                    // Verifica se a lista de unidades está vazia
                    if (currentNode->unitValues == NULL) {
                        // Remove o nó da árvore binária se não houver mais unidades
                        removeEnglishWord(&(*rootNode)->info.englishWordNode, englishTerm);
                    }
                    break; // Palavra encontrada e tratada
                }

                // Navega para o próximo nó na árvore binária
                if (strcmp(englishTerm, currentNode->englishWord) < 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }

        // Recursão para o lado direito da árvore
        FindEnglishTerm(&(*rootNode)->right, englishTerm, unit);
    }
}

void exibir_arvorebinaria(BinaryTreeNode *rootNode) {
    if (rootNode != NULL) {
        exibir_arvorebinaria(rootNode->left);

        // Imprime a palavra em inglês
        printf("Palavra em inglês: %s\n", rootNode->englishWord);

        // Imprime as unidades associadas (caso necessário para depuração)
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