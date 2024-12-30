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

int removerPalavraIngles(BinaryTreeNode **raiz, const char *palavra)
{
    BinaryTreeNode *endFilho = NULL;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->englishWord) == 0)
        {
            BinaryTreeNode *aux = *raiz;
            existe = 1;
            if (eh_Folha(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(raiz)) != NULL)
            {
                free(aux);
                *raiz = endFilho;
            }
            else
            {
                endFilho = menorFilho((*raiz)->rigth);
                strcpy((*raiz)->englishWord, endFilho->englishWord);
                (*raiz)->unitValue = endFilho->unitValue;

                removerPalavraIngles(&(*raiz)->rigth, endFilho->englishWord);
            }
        }
        else if (strcmp(palavra, (*raiz)->englishWord) < 0)
        {
            existe = removerPalavraIngles(&(*raiz)->left, palavra);
        }
        else
        {
            existe = removerPalavraIngles(&(*raiz)->left, palavra);
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
