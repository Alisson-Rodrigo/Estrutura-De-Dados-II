#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvrb.h"
BinaryTreeNode *initializeBinaryTreeNode(char *palavraIngles, int unidade)
{
    BinaryTreeNode *novoNo = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (novoNo != NULL)
    {
        strcpy(novoNo->englishWord, palavraIngles);
        novoNo->unitValue = unidade;
        novoNo->left = novoNo->rigth = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
BinaryTreeNode *insertEnglishWord(BinaryTreeNode *root, char *palavraIngles, int unidade)
{
    BinaryTreeNode *result;
    if (root == NULL)
    {
        result = initializeBinaryTreeNode(palavraIngles, unidade);
    }
    else
    {
        if (strcmp(palavraIngles, root->englishWord) < 0)
        {
            root->left = insertEnglishWord(root->left, palavraIngles, unidade);
        }
        else if (strcmp(palavraIngles, root->englishWord) > 0)
        {
            root->rigth = insertEnglishWord(root->rigth, palavraIngles, unidade);
        }
        result = root;
    }
    return result;
}

void addEnglishTranslation(RedBlackTreePT *raiz, char *palavraIng, int unidade)
{
    raiz->info.englishWordNode = insertEnglishWord(raiz->info.englishWordNode, palavraIng, unidade);
}


void printBinaryTree(BinaryTreeNode *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left); // Percorre a árvore à esquerda
        printf("\n");
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s = Unidade: %d\n", root->englishWord, root->unitValue);
        printBinaryTree(root->rigth); // Percorre a árvore à direita
    }
}

int isLeafNodes(BinaryTreeNode *raiz)
{
    return (raiz->left == NULL && raiz->rigth == NULL);
}

BinaryTreeNode *singleChildNode(BinaryTreeNode *raiz)
{
    BinaryTreeNode *aux;
    aux = NULL;

    if (raiz->rigth == NULL)
    {
        aux = raiz->left;
    }
    else if (raiz->left == NULL)
    {
        aux = raiz->rigth;
    }

    return aux;
}

BinaryTreeNode *minimumChildNode(BinaryTreeNode *raiz)
{
    BinaryTreeNode *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->left)
            aux = minimumChildNode(raiz->left);
    }

    return aux;
}

int removeEnglishWord(BinaryTreeNode **raiz, char *palavra)
{
    BinaryTreeNode *endFilho;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->englishWord) == 0)
        {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            BinaryTreeNode *aux = *raiz;
            if (isLeafNodes(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = singleChildNode(*raiz)) != NULL)
            {
                free(aux);
                *raiz = endFilho;
            }
            else
            {
                endFilho = minimumChildNode((*raiz)->rigth);
                strcpy((*raiz)->englishWord, endFilho->englishWord);
                (*raiz)->unitValue = endFilho->unitValue;

                removeEnglishWord(&(*raiz)->rigth, endFilho->englishWord);
            }
        }
        else if (strcmp(palavra, (*raiz)->englishWord) < 0)
        {
            existe = removeEnglishWord(&(*raiz)->left, palavra);
        }
        else
        {
            existe = removeEnglishWord(&(*raiz)->rigth, palavra);
        }
    }

    return existe;
}

void FindEnglishTerm(RedBlackTreePT **raiz, char *palavraIngles, int unidade)
{
    if (*raiz != NULL)
    {
        FindEnglishTerm(&(*raiz)->left, palavraIngles, unidade);

        if ((*raiz)->info.englishWordNode != NULL && (*raiz)->info.englishWordNode->unitValue == unidade)
            removeEnglishWord(&(*raiz)->info.englishWordNode, palavraIngles);
        FindEnglishTerm(&(*raiz)->right, palavraIngles, unidade);
    }
}