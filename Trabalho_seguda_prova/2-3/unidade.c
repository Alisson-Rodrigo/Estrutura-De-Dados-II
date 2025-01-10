#include <stdio.h>
#include <stdlib.h>
#include "unidade.h"

int remove_unit(Unit **lista, int nome) {
    int confirmacao = 0;

    if (*lista) {
        if ((*lista)->unitValue == nome) {
            Unit *aux = *lista;
            *lista = (*lista)->nextNode;
            // Não liberar `unitValue` se não foi alocado dinamicamente
            free(aux);
            confirmacao = 1;
        } else {
            confirmacao = remove_unit(&(*lista)->nextNode, nome);
        }
    }

    return confirmacao;
}


Unit *find_unit(Unit *unitList, int unitValue) {
    Unit *aux = unitList;

    while (aux != NULL && aux->unitValue != unitValue) {
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


Unit *create_unit(int unitValue) {
    Unit *nova_unidade = (Unit *)malloc(sizeof(Unit));
    if (nova_unidade != NULL) {
        nova_unidade->unitValue = unitValue;
        nova_unidade->nextNode = NULL;
    }
    return nova_unidade;
}


Unit *insert_unit_sorted(Unit *unitList, Unit *new_unit) {
    Unit *head = unitList;  
    Unit *previousUnit = NULL;
    Unit *currentUnit = unitList;

    if (unitList == NULL) {
      
        new_unit->nextNode = NULL;
        head = new_unit;
    } else {
 
        while (currentUnit != NULL && currentUnit->unitValue < new_unit->unitValue) {
            previousUnit = currentUnit;
            currentUnit = currentUnit->nextNode;
        }

       
        if (currentUnit != NULL && currentUnit->unitValue == new_unit->unitValue) {
         
            free(new_unit);
        } else if (previousUnit == NULL) {
          
            new_unit->nextNode = unitList;
            head = new_unit;
         
        } else {
       
            previousUnit->nextNode = new_unit;
            new_unit->nextNode = currentUnit;
        }
    }

    return head;  
}




void free_list(Unit *unitList) {
    Unit *currentUnit = unitList;
    while (currentUnit != NULL) {
        Unit *temp = currentUnit;
        currentUnit = currentUnit->nextNode;
        free(temp);
    }
}
