#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H
#include "notas.h"
typedef struct Arvore_disciplinas {
    int codigo;
    char nome[50];
    int carga_horaria;
    short altura;
    int periodo;
    struct Arvore_disciplinas *esq;
    struct Arvore_disciplinas *dir;
}arvore_disciplinas;

arvore_disciplinas *criar_disciplina();
arvore_disciplinas *inserir_disciplina(arvore_disciplinas *raiz, arvore_disciplinas *no);
arvore_disciplinas *buscar_disciplina(arvore_disciplinas *raiz, int codigo);
arvore_disciplinas *remover_disciplina(arvore_disciplinas *raiz, int codigo);
void imprimir_disciplinas(arvore_disciplinas *raiz);
void imprimir_disciplinas_periodo(arvore_disciplinas *raiz, int periodo);
void imprimir_historico_disciplinas(arvore_notas *raiz_notas, arvore_disciplinas *raiz_disciplinas);
#endif 