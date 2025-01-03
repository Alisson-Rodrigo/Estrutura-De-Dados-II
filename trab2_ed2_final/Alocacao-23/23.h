#ifndef ARVORE23_H
#define ARVORE23_H

#define OCUPADO 0
#define LIVRE 1


typedef struct data
{
    int num_start;
    int num_end;
    int *end_start;
    int *end_end;
    int status;
} Data;

typedef struct arvore23
{
    Data info1;
    Data info2;
    int n_infos;
    struct arvore23 *left;
    struct arvore23 *center;
    struct arvore23 *right;
} Arvore23;


int eh_folha(Arvore23 no);

Arvore23 *no23_alocar();

void no23_desalocar(Arvore23 **no);

Arvore23 *no23_criar(Data info, Arvore23 *filho_left, Arvore23 *filho_center);

Data *no23_maior_info(Arvore23 *raiz);

Arvore23 *arvore23_criar();

Arvore23 *arvore23_buscar(Arvore23 *raiz, int info);

Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai);

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Data **maior_valor);

Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, int info);

Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, int info);

Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, int info);

void arvore23_desalocar(Arvore23 **raiz);

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info);

int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

int arvore23_remover2(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

int arvore23_remover(Arvore23 **raiz, int info);

int arvore23_rebalancear(Arvore23 **raiz, int info, Arvore23 **maior);

void no23_exibir(Data no);

void arvore23_exibir_pre(Arvore23 *raiz);

void arvore23_exibir_ordem(Arvore23 *raiz);

void arvore23_exibir_pos(Arvore23 *raiz);

#endif


