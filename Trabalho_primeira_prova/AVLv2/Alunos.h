#ifndef ALUNOS_H
#define ALUNOS_H
#include "notas.h"
#include "matricula.h"

typedef struct aluno
{
    int matricula;
    char nome[50];
    int codigo_curso;
    struct Arvore_notas *raiz_notas;   
    struct arvore_matricula *raiz_matriculas; 
    struct aluno *prox;
} Aluno;


Aluno *criar_aluno();


Aluno *inserir_aluno(Aluno *aluno, Aluno *no);


Aluno *remover_aluno(Aluno *aluno, int matricula);

Aluno *buscar_aluno(Aluno *aluno, int matricula);

void imprimir_alunos(Aluno *aluno, int codigo_curso);

int verificar_matricula_disciplinas(Aluno *raiz_alunos, int codigo_disciplina);
#endif