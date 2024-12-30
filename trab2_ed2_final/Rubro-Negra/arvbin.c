#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvrb.h"
#include "unidade.c"

BinaryTreeNode *initializeBinaryTreeNode(char *palavraIngles, int unidade) {
    BinaryTreeNode *novoNo = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (novoNo != NULL) {
        strcpy(novoNo->englishWord, palavraIngles);
        novoNo->unitValue = criar_unidade(unidade); // Inicializa a lista de unidades com o valor inicial
        novoNo->left = novoNo->rigth = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na árvore binária de busca
BinaryTreeNode *insertEnglishWord(BinaryTreeNode *root, char *palavraIngles, int unidade) {
    if (root == NULL) {
        // Cria um novo nó se a raiz for nula
        return initializeBinaryTreeNode(palavraIngles, unidade);
    } else {
        if (strcmp(palavraIngles, root->englishWord) < 0) {
            // Insere à esquerda
            root->left = insertEnglishWord(root->left, palavraIngles, unidade);
        } else if (strcmp(palavraIngles, root->englishWord) > 0) {
            // Insere à direita
            root->rigth = insertEnglishWord(root->rigth, palavraIngles, unidade);
        } else {
            // Palavra já existe, adiciona a unidade na lista
            root->unitValue = adicionar_unidade_ordenada(root->unitValue, criar_unidade(unidade));
        }
        return root;
    }
}

void addEnglishTranslation(RedBlackTreePT *raiz, char *palavraIng, int unidade) {
    // Adiciona a tradução em inglês e a unidade
    raiz->info.englishWordNode = insertEnglishWord(raiz->info.englishWordNode, palavraIng, unidade);
}

void printBinaryTree(BinaryTreeNode *root) {
    if (root != NULL) {
        printBinaryTree(root->left); // Percorre a árvore à esquerda
        printf("\n");
        printf("Palavra em Inglês: %s\n", root->englishWord);
        printf("Unidades: ");
        imprimir_unidades(root->unitValue); // Imprime a lista de unidades associadas
        printBinaryTree(root->rigth); // Percorre a árvore à direita
    }
}

int isLeafNodes(BinaryTreeNode *raiz) {
    return (raiz->left == NULL && raiz->rigth == NULL);
}

BinaryTreeNode *singleChildNode(BinaryTreeNode *raiz) {
    BinaryTreeNode *aux = NULL;

    if (raiz->rigth == NULL) {
        aux = raiz->left;
    } else if (raiz->left == NULL) {
        aux = raiz->rigth;
    }

    return aux;
}

BinaryTreeNode *minimumChildNode(BinaryTreeNode *raiz) {
    if (raiz && raiz->left) {
        return minimumChildNode(raiz->left);
    }
    return raiz;
}

int removeEnglishWord(BinaryTreeNode **raiz, char *palavra) {
    BinaryTreeNode *endFilho;
    int existe = 0;

    if (*raiz) {
        if (strcmp(palavra, (*raiz)->englishWord) == 0) {
            existe = 1;
            printf("Removendo palavra: %s\n", palavra);

            // Remove o nó da árvore binária
            BinaryTreeNode *aux = *raiz;
            if (isLeafNodes(*raiz)) {
                liberar_lista(aux->unitValue); // Libera a lista de unidades associada
                free(aux);
                *raiz = NULL;
            } else if ((endFilho = singleChildNode(*raiz)) != NULL) {
                liberar_lista(aux->unitValue); // Libera a lista de unidades associada
                free(aux);
                *raiz = endFilho;
            } else {
                endFilho = minimumChildNode((*raiz)->rigth);
                strcpy((*raiz)->englishWord, endFilho->englishWord);
                liberar_lista((*raiz)->unitValue);
                (*raiz)->unitValue = endFilho->unitValue; // Transfere a lista de unidades
                removeEnglishWord(&(*raiz)->rigth, endFilho->englishWord);
            }
        } else if (strcmp(palavra, (*raiz)->englishWord) < 0) {
            existe = removeEnglishWord(&(*raiz)->left, palavra);
        } else {
            existe = removeEnglishWord(&(*raiz)->rigth, palavra);
        }
    }

    return existe;
}

void FindEnglishTerm(RedBlackTreePT **raiz, char *palavraIngles, int unidade) {
    if (*raiz != NULL) {
        FindEnglishTerm(&(*raiz)->left, palavraIngles, unidade);

        if ((*raiz)->info.englishWordNode != NULL) {
            // Verifica se a unidade está na lista e remove a unidade
            Unidade *unidadeEncontrada = buscar_unidade((*raiz)->info.englishWordNode->unitValue, unidade);
            if (unidadeEncontrada) {
                (*raiz)->info.englishWordNode->unitValue = remover_unidade((*raiz)->info.englishWordNode->unitValue, unidade);

                // Se a lista de unidades ficar vazia, remova a palavra
                if ((*raiz)->info.englishWordNode->unitValue == NULL) {
                    removeEnglishWord(&(*raiz)->info.englishWordNode, palavraIngles);
                }
            }
        }

        FindEnglishTerm(&(*raiz)->right, palavraIngles, unidade);
    }
}
