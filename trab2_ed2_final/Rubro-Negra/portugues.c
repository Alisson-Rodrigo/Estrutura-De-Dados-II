#include "portugues.h"
#include "ingles.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1
#define BLACK 0

Info CriarInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.palavraPortugues = malloc(strlen(palavra) + 1);
    strcpy(info.palavraPortugues, palavra);

    info.palavraIngles = NULL;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    return info;
}

Arv_portugues*criaNo(Info *informacao)
{
    Arv_portugues *novo = (Arv_portugues *)malloc(sizeof(Arv_portugues));
    novo->info = *informacao;
    novo->cor = 1;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}
int cor_no(Arv_portugues *raiz)
{
    int cor;

    if (raiz == NULL)
        cor = BLACK;
    else
        cor = raiz->cor;

    return cor;
}

void trocar_cor(Arv_portugues **raiz)
{
    (*raiz)->cor = !(*raiz)->cor;
    if ((*raiz)->esq)
        (*raiz)->esq->cor = !(*raiz)->esq->cor;
    if ((*raiz)->dir)
        (*raiz)->dir->cor = !(*raiz)->dir->cor;
}

void rotar_direita(Arv_portugues **raiz)
{
    Arv_portugues *aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = RED;
    (*raiz) = aux;
}

void rotar_esquerda(Arv_portugues **raiz)
{
    Arv_portugues *aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = RED;
    (*raiz) = aux;
}

void balancear(Arv_portugues **raiz)
{
    if (*raiz)
    {
        if (cor_no((*raiz)->dir) == RED && cor_no((*raiz)->esq) == BLACK)
            rotar_esquerda(raiz);
        if (cor_no((*raiz)->esq) == RED && cor_no((*raiz)->esq->esq) == RED)
            rotar_direita(raiz);
        if (cor_no((*raiz)->esq) == RED && cor_no((*raiz)->dir) == RED)
            trocar_cor(raiz);
    }
}

int inserir_arv(Arv_portugues **raiz, Info *informacao)
{

    int inseriu = 1;
    if (*raiz == NULL)
    {
        *raiz = criaNo(informacao);
    }
    else
    {
        if (strcmp(informacao->palavraPortugues, (*raiz)->info.palavraPortugues) < 0)
        {
            inseriu = inserir_arv(&(*raiz)->esq, informacao);
        }
        else
        {
            inseriu = inserir_arv(&(*raiz)->dir, informacao);
        }
        balancear(&(*raiz));
    }

    return inseriu;
}

int inserirArvRec(Arv_portugues **raiz, Info *informacao)
{
    int inseriu = inserir_arv(raiz, informacao);
    if (inseriu)
    {
        (*raiz)->cor = BLACK;
    }
    return inseriu;
}

void move2EsqVermelha(Arv_portugues **raiz)
{
    trocar_cor(raiz);

    if ((*raiz)->dir && cor_no((*raiz)->dir->esq) == RED)
    {
        rotar_direita(&(*raiz)->dir);
        rotar_esquerda((raiz));
        trocar_cor(raiz);
    }
}

void move2DirVermelha(Arv_portugues **raiz)
{
    trocar_cor(raiz);

    if ((*raiz)->esq && cor_no((*raiz)->esq->esq) == RED)
    {
        rotar_direita(raiz);
        trocar_cor(raiz);
    }
}

void RemoverMenor(Arv_portugues **raiz)
{
    if (!((*raiz)->esq))
    {
        free(*raiz);
        *raiz = NULL;
    }
    else
    {
        if (cor_no((*raiz)->esq) == BLACK && cor_no((*raiz)->esq->esq) == BLACK)
            move2EsqVermelha(raiz);

        RemoverMenor(&(*raiz)->esq);
        balancear(raiz);
    }
}

Arv_portugues *ProcurarMenor(Arv_portugues *raiz)
{
    Arv_portugues *menor;
    menor = raiz;

    if (raiz)
        if (raiz->esq)
            menor = ProcurarMenor(raiz->esq);

    return menor;
}

int RemoverNo(Arv_portugues **raiz, char *valor)
{
    int existe = 0;


    if (*raiz)
    {
        if (strcmp(valor, (*raiz)->info.palavraPortugues) < 0)
        {
            if ((*raiz)->esq && cor_no((*raiz)->esq) == BLACK && cor_no((*raiz)->esq->esq) == BLACK)
                move2EsqVermelha(raiz);

            existe = RemoverNo(&(*raiz)->esq, valor);
        }
        else
        {
            if (cor_no((*raiz)->esq) == RED)
                rotar_direita(raiz);

            if (strcmp(valor, (*raiz)->info.palavraPortugues) == 0 && (*raiz)->dir == NULL)
            {
                free(*raiz);
                *raiz = NULL;

                existe = 1;
            }else{
                if ((*raiz)->dir && cor_no((*raiz)->dir) == BLACK && cor_no((*raiz)->dir->esq) == BLACK)
                move2DirVermelha(raiz);

                if (strcmp(valor, (*raiz)->info.palavraPortugues) == 0)
                {
                    Arv_portugues *aux;
                    aux = ProcurarMenor((*raiz)->dir);
                    (*raiz)->info = aux->info;
                    RemoverMenor(&(*raiz)->dir);

                    existe = 1;
                }
                else
                {
                    existe = RemoverNo(&(*raiz)->dir, valor);
                }
            }
        }
    }
    balancear(raiz);
    return existe;
}

void AdicionarTraducaoEN(Arv_portugues *raiz, char *palavraIng, int unidade)
{
    raiz->info.palavraIngles = insertpalavraIngles(raiz->info.palavraIngles, palavraIng, unidade);
}

Arv_portugues *BuscarPalavra(Arv_portugues **arvore, char *palavraPortugues)
{
    Arv_portugues *atual = NULL;

    if (*arvore != NULL)
    {
        if (strcmp(palavraPortugues, (*arvore)->info.palavraPortugues) == 0)
        {
            atual = *arvore;
        }
        else if (strcmp(palavraPortugues, (*arvore)->info.palavraPortugues) < 0)
        {
            atual = BuscarPalavra(&(*arvore)->esq, palavraPortugues);
        }
        else
        {
            atual = BuscarPalavra(&(*arvore)->dir, palavraPortugues);
        }
    }
    return atual;
}

void exibirArvore(Arv_portugues *raiz)
{
    if (raiz)
    {
        exibirArvore(raiz->esq);
        printf("Cor - %d\n", raiz->cor);
        printf("Valor - %s\n", raiz->info.palavraPortugues);
        printBinaryTree(raiz->info.palavraIngles);
        printf("\n");
        exibirArvore(raiz->dir);
    }
}

void BuscarPalavraIngles(Arv_portugues **raiz, char *palavraIngles, int unidade)
{
    if (*raiz != NULL)
    {
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade);

        if ((*raiz)->info.palavraIngles != NULL && (*raiz)->info.palavraIngles->unidade == unidade)
            removerPalavraIngles(&(*raiz)->info.palavraIngles, palavraIngles);
        BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade);
    }
}


void exibirPalavrasPorUnidade1(Arv_portugues *raiz, int unidade) {
    // Verifica se a árvore não está vazia
    if (raiz != NULL) {
        // Primeiro, processa a subárvore esquerda
        exibirPalavrasPorUnidade1(raiz->esq, unidade);

        // Processa o nó atual
        if (raiz->info.palavraIngles != NULL) {
            Arv_ingles *traducao = raiz->info.palavraIngles;

            // Percorre todas as traduções e imprime todas as palavras para a unidade fornecida
            while (traducao != NULL) {
                if (traducao->unidade == unidade) {
                    printf("%s: %s;\n", raiz->info.palavraPortugues, traducao->palavraIngles);
                }
                traducao = traducao->dir; // Avança para a próxima tradução
            }
        }

        // Por último, processa a subárvore direita
        exibirPalavrasPorUnidade1(raiz->dir, unidade);
    }
}