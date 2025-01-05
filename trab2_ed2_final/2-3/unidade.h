#ifndef UNIDADE_H
#define UNIDADE_H

// Estrutura para a lista de unidades
typedef struct Unit {
    int unitValue;              // Valor da unidade
    struct Unit *nextNode;     // Ponteiro para o próximo nó da lista
} Unit;

// Função para criar um novo nó de Unidade
Unit *create_unit(int valor);

// Função para adicionar uma nova unidade de forma ordenada na lista
Unit *insert_unit_sorted(Unit *lista, Unit *novo_no);

// Função para remover uma unidade da lista
Unit *remove_unit(Unit *lista, int valor);

// Função para buscar uma unidade na lista
Unit *find_unit(Unit *lista, int valor);

// Função para imprimir todas as unidades da lista
void print_units(Unit *lista);

// Função para liberar a memória alocada pela lista de unidades
void free_list(Unit *lista);

#endif // UNIDADE_H
