#ifndef MATRICULA_H
#define MATRICULA_H


typedef struct arvore_matricula {
    int codigo_disciplina;   
    struct arvore_matricula *esq;  
    struct arvore_matricula *dir;  
} arvore_matricula;



void Ler_matricula(arvore_matricula *no);


arvore_matricula *inserir_matriculas(arvore_matricula *raiz, arvore_matricula *no);


void imprimir_matricula(arvore_matricula *raiz);


arvore_matricula *buscar_matricula(arvore_matricula *raiz, int codigo_disciplina);


arvore_matricula *remover_matricula(arvore_matricula *raiz, int codigo_disciplina);

#endif 