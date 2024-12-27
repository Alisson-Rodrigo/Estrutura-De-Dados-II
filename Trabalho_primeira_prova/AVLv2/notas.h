#ifndef NOTAS_H
#define NOTAS_H

typedef struct Arvore_notas {
    int codigo_disciplina;  
    char semestre_cursado[10];   
    float nota_final;              
    short altura;  
    struct Arvore_notas *esq;   
    struct Arvore_notas *dir;    
} arvore_notas;


arvore_notas *criar_nota();
arvore_notas *inserir_nota(arvore_notas *raiz, arvore_notas *no);  
void imprimir_notas(arvore_notas *raiz);  
arvore_notas *buscar_notas(arvore_notas *raiz, int codigo_disciplina); 
arvore_notas *remover_nota(arvore_notas *raiz, int codigo_disciplina);  
arvore_notas *buscar_nota_periodo(arvore_notas *raiz, int periodo);
#endif
