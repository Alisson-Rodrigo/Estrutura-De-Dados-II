#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingles.h"
#include "portugues.h"

Arv_ingles *createNode(char *palavraIngles, int unidade)
{
    Arv_ingles *novoNo = (Arv_ingles *)malloc(sizeof(Arv_ingles));
    if (novoNo != NULL)
    {
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidade = unidade;
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

Arv_ingles *insertpalavraIngles(Arv_ingles *root, char *palavraIngles, int unidade)
{
    Arv_ingles *result;
    if (root == NULL)
    {
        result = createNode(palavraIngles, unidade);
    }
    else
    {
        if (strcmp(palavraIngles, root->palavraIngles) < 0)
        {
            root->esq = insertpalavraIngles(root->esq, palavraIngles, unidade);
        }
        else if (strcmp(palavraIngles, root->palavraIngles) > 0)
        {
            root->dir = insertpalavraIngles(root->dir, palavraIngles, unidade);
        }
        result = root;
    }
    return result;
}
int ehFolhas(Arv_ingles *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Arv_ingles *soUmFilho(Arv_ingles *raiz)
{
    Arv_ingles *aux;
    aux = NULL;

    if (raiz->dir == NULL)
    {
        aux = raiz->esq;
    }
    else if (raiz->esq == NULL)
    {
        aux = raiz->dir;
    }

    return aux;
}

Arv_ingles *menorFilho(Arv_ingles *raiz)
{
    Arv_ingles *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Arv_ingles **raiz, char *palavra) {
    int resultado = 0; 

    if (*raiz == NULL) {
        resultado = 0;
    } else if (strcmp(palavra, (*raiz)->palavraIngles) < 0) {
        resultado = removerPalavraIngles(&(*raiz)->esq, palavra);
    } else if (strcmp(palavra, (*raiz)->palavraIngles) > 0) {
        resultado = removerPalavraIngles(&(*raiz)->dir, palavra);
    } else {
        Arv_ingles *temp = *raiz;

        if ((*raiz)->esq == NULL && (*raiz)->dir == NULL) {
            *raiz = NULL;
        } else if ((*raiz)->esq == NULL) {
            *raiz = (*raiz)->dir;
        } else if ((*raiz)->dir == NULL) {
            *raiz = (*raiz)->esq;
        } else {
            Arv_ingles *menor = menorFilho((*raiz)->dir);
            strcpy((*raiz)->palavraIngles, menor->palavraIngles);
            (*raiz)->unidade = menor->unidade;
            removerPalavraIngles(&(*raiz)->dir, menor->palavraIngles);
        }

        free(temp); 
        resultado = 1; 
    }

    return resultado; 
}


void printBinaryTree(Arv_ingles *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); 
        printf("\n");
        printf("Palavra em Inglês: %s = Unidade: %d\n", root->palavraIngles, root->unidade);
        printBinaryTree(root->dir); 
    }
}

