#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.h"
#include "arvrb.h"
#define RED 1
#define BLACK 0

int insertPortugueseWord(RedBlackTreePT **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    int inseriu = 0;

    // Busca a palavra na árvore
    RedBlackTreePT *noExistente = NULL;
    noExistente =  SearchWordInTree(arvore, palavraPortugues);

    if (noExistente != NULL) {
        addEnglishTranslation(noExistente, palavraIngles, unidade);
        inseriu = 1;
    } else {
        Info novoInfo = createInfo(palavraPortugues, palavraIngles, unidade);
        insertRedBlackTree(arvore, &novoInfo);
        inseriu = 1;
    }
    return inseriu;
}
Info createInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.portugueseWord = malloc(strlen(palavra) + 1);
    strcpy(info.portugueseWord, palavra);

    info.englishWordNode = NULL;
    info.englishWordNode = insertEnglishWord(info.englishWordNode, palavraIngles, unidade);
    return info;
}

RedBlackTreePT *createNode(Info *informacao)
{
    RedBlackTreePT *novo = (RedBlackTreePT *)malloc(sizeof(RedBlackTreePT));
    novo->info = *informacao;
    novo->color = 1;
    novo->left = NULL;
    novo->right = NULL;
    return novo;
}
int getNodeColor(RedBlackTreePT *raiz)
{
    int cor;

    if (raiz == NULL)
        cor = BLACK;
    else
        cor = raiz->color;

    return cor;
}

void switch_node_color(RedBlackTreePT **raiz)
{
    (*raiz)->color = !(*raiz)->color;
    if ((*raiz)->left)
        (*raiz)->left->color = !(*raiz)->left->color;
    if ((*raiz)->right)
        (*raiz)->right->color = !(*raiz)->right->color;
}

void rotate_right(RedBlackTreePT **raiz)
{
    RedBlackTreePT *aux = (*raiz)->left;
    (*raiz)->left = aux->right;
    aux->right = *raiz;
    aux->color = (*raiz)->color;
    (*raiz)->color = RED;
    (*raiz) = aux;
}

void left_rotate(RedBlackTreePT **raiz)
{
    RedBlackTreePT *aux = (*raiz)->right;
    (*raiz)->right = aux->left;
    aux->left = *raiz;
    aux->color = (*raiz)->color;
    (*raiz)->color = RED;
    (*raiz) = aux;
}

void balanceTree(RedBlackTreePT **raiz)
{
    if (*raiz)
    {
        if (getNodeColor((*raiz)->right) == RED && getNodeColor((*raiz)->left) == BLACK)
            left_rotate(raiz);
        if (getNodeColor((*raiz)->left) == RED && getNodeColor((*raiz)->left->left) == RED)
            rotate_right(raiz);
        if (getNodeColor((*raiz)->left) == RED && getNodeColor((*raiz)->right) == RED)
            switch_node_color(raiz);
    }
}

int insertRedBlackNode(RedBlackTreePT **raiz, Info *informacao)
{

    int inseriu = 1;
    if (*raiz == NULL)
    {
        *raiz = createNode(informacao);
    }
    else
    {
        if (strcmp(informacao->portugueseWord, (*raiz)->info.portugueseWord) < 0)
        {
            inseriu = insertRedBlackNode(&(*raiz)->left, informacao);
        }
        else
        {
            inseriu = insertRedBlackNode(&(*raiz)->right, informacao);
        }
        balanceTree(&(*raiz));
    }

    return inseriu;
}

int insertRedBlackTree(RedBlackTreePT **raiz, Info *informacao)
{
    int inseriu = insertRedBlackNode(raiz, informacao);
    if (inseriu)
    {
        (*raiz)->color = BLACK;
    }
    return inseriu;
}

void shiftLeftRed(RedBlackTreePT **raiz)
{
    switch_node_color(raiz);

    if ((*raiz)->right && getNodeColor((*raiz)->right->left) == RED)
    {
        rotate_right(&(*raiz)->right);
        left_rotate((raiz));
        switch_node_color(raiz);
    }
}

void rotateRedRight(RedBlackTreePT **raiz)
{
    switch_node_color(raiz);

    if ((*raiz)->left && getNodeColor((*raiz)->left->left) == RED)
    {
        rotate_right(raiz);
        switch_node_color(raiz);
    }
}

void removeMinimum(RedBlackTreePT **raiz)
{
    if (!((*raiz)->left))
    {
        free(*raiz);
        *raiz = NULL;
    }
    else
    {
        if (getNodeColor((*raiz)->left) == BLACK && getNodeColor((*raiz)->left->left) == BLACK)
            shiftLeftRed(raiz);

        removeMinimum(&(*raiz)->left);
        balanceTree(raiz);
    }
}

RedBlackTreePT *findMinimum(RedBlackTreePT *raiz)
{
    RedBlackTreePT *menor;
    menor = raiz;

    if (raiz)
        if (raiz->left)
            menor = findMinimum(raiz->left);

    return menor;
}

int removeNodeFromRBTree(RedBlackTreePT **raiz, char *valor)
{
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(valor, (*raiz)->info.portugueseWord) < 0)
        {
            if ((*raiz)->left && getNodeColor((*raiz)->left) == BLACK && getNodeColor((*raiz)->left->left) == BLACK)
                shiftLeftRed(raiz);

            existe = removeNodeFromRBTree(&(*raiz)->left, valor);
        }
        else
        {
            if (getNodeColor((*raiz)->left) == RED)
                rotate_right(raiz);

            if (strcmp(valor, (*raiz)->info.portugueseWord) == 0 && (*raiz)->right == NULL)
            {
                free(*raiz);
                *raiz = NULL;

                existe = 1;
            }else{
                if ((*raiz)->right && getNodeColor((*raiz)->right) == BLACK && getNodeColor((*raiz)->right->left) == BLACK)
                rotateRedRight(raiz);

                if (strcmp(valor, (*raiz)->info.portugueseWord) == 0)
                {
                    RedBlackTreePT *aux;
                    aux = findMinimum((*raiz)->right);
                    (*raiz)->info = aux->info;
                    removeMinimum(&(*raiz)->right);

                    existe = 1;
                }
                else
                {
                    existe = removeNodeFromRBTree(&(*raiz)->right, valor);
                }
            }
        }
    }
    balanceTree(raiz);
    return existe;
}

int removeRBTreeNode(RedBlackTreePT **raiz, char *valor)
{
    int removeu = removeNodeFromRBTree(raiz, valor);
    if (removeu)
    {
        (*raiz)->color = BLACK;
    }
    return removeu;
}


RedBlackTreePT *SearchWordInTree(RedBlackTreePT **arvore, char *palavraPortugues)
{
    RedBlackTreePT *atual = NULL;

    if (*arvore != NULL)
    {
        if (strcmp(palavraPortugues, (*arvore)->info.portugueseWord) == 0)
        {
            atual = *arvore;
        }
        else if (strcmp(palavraPortugues, (*arvore)->info.portugueseWord) < 0)
        {
            atual = SearchWordInTree(&(*arvore)->left, palavraPortugues);
        }
        else
        {
            atual = SearchWordInTree(&(*arvore)->right, palavraPortugues);
        }
    }
    return atual;
}


void printWordsByUnit(RedBlackTreePT *arvore, int unidade)
{
    if (arvore)
    {
        printWordsByUnit(arvore->left, unidade);
        printTranslations(arvore->info.englishWordNode, unidade, arvore->info.portugueseWord);
        printWordsByUnit(arvore->right, unidade);
    }
}

void printTranslations(BinaryTreeNode *node, int unidade, char *palavraPortugues)
{
    if (node)
    {
        if (node->unitValue == unidade)
        {
            printf("Palavra em Português: %s\n", palavraPortugues);
            printf("Palavra em inglês: %s\n", node->englishWord);
        }
        printTranslations(node->left, unidade, palavraPortugues);
        printTranslations(node->rigth, unidade, palavraPortugues);
    }
}

void showPortugueseTranslation(RedBlackTreePT **raiz, char *palavraPortugues)
{
    RedBlackTreePT *resultado = NULL;
    if (raiz != NULL)
    {
        resultado = SearchWordInTree(raiz, palavraPortugues);
        if (resultado)
        {
            printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);

            if (strcmp(palavraPortugues, resultado->info.portugueseWord) == 0)
            {
                printBinaryTree(resultado->info.englishWordNode);
            }

        }
    }
}

void showRedBlackTree(RedBlackTreePT *raiz)
{
    if (raiz)
    {
        showRedBlackTree(raiz->left);
        printf("Cor - %d\n", raiz->color);
        printf("Palavra em Português - %s\n", raiz->info.portugueseWord);
        printBinaryTree(raiz->info.englishWordNode);
        printf("\n");
        showRedBlackTree(raiz->right);
    }
}
RedBlackTreePT *SearchEnglishWordInRBTree(RedBlackTreePT *raiz, char *palavraIngles, int unidade) {
    RedBlackTreePT *result = NULL; // Variável para armazenar o resultado

    if (raiz != NULL) {
        // Verifica na árvore binária associada ao nó atual
        BinaryTreeNode *currentNode = raiz->info.englishWordNode;
        while (currentNode != NULL) {
            if (currentNode->unitValue == unidade && strcmp(currentNode->englishWord, palavraIngles) == 0) {
                result = raiz; // Palavra encontrada na árvore binária
                break; // Sai do loop
            }

            if (strcmp(palavraIngles, currentNode->englishWord) < 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->rigth;
            }
        }

        // Se não encontrada no nó atual, busca nos filhos
        if (result == NULL) {
            result = SearchEnglishWordInRBTree(raiz->left, palavraIngles, unidade);
            if (result == NULL) {
                result = SearchEnglishWordInRBTree(raiz->right, palavraIngles, unidade);
            }
        }
    }

    return result; // Retorna o resultado final
}
