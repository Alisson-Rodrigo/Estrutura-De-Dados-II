#include <stdio.h>
#include <stdlib.h>
#include "23.h"

// Função auxiliar para calcular altura da árvore
static int calcular_altura(Arvore23 *no) {
    return (no == NULL) ? -1 : 1 + calcular_altura(no->esquerdo);
}

// Verifica se é possível remover nós mantendo as propriedades da árvore 2-3
static int possivel_remover(Arvore23 *raiz) {
    if (raiz == NULL) return 0;
    if (raiz->n_infos == 2) return 1;
    return possivel_remover(raiz->centro) || possivel_remover(raiz->esquerdo);
}

// Funções de alocação e desalocação de nós
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

// Criação de um nó da árvore 2-3
Arvore23 *no23_criar(Data info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro) {
    Arvore23 *no = no23_alocar();
    no->info1 = info;
    no->esquerdo = filho_esquerdo;
    no->centro = filho_centro;
    no->direito = NULL;
    no->n_infos = 1;
    return no;
}

// Função para quebrar um nó com 2 informações
static Arvore23 *no23_quebrar(Arvore23 *no, Data info, Data *promove, Arvore23 *filho_maior) {
    Arvore23 *novo_no;
    if (info.numero_inicial > no->info2.numero_inicial) {
        *promove = no->info2;
        novo_no = no23_criar(info, no->direito, filho_maior);
    } else if (info.numero_inicial > no->info1.numero_inicial) {
        *promove = info;
        novo_no = no23_criar(no->info2, filho_maior, no->direito);
    } else {
        *promove = no->info1;
        novo_no = no23_criar(no->info2, no->centro, no->direito);
        no->info1 = info;
        no->centro = filho_maior;
    }
    no->n_infos = 1;
    return novo_no;
}

// Adiciona informações a um nó existente
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

// Criação de uma nova árvore
Arvore23 *arvore23_criar() {
    return NULL;
}

// Inserção de um nó na árvore 2-3
Arvore23 *arvore23_inserir_no(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove) {
    if (*raiz == NULL) {
        *raiz = no23_criar(info, NULL, NULL);
        return NULL;
    }
    if (eh_folha(**raiz)) {
        if ((*raiz)->n_infos == 1) {
            no23_adicionar_info(*raiz, info, NULL);
            return NULL;
        } else {
            Arvore23 *novo_no = no23_quebrar(*raiz, info, promove, NULL);
            if (pai == NULL) {
                *raiz = no23_criar(*promove, *raiz, novo_no);
                return NULL;
            }
            return novo_no;
        }
    }

    Arvore23 *novo_no = NULL;
    if (info.numero_inicial < (*raiz)->info1.numero_inicial) {
        novo_no = arvore23_inserir_no(&((*raiz)->esquerdo), info, *raiz, promove);
    } else if ((*raiz)->n_infos == 1 || info.numero_inicial < (*raiz)->info2.numero_inicial) {
        novo_no = arvore23_inserir_no(&((*raiz)->centro), info, *raiz, promove);
    } else {
        novo_no = arvore23_inserir_no(&((*raiz)->direito), info, *raiz, promove);
    }

    if (novo_no) {
        if ((*raiz)->n_infos == 1) {
            no23_adicionar_info(*raiz, *promove, novo_no);
            return NULL;
        }
        Arvore23 *split_no = no23_quebrar(*raiz, *promove, promove, novo_no);
        if (pai == NULL) {
            *raiz = no23_criar(*promove, *raiz, split_no);
            return NULL;
        }
        return split_no;
    }
    return NULL;
}

// Inserção principal
Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info) {
    Data promove;
    return arvore23_inserir_no(raiz, info, NULL, &promove);
}

// Funções para exibição da árvore
void arvore23_exibir_pre(Arvore23 *raiz) {
    if (raiz == NULL) return;
    printf("[1º] Bloco: [%d-%d]\n", raiz->info1.numero_inicial, raiz->info1.numero_final);
    if (raiz->n_infos == 2) {
        printf("[2º] Bloco: [%d-%d]\n", raiz->info2.numero_inicial, raiz->info2.numero_final);
    }
    arvore23_exibir_pre(raiz->esquerdo);
    arvore23_exibir_pre(raiz->centro);
    if (raiz->n_infos == 2) arvore23_exibir_pre(raiz->direito);
}
