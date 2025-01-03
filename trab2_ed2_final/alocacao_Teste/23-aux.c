#include <stdio.h>
#include <stdlib.h>
#include "23.h"

// ---------------------------- Funções Auxiliares ----------------------------
int eh_folha(Arvore23 no) {
    return no.esquerdo == NULL;
}

static int eh_info1(Arvore23 no, int info) {
    return info == no.info1.numero_inicial;
}

static int eh_info2(Arvore23 no, int info) {
    return no.n_infos == 2 && info == no.info2.numero_inicial;
}

static int calcular_altura(Arvore23 *no) {
    return (no == NULL) ? -1 : 1 + calcular_altura(no->esquerdo);
}

static int possivel_remover(Arvore23 *raiz) {
    if (raiz == NULL) return 0;
    if (raiz->n_infos == 2) return 1;
    return possivel_remover(raiz->centro) || possivel_remover(raiz->esquerdo);
}

// ---------------------------- Operações com Nó ----------------------------
Arvore23 *no23_alocar() {
    Arvore23 *no = (Arvore23 *)malloc(sizeof(Arvore23));
    if (!no) {
        perror("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }
    return no;
}

void no23_desalocar(Arvore23 **no) {
    free(*no);
    *no = NULL;
}

Arvore23 *no23_criar(Data info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro) {
    Arvore23 *no = no23_alocar();
    no->info1 = info;
    no->esquerdo = filho_esquerdo;
    no->centro = filho_centro;
    no->direito = NULL;
    no->n_infos = 1;
    return no;
}

static Arvore23 *no23_quebrar(Arvore23 *no, Data info, Data *promove, Arvore23 *filho_maior) {
    Arvore23 *maior;
    if (info.numero_inicial > no->info2.numero_inicial) {
        *promove = no->info2;
        maior = no23_criar(info, no->direito, filho_maior);
    } else if (info.numero_inicial > no->info1.numero_inicial) {
        *promove = info;
        maior = no23_criar(no->info2, filho_maior, no->direito);
    } else {
        *promove = no->info1;
        maior = no23_criar(no->info2, no->centro, no->direito);
        no->info1 = info;
        no->centro = filho_maior;
    }
    no->n_infos = 1;
    return maior;
}

static void no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior) {
    if (info.numero_inicial > no->info1.numero_inicial) {
        no->info2 = info;
        no->direito = filho_maior;
    } else {
        no->info2 = no->info1;
        no->direito = no->centro;
        no->centro = filho_maior;
        no->info1 = info;
    }
    no->n_infos = 2;
}

// ---------------------------- Operações na Árvore ----------------------------
Arvore23 *arvore23_criar() {
    return NULL;
}

Arvore23 *arvore23_buscar(Arvore23 *raiz, int info) {
    if (raiz == NULL) return NULL;
    if (eh_info1(*raiz, info) || eh_info2(*raiz, info)) return raiz;
    if (info < raiz->info1.numero_inicial) return arvore23_buscar(raiz->esquerdo, info);
    if (raiz->n_infos == 1 || info < raiz->info2.numero_inicial) return arvore23_buscar(raiz->centro, info);
    return arvore23_buscar(raiz->direito, info);
}

Arvore23 *arvore23_inserir_no(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove) {
    if (*raiz == NULL) {
        *raiz = no23_criar(info, NULL, NULL);
        return NULL;
    }

    if (eh_folha(**raiz)) {
        if ((*raiz)->n_infos == 1) {
            no23_adicionar_info(*raiz, info, NULL);
            return NULL;
        }
        Arvore23 *maior = no23_quebrar(*raiz, info, promove, NULL);
        if (pai == NULL) {
            *raiz = no23_criar(*promove, *raiz, maior);
            return NULL;
        }
        return maior;
    }

    Arvore23 *maior;
    if (info.numero_inicial < (*raiz)->info1.numero_inicial) {
        maior = arvore23_inserir_no(&((*raiz)->esquerdo), info, *raiz, promove);
    } else if ((*raiz)->n_infos == 1 || info.numero_inicial < (*raiz)->info2.numero_inicial) {
        maior = arvore23_inserir_no(&((*raiz)->centro), info, *raiz, promove);
    } else {
        maior = arvore23_inserir_no(&((*raiz)->direito), info, *raiz, promove);
    }

    if (maior != NULL) {
        if ((*raiz)->n_infos == 1) {
            no23_adicionar_info(*raiz, *promove, maior);
            return NULL;
        }
        Data promove_aux;
        maior = no23_quebrar(*raiz, *promove, &promove_aux, maior);
        *promove = promove_aux;
        if (pai == NULL) {
            *raiz = no23_criar(promove_aux, *raiz, maior);
            return NULL;
        }
    }
    return maior;
}

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info) {
    Data promove;
    return arvore23_inserir_no(raiz, info, NULL, &promove);
}

Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai) {
    while (raiz && !eh_folha(*raiz)) {
        *pai = raiz;
        raiz = raiz->esquerdo;
    }
    return raiz;
}

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Data **maior_valor) {
    while (raiz && !eh_folha(*raiz)) {
        *pai = raiz;
        if (raiz->n_infos == 1) {
            raiz = raiz->centro;
        } else {
            raiz = raiz->direito;
        }
    }

    if (raiz) {
        *maior_valor = no23_maior_info(raiz);
    }

    return raiz;
}

int arvore23_remover(Arvore23 **raiz, int info) {
    if (*raiz == NULL) return 0;

    // Caso seja folha
    if (eh_folha(**raiz)) {
        if (eh_info1(**raiz, info) || eh_info2(**raiz, info)) {
            if ((*raiz)->n_infos == 2) {
                if (eh_info1(**raiz, info)) {
                    (*raiz)->info1 = (*raiz)->info2;
                }
                (*raiz)->n_infos = 1;
            } else {
                no23_desalocar(raiz);
            }
            return 1;
        }
    }

    // Caso não seja folha
    Arvore23 *pai = NULL;
    Arvore23 *substituto = NULL;
    Data *maior_valor;

    if (info < (*raiz)->info1.numero_inicial) {
        substituto = arvore23_buscar_menor_filho((*raiz)->esquerdo, &pai);
    } else if ((*raiz)->n_infos == 1 || info < (*raiz)->info2.numero_inicial) {
        substituto = arvore23_buscar_menor_filho((*raiz)->centro, &pai);
    } else {
        substituto = arvore23_buscar_maior_filho((*raiz)->direito, &pai, &maior_valor);
    }

    if (substituto) {
        if (eh_info1(**raiz, info)) {
            (*raiz)->info1 = *maior_valor;
        } else {
            (*raiz)->info2 = *maior_valor;
        }
        return arvore23_remover(&substituto, maior_valor->numero_inicial);
    }

    return 0;
}

void arvore23_desalocar(Arvore23 **raiz) {
    if (*raiz == NULL) return;
    arvore23_desalocar(&((*raiz)->esquerdo));
    arvore23_desalocar(&((*raiz)->centro));
    if ((*raiz)->n_infos == 2) arvore23_desalocar(&((*raiz)->direito));
    no23_desalocar(raiz);
}

// ---------------------------- Exibição da Árvore ----------------------------
void no23_exibir(Data no) {
    printf("Bloco de [%d] até [%d] - [%s]\n", no.numero_inicial, no.numero_final, no.status == LIVRE ? "Livre" : "Ocupado");
}

void arvore23_exibir_pre(Arvore23 *raiz) {
    if (raiz == NULL) return;
    no23_exibir(raiz->info1);
    if (raiz->n_infos == 2) no23_exibir(raiz->info2);
    arvore23_exibir_pre(raiz->esquerdo);
    arvore23_exibir_pre(raiz->centro);
    if (raiz->n_infos == 2) arvore23_exibir_pre(raiz->direito);
}

void arvore23_exibir_ordem(Arvore23 *raiz) {
    if (raiz == NULL) return;
    arvore23_exibir_ordem(raiz->esquerdo);
    no23_exibir(raiz->info1);
    arvore23_exibir_ordem(raiz->centro);
    if (raiz->n_infos == 2) {
        no23_exibir(raiz->info2);
        arvore23_exibir_ordem(raiz->direito);
    }
}

void arvore23_exibir_pos(Arvore23 *raiz) {
    if (raiz == NULL) return;
    arvore23_exibir_pos(raiz->esquerdo);
    arvore23_exibir_pos(raiz->centro);
    if (raiz->n_infos == 2) arvore23_exibir_pos(raiz->direito);
    no23_exibir(raiz->info1);
    if (raiz->n_infos == 2) no23_exibir(raiz->info2);
}
