#include "arvbin.h"
#include "lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Inglesbin *createNode(const char *palavraIngles, char *unidade)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidades = NULL;
        inserir_lista_encadeada_unidade(&(novoNo->unidades), unidade);
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
int insertpalavraIngles(Inglesbin **root, Info *informacao)
{
    int result = 0;
    if ((*root) == NULL)
    {
        Inglesbin *new = createNode(informacao->palavraIngles->palavraIngles, informacao->palavraIngles->unidades->nome_unidade);
        *root = new;
        result = 1;
    }
    else if (strcmp(informacao->palavraIngles->palavraIngles, (*root)->palavraIngles) < 0)
        result = insertpalavraIngles(&(*root)->esq, informacao);
    else if (strcmp(informacao->palavraIngles->palavraIngles, (*root)->palavraIngles) > 0)
        result = insertpalavraIngles(&(*root)->dir, informacao);
    else
        result = inserir_lista_encadeada_unidade(&((*root)->unidades), informacao->palavraIngles->unidades->nome_unidade);
    
    return result;
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); // Percorre a árvore à esquerda
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s \n", root->palavraIngles);
        show_lista_encadeada_unidade(root->unidades);
        printBinaryTree(root->dir); // Percorre a árvore à direita
    }
}

int ehFolhas(Inglesbin *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin *raiz)
{
    Inglesbin *aux;
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

Inglesbin *menorFilho(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, const char *palavra)
{
    Inglesbin *endFilho;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0)
        {
            existe = 1;
            if (ehFolhas(*raiz))
            {
                free_arvore_binaria(*raiz);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(*raiz)) != NULL)
            {
                free_arvore_binaria(*raiz);
                *raiz = endFilho;
            }
            else
            {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidades = endFilho->unidades;

                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        }
        else if (strcmp(palavra, (*raiz)->palavraIngles) < 0)
        {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        }
        else
        {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}

void free_arvore_binaria(Inglesbin *raiz)
{
    if (raiz)
    {
        free_arvore_binaria(raiz->esq);
        free_arvore_binaria(raiz->dir);
        free(raiz->palavraIngles);
        free(raiz);
    }
}