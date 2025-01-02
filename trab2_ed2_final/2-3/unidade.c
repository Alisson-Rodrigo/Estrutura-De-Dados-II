#include <stdio.h>
#include <stdlib.h>
#include "unidade.h"

Unidade *remover_unidade(Unidade *lista, int valor) {
    Unidade *aux = lista;
    Unidade *ant = NULL;
    Unidade *resultado = lista;  

    while (aux != NULL && aux->unidade != valor) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux != NULL) {
        if (ant == NULL) {
            resultado = aux->prox;
        } else {
            ant->prox = aux->prox;
        }
        free(aux);
    }

    return resultado;
}

Unidade *buscar_unidade(Unidade *lista, int valor) {
    Unidade *aux = lista;

    while (aux != NULL && aux->unidade != valor) {
        aux = aux->prox;
    }

    return aux;
}

void imprimir_unidades(Unidade *lista) {
    Unidade *aux = lista;
    while (aux != NULL) {
        printf("Unidade: %d\n", aux->unidade);
        aux = aux->prox;
    }
}

// Função para criar um novo nó de Unidade.
Unidade *criar_unidade(int valor) {
    Unidade *nova_unidade = (Unidade *)malloc(sizeof(Unidade));
    if (nova_unidade != NULL) {
        nova_unidade->unidade = valor;
        nova_unidade->prox = NULL;
    }
    return nova_unidade;
}

// Função para inserir uma nova unidade ordenada.
Unidade *adicionar_unidade_ordenada(Unidade *lista, Unidade *novo_no) {
    Unidade *aux = lista;

    if (lista == NULL) {
        // Caso a lista esteja vazia, o novo nó se torna o primeiro.
        novo_no->prox = NULL;
        aux = novo_no;
    } else {
        Unidade *anterior = NULL;
        Unidade *atual = lista;

        // Percorre a lista para encontrar a posição correta (ordem crescente de "unidade").
        while (atual != NULL && atual->unidade < novo_no->unidade) {
            anterior = atual;
            atual = atual->prox;
        }

        // Verifica se a unidade já existe para evitar duplicatas.
        if (atual != NULL && atual->unidade == novo_no->unidade) {
            free(novo_no); // Libera o novo nó, pois já existe na lista.
        } else if (anterior == NULL) {
            // Insere o novo nó no início da lista.
            novo_no->prox = lista;
            aux = novo_no;
        } else {
            // Insere o novo nó no meio ou no final da lista.
            anterior->prox = novo_no;
            novo_no->prox = atual;
        }
    }

    return aux;
}

// Função para liberar a memória da lista.
void liberar_lista(Unidade *lista) {
    Unidade *atual = lista;
    while (atual != NULL) {
        Unidade *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}
