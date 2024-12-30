#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Inglesbin *createNode(const char *palavraIngles, int unidade)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidade = unidade;
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
Inglesbin *insertpalavraIngles(Inglesbin *root, const char *palavraIngles, int unidade)
{
    Inglesbin *result;
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

void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade)
{
    info->palavraIngles = insertpalavraIngles(info->palavraIngles, palavraIng, unidade);
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); // Percorre a árvore à esquerda
        printf("\n");
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s = Unidade: %d\n", root->palavraIngles, root->unidade);
        printBinaryTree(root->dir); // Percorre a árvore à direita
    }
}

int ehFolhas(Inglesbin *raiz){
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin *raiz){
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

Inglesbin *menorFilho(Inglesbin *raiz){
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, char *palavra)
{
    Inglesbin *endFilho;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0)
        {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            Inglesbin *aux = *raiz;
            if (ehFolhas(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(*raiz)) != NULL)
            {
                free(aux);
                *raiz = endFilho;
            }
            else
            {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidade = endFilho->unidade;

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

void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, int unidade, Portugues23 **pai)
{
    int removeu;
    if (*raiz != NULL)
    {
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade, pai);

        if ((*raiz)->info1.palavraIngles != NULL && (*raiz)->info1.palavraIngles->unidade == unidade)
        {
            removeu = removerPalavraIngles(&(*raiz)->info1.palavraIngles, palavraIngles);
            if(removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);
            if ((*raiz)->info1.palavraIngles == NULL)
            {
                removeu = remover23(pai, raiz, (*raiz)->info1.portugueseWord);
                if(removeu) printf("Removido\n\n");
            }
        }

        BuscarPalavraIngles(&(*raiz)->cent, palavraIngles, unidade, raiz);

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL && (*raiz)->info2.palavraIngles->unidade == unidade)
        {
            removeu = removerPalavraIngles(&(*raiz)->info2.palavraIngles, palavraIngles);
            if(removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);
            if ((*raiz)->info2.palavraIngles == NULL)
            {
                removeu = remover23(pai, raiz, (*raiz)->info2.portugueseWord);
                if(removeu) printf("Removido\n\n");
            }
        }
        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL)
        {
            BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade, raiz);
        }
    }
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