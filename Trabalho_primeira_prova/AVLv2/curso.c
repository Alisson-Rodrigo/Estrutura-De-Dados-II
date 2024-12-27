#include "curso.h"
#include "disciplinas.h"
#include "Alunos.h"
#include <stdlib.h>
#include <stdio.h>

short maior_no_curso(short a, short b)
{
    return (a > b) ? a : b;
}

short altura_do_no_curso(Arvore_curso *no)
{
    short altura;

    if (no == NULL)
    {
        altura = -1;
    }
    else
    {
        altura = no->altura;
    }

    return altura;
}

short fator_balanceamento_curso(Arvore_curso *no)
{
    short fb = 0;

    if (no != NULL)
    {
        fb = altura_do_no_curso(no->esq) - altura_do_no_curso(no->dir);
    }

    return fb;
}

Arvore_curso *rotar_esquerda_curso(Arvore_curso *no)
{
    Arvore_curso *aux, *aux1;

    aux = no->dir;
    aux1 = aux->esq;

    aux->esq = no;
    no->dir = aux1;

    no->altura = maior_no_curso(altura_do_no_curso(no->esq), altura_do_no_curso(no->dir)) + 1;
    aux->altura = maior_no_curso(altura_do_no_curso(aux->esq), altura_do_no_curso(aux->dir)) + 1;

    return aux;
}

Arvore_curso *rotar_direita_curso(Arvore_curso *no)
{
    Arvore_curso *aux, *aux1;

    aux = no->esq;
    aux1 = aux->dir;

    aux->dir = no;
    no->esq = aux1;

    no->altura = maior_no_curso(altura_do_no_curso(no->esq), altura_do_no_curso(no->dir)) + 1;
    aux->altura = maior_no_curso(altura_do_no_curso(aux->esq), altura_do_no_curso(aux->dir)) + 1;

    return aux;
}

Arvore_curso *rotar_direita_esquerda_curso(Arvore_curso *no)
{
    no->dir = rotar_direita_curso(no->dir);
    return rotar_esquerda_curso(no);
}

Arvore_curso *rotar_esquerda_direita_curso(Arvore_curso *no)
{
    no->esq = rotar_esquerda_curso(no->esq);
    return rotar_direita_curso(no);
}

Arvore_curso *balencar_arvore_curso(Arvore_curso *raiz)
{
    short fb = fator_balanceamento_curso(raiz);

    if (fb < -1 && fator_balanceamento_curso(raiz->dir) <= 0)
    {
        raiz = rotar_esquerda_curso(raiz);
    }
    else if (fb > 1 && fator_balanceamento_curso(raiz->esq) >= 0)
    {
        raiz = rotar_direita_curso(raiz);
    }
    else if (fb > 1 && fator_balanceamento_curso(raiz->esq) < 0)
    {
        raiz = rotar_esquerda_direita_curso(raiz);
    }
    else if (fb < -1 && fator_balanceamento_curso(raiz->dir) > 0)
    {
        raiz = rotar_direita_esquerda_curso(raiz);
    }
    return raiz;
}

Arvore_curso *criar_curso()
{
    Arvore_curso *curso = (Arvore_curso *)malloc(sizeof(Arvore_curso));
    if (curso == NULL)
    {
        printf("Erro ao alocar memória para o curso\n");
        exit(1);
    }
    curso->raiz_disciplinas = NULL;
    curso->esq = NULL;
    curso->dir = NULL;
    curso->altura = 0;
    return curso;
}

Arvore_curso *inserir_curso(Arvore_curso *raiz, Arvore_curso *no)
{
    if (raiz == NULL)
    {
        raiz = no;
    }
    else
    {
        if (no->codigo < raiz->codigo)
        {
            raiz->esq = inserir_curso(raiz->esq, no);
        }
        else
        {
            raiz->dir = inserir_curso(raiz->dir, no);
        }
    }
    raiz = balencar_arvore_curso(raiz);
    raiz->altura = maior_no_curso(altura_do_no_curso(raiz->esq), altura_do_no_curso(raiz->dir)) + 1;
    return raiz;
}

void imprimir_cursos(Arvore_curso *raiz)
{
    if (raiz != NULL)
    {
        imprimir_cursos(raiz->esq);
        printf("Codigo: %d\n", raiz->codigo);
        printf("Nome: %s\n", raiz->nome);
        printf("Periodo: %d\n", raiz->periodo);
        printf("\n");
        imprimir_cursos(raiz->dir);
    }
}

Arvore_curso *buscar_curso(Arvore_curso *raiz, int codigo)
{
    Arvore_curso *aux = NULL;
    if (raiz != NULL)
    {
        if (codigo == raiz->codigo)
        {
            aux = raiz;
        }
        else
        {
            if (codigo < raiz->codigo)
            {
                aux = buscar_curso(raiz->esq, codigo);
            }
            else
            {
                aux = buscar_curso(raiz->dir, codigo);
            }
        }
    }
    return aux;
}

Arvore_curso *remover_curso(Arvore_curso *raiz, int codigo)
{
    if (raiz != NULL)
    {

        if (codigo < raiz->codigo)
        {
            raiz->esq = remover_curso(raiz->esq, codigo);
        }
        else if (codigo > raiz->codigo)
        {
            raiz->dir = remover_curso(raiz->dir, codigo);
        }
        else
        {

            if (raiz->esq == NULL && raiz->dir == NULL)
            {
                free(raiz);
                raiz = NULL;
            }

            else if (raiz->esq == NULL)
            {
                Arvore_curso *aux = raiz;
                raiz = raiz->dir;
                free(aux);
            }
            else if (raiz->dir == NULL)
            {
                Arvore_curso *aux = raiz;
                raiz = raiz->esq;
                free(aux);
            }
            else
            {
                Arvore_curso *aux = raiz->esq;
                while (aux->dir != NULL)
                {
                    aux = aux->dir;
                }
                raiz->codigo = aux->codigo;
                raiz->esq = remover_curso(raiz->esq, aux->codigo);
            }
        }

        if (raiz != NULL)
        {
            raiz->altura = maior_no_curso(altura_do_no_curso(raiz->esq), altura_do_no_curso(raiz->dir)) + 1;
            raiz = balencar_arvore_curso(raiz);
        }
    }
    return raiz;
}

void imprimir_historico(Aluno *aluno, Arvore_curso *raiz_cursos)
{
    Arvore_curso *curso = buscar_curso(raiz_cursos, aluno->codigo_curso);
    if (curso == NULL)
    {
        printf("Curso nao encontrado.\n");
    }

    printf("Curso: %s\n", curso->nome);

    imprimir_historico_disciplinas(aluno->raiz_notas, curso->raiz_disciplinas);
}
