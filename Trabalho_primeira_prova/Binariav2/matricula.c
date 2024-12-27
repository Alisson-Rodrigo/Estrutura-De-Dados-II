#include <stdio.h>
#include <stdlib.h>
#include "matricula.h"


struct arvore_matricula;

arvore_matricula *criar_matricula()
{
    arvore_matricula *no = (arvore_matricula *)malloc(sizeof(arvore_matricula));
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

arvore_matricula *inserir_matriculas(arvore_matricula *raiz, arvore_matricula *no) {
    if (raiz == NULL) {
        
        raiz = no;
    } else {
        
        if (no->codigo_disciplina < raiz->codigo_disciplina) {
          
            raiz->esq = inserir_matriculas(raiz->esq, no);
        } else if (no->codigo_disciplina > raiz->codigo_disciplina) {
         
            raiz->dir = inserir_matriculas(raiz->dir, no);
        }
       
    }

    return raiz;
}



void imprimir_matricula(arvore_matricula *raiz)
{
    if (raiz != NULL)
    {
        printf("Codigo da disciplina: %d\n", raiz->codigo_disciplina);

        imprimir_matricula(raiz->esq); 
        imprimir_matricula(raiz->dir); 
    }
}

arvore_matricula *buscar_matricula(arvore_matricula *raiz, int codigo_disciplina)
{
    arvore_matricula *aux = NULL;
    if (raiz != NULL)
    {
        if (codigo_disciplina == raiz->codigo_disciplina)
        {
            aux = raiz;
        }
        else if (codigo_disciplina < raiz->codigo_disciplina)
        {
            aux = buscar_matricula(raiz->esq, codigo_disciplina); 
        }
        else
        {
            aux = buscar_matricula(raiz->dir, codigo_disciplina); 
        }
    }
    return aux;
}



arvore_matricula *remover_matricula(arvore_matricula *raiz, int codigo_disciplina)
{
    if (raiz != NULL)
    {
        if (codigo_disciplina < raiz->codigo_disciplina)
        {
            raiz->esq = remover_matricula(raiz->esq, codigo_disciplina); 
        }
        else if (codigo_disciplina > raiz->codigo_disciplina)
        {
            raiz->dir = remover_matricula(raiz->dir, codigo_disciplina); 
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
                arvore_matricula *aux = raiz;
                raiz = raiz->dir;
                free(aux);
            }
            else if (raiz->dir == NULL)
            {
                arvore_matricula *aux = raiz;
                raiz = raiz->esq;
                free(aux);
            }
            else
            {
                arvore_matricula *aux = raiz->esq;
                while (aux->dir != NULL)
                {
                    aux = aux->dir;
                }
                raiz->codigo_disciplina = aux->codigo_disciplina;
                raiz->esq = remover_matricula(raiz->esq, aux->codigo_disciplina); 
            }
        }
    }
    return raiz;
}
