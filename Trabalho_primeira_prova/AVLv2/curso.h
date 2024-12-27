#ifndef CURSO_H
#define CURSO_H

#include "disciplinas.h"
#include "Alunos.h"

typedef struct arvore_curso {
    int codigo;
    char nome[100];
    int periodo;
    short altura;
    arvore_disciplinas *raiz_disciplinas;
    struct arvore_curso *esq;
    struct arvore_curso *dir;
} Arvore_curso;

Arvore_curso *criar_curso();
void ler_dados_curso(Arvore_curso *curso);
Arvore_curso *inserir_curso(Arvore_curso *raiz, Arvore_curso *no);
void imprimir_cursos(Arvore_curso *raiz);
Arvore_curso *buscar_curso(Arvore_curso *raiz, int codigo);
Arvore_curso *remover_curso(Arvore_curso *raiz, int codigo);
void imprimir_historico(Aluno *aluno, Arvore_curso *raiz_cursos);

#endif
