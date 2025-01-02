#ifndef UNIDADE_H
#define UNIDADE_H

// Estrutura para a lista de unidades
typedef struct unidade {
    int unidade;              // Valor da unidade
    struct unidade *prox;     // Ponteiro para o próximo nó da lista
} Unidade;

// Função para criar um novo nó de Unidade
Unidade *criar_unidade(int valor);

// Função para adicionar uma nova unidade de forma ordenada na lista
Unidade *adicionar_unidade_ordenada(Unidade *lista, Unidade *novo_no);

// Função para remover uma unidade da lista
Unidade *remover_unidade(Unidade *lista, int valor);

// Função para buscar uma unidade na lista
Unidade *buscar_unidade(Unidade *lista, int valor);

// Função para imprimir todas as unidades da lista
void imprimir_unidades(Unidade *lista);

// Função para liberar a memória alocada pela lista de unidades
void liberar_lista(Unidade *lista);

#endif // UNIDADE_H
