#include <stdlib.h>
#include <string.h>
#include "ingles_arv.h"
#include "portugues_arv.h"
Arv_en *createNode(const char *palavraIngles, int unidade)
{
    Arv_en *novoNo = (Arv_en *)malloc(sizeof(Arv_en));
    if (novoNo != NULL)
    {
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidade.unidade = unidade;
        novoNo->esq = novoNo->dir = NULL;
        novoNo->unidade.prox = NULL;
    }
    return novoNo;
}


Arv_en *insertpalavraIngles(Arv_en *root, const char *palavraIngles, int unidade)
{
    Arv_en *result;
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

void printBinaryTree(Arv_en *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); 
        printf("%s\n", root->palavraIngles); 
        printBinaryTree(root->dir); 
    }
}


int ehFolhas(Arv_en *raiz){
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Arv_en *soUmFilho(Arv_en *raiz){
    Arv_en *aux;
    aux = NULL;

    if(raiz->dir == NULL){
        aux = raiz->esq;
    }else if(raiz->esq == NULL){
        aux = raiz->dir;
    }

    return aux;
}

Arv_en *menorFilho(Arv_en *raiz){
    Arv_en *aux;
    aux = raiz;

    if(raiz){
        if(raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Arv_en **raiz, char *palavra) {
    Arv_en *endFilho;
    int existe = 0;

    if (*raiz) {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0) {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            Arv_en *aux = *raiz;
            if (ehFolhas(*raiz)) {
                free(aux);
                *raiz = NULL;
            } else if ((endFilho = soUmFilho(*raiz)) != NULL) {
                free(aux);
                *raiz = endFilho;
            } else {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidade = endFilho->unidade;

                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        } else if (strcmp(palavra, (*raiz)->palavraIngles) < 0) {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        } else {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}

