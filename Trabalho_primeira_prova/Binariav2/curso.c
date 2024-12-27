#include "curso.h"
#include "disciplinas.h"
#include "alunos.h"
#include <stdlib.h>
#include <stdio.h>


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
    return curso;
}

void ler_dados_curso(Arvore_curso *curso)
{
    printf("Digite o codigo do curso: ");
    scanf("%d", &curso->codigo);

    printf("Digite o nome do curso: ");
    scanf(" %[^\n]", curso->nome);

    printf("Digite o periodo do curso: ");
    scanf("%d", &curso->periodo);
}

Arvore_curso *inserir_curso(Arvore_curso *curso, Arvore_curso *no)
{
    if (curso == NULL)
    {
        curso = no;
    }
    else
    {
        if (no->codigo < curso->codigo)
        {
            curso->esq = inserir_curso(curso->esq, no);
        }
        else
        {
            curso->dir = inserir_curso(curso->dir, no);
        }
    }
    return curso;
}

void imprimir_cursos(Arvore_curso *curso)
{
    if (curso != NULL)
    {
        imprimir_cursos(curso->esq);
        printf("Codigo: %d\n", curso->codigo);
        printf("Nome: %s\n", curso->nome);
        printf("Periodo: %d\n", curso->periodo);
        printf("\n");
        imprimir_cursos(curso->dir);
    }
}

Arvore_curso *buscar_curso (Arvore_curso *curso, int codigo)
{
    Arvore_curso *aux = NULL;
    if (curso != NULL)
    {
        if (codigo == curso->codigo)
        {
            aux = curso;
        }
        else
        {
            if (codigo < curso->codigo)
            {
                aux = buscar_curso(curso->esq, codigo);
            }
            else
            {
                aux = buscar_curso(curso->dir, codigo);
            }
        }
    }
    return aux;
}

Arvore_curso *remover_curso(Arvore_curso *curso, int codigo)
{
    if (curso != NULL)
    {
        if (codigo < curso->codigo)
        {
            curso->esq = remover_curso(curso->esq, codigo);
        }
        else if (codigo > curso->codigo)
        {
            curso->dir = remover_curso(curso->dir, codigo);
        }
        else
        {
            if (curso->esq == NULL && curso->dir == NULL)
            {
                free(curso);
                curso = NULL;
            }
            else if (curso->esq == NULL)
            {
                Arvore_curso *aux = curso;
                curso = curso->dir;
                free(aux);
            }
            else if (curso->dir == NULL)
            {
                Arvore_curso *aux = curso;
                curso = curso->esq;
                free(aux);
            }
            else
            {
                Arvore_curso *aux = curso->esq;
                while (aux->dir != NULL)
                {
                    aux = aux->dir;
                }
                curso->codigo = aux->codigo;
                curso->esq = remover_curso(curso->esq, aux->codigo);
            }
        }
    }
    return curso;


}

void imprimir_historico(Aluno *aluno, Arvore_curso *raiz_cursos)
{

    Arvore_curso *curso = buscar_curso(raiz_cursos, aluno->codigo_curso);
    if (curso == NULL)
    {
        printf("Curso nao encontrado.\n");
        return;
    }

    printf("Curso: %s\n", curso->nome);

   
    imprimir_historico_disciplinas(aluno->raiz_notas, curso->raiz_disciplinas);
}