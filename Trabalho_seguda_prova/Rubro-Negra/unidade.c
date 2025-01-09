#include <stdio.h>
#include <stdlib.h>
#include "unidade.h"

Unit *remove_unit(Unit *unitList, int unitValue) {
    Unit *aux = unitList;
    Unit *ant = NULL;
    Unit *unitListHead = unitList;  

    while (aux != NULL && aux->unitValue != unitValue) {
        ant = aux;
        aux = aux->nextNode;
    }

    if (aux != NULL) {
        if (ant == NULL) {
            unitListHead = aux->nextNode;
        } else {
            ant->nextNode = aux->nextNode;
        }
        free(aux);
    }

    return unitListHead;
}

Unit *find_unit(Unit *unitList, int unit) {
    Unit *aux = unitList;

    while (aux != NULL && aux->unitValue != unit) {
        aux = aux->nextNode;
    }

    return aux;
}

void print_units(Unit *unitList) {
    Unit *aux = unitList;
    while (aux != NULL) {
        printf("Unidade: %d\n", aux->unitValue);
        aux = aux->nextNode;
    }
}


Unit *create_unit(int unit) {
    Unit *new_unit = (Unit *)malloc(sizeof(Unit));
    if (new_unit != NULL) {
        new_unit->unitValue = unit;
        new_unit->nextNode = NULL;
    }
    return new_unit;
}

Unit *insert_unit_sorted(Unit *unitList, Unit *new_unit) {
    Unit *aux = unitList;

    if (unitList == NULL) {

        new_unit->nextNode = NULL;
        aux = new_unit;
    } else {
        Unit *anterior = NULL;
        Unit *atual = unitList;

  
        while (atual != NULL && atual->unitValue < new_unit->unitValue) {
            anterior = atual;
            atual = atual->nextNode;
        }

        if (anterior == NULL) {
            
            new_unit->nextNode = unitList;
            aux = new_unit;
        } else {
         
            anterior->nextNode = new_unit;
            new_unit->nextNode = atual;
        }
    }

    return aux;
}


void free_list(Unit *unitList) {
    Unit *atual = unitList;
    while (atual != NULL) {
        Unit *temp = atual;
        atual = atual->nextNode;
        free(temp);
    }
}
