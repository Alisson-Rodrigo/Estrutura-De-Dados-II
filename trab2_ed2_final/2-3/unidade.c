#include <stdio.h>
#include <stdlib.h>
#include "unidade.h"

Unit *remove_unit(Unit *lista, int valor) {
    Unit *next = lista;       // Ponteiro para percorrer a lista
    Unit *prev = NULL;        // Ponteiro para o nó anterior
    Unit *result = lista; // Cabeça da lista, pode mudar se o primeiro nó for removido

    // Percorre a lista até encontrar o nó com o valor ou o final da lista
    while (next != NULL && next->unitValue != valor) {
        prev = next;
        next = next->nextNode;
    }

    // Se encontrou o nó, remove-o
    if (next != NULL) {
        if (prev == NULL) {
            // Se o nó a ser removido é o primeiro
            result = next->nextNode;
        } else {
            // Se o nó está no meio ou no final
            prev->nextNode = next->nextNode;
        }
        free(next); // Libera a memória do nó removido
    }

    return result; // Retorna a nova cabeça da lista
}


Unit *find_unit(Unit *lista, int valor) {
    Unit *aux = lista;

    while (aux != NULL && aux->unitValue != valor) {
        aux = aux->nextNode;
    }

    return aux;
}

void print_units(Unit *list) {
    Unit *aux = list;
    while (aux != NULL) {
        printf("Unidade: %d\n", aux->unitValue);
        aux = aux->nextNode;
    }
}

// Função para criar um novo nó de Unidade.
Unit *create_unit(int unitValue) {
    Unit *nova_unidade = (Unit *)malloc(sizeof(Unit));
    if (nova_unidade != NULL) {
        nova_unidade->unitValue = unitValue;
        nova_unidade->nextNode = NULL;
    }
    return nova_unidade;
}

// Função para inserir uma nova unidade ordenada.
Unit *insert_unit_sorted(Unit *unitList, Unit *new_unit) {
    Unit *head = unitList;  // Aponta para o início da lista
    Unit *previousUnit = NULL;
    Unit *currentUnit = unitList;

    if (unitList == NULL) {
        // Lista vazia, adiciona como primeiro elemento
        printf("Adicionando unidade %d como a primeira na lista.\n", new_unit->unitValue);
        new_unit->nextNode = NULL;
        head = new_unit;
    } else {
        // Percorre a lista até encontrar a posição correta ou verificar duplicata
        while (currentUnit != NULL && currentUnit->unitValue < new_unit->unitValue) {
            previousUnit = currentUnit;
            currentUnit = currentUnit->nextNode;
        }

        // Verifica se a unidade já existe
        if (currentUnit != NULL && currentUnit->unitValue == new_unit->unitValue) {
            printf("Unidade %d já existe na lista. Ignorando.\n", new_unit->unitValue);
            free(new_unit);
        } else if (previousUnit == NULL) {
            // Adiciona no início da lista
            new_unit->nextNode = unitList;
            head = new_unit;
            printf("Unidade %d adicionada no início da lista.\n", new_unit->unitValue);
        } else {
            // Adiciona no meio ou no final
            previousUnit->nextNode = new_unit;
            new_unit->nextNode = currentUnit;
            printf("Unidade %d adicionada na lista.\n", new_unit->unitValue);
        }
    }

    return head;  // Sempre retorna o início da lista
}



// Função para liberar a memória da lista.
void free_list(Unit *unitList) {
    Unit *currentUnit = unitList;
    while (currentUnit != NULL) {
        Unit *temp = currentUnit;
        currentUnit = currentUnit->nextNode;
        free(temp);
    }
}
