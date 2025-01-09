#include <stdio.h>
#include <stdlib.h>
#include "unidade.h"

Unit *remove_unit(Unit *unitList, int unitValue) {
    Unit *next = unitList;       
    Unit *prev = NULL;        
    Unit *result = unitList; 

 
    while (next != NULL && next->unitValue != unitValue) {
        prev = next;
        next = next->nextNode;
    }


    if (next != NULL) {
        if (prev == NULL) {
          
            result = next->nextNode;
        } else {
       
            prev->nextNode = next->nextNode;
        }
        free(next); 
    }

    return result; 
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
      
        printf("Adicionando unidade %d como a primeira na lista.\n", new_unit->unitValue);
        new_unit->nextNode = NULL;
        head = new_unit;
    } else {
 
        while (currentUnit != NULL && currentUnit->unitValue < new_unit->unitValue) {
            previousUnit = currentUnit;
            currentUnit = currentUnit->nextNode;
        }

       
        if (currentUnit != NULL && currentUnit->unitValue == new_unit->unitValue) {
            printf("Unidade %d já existe na lista. Ignorando.\n", new_unit->unitValue);
            free(new_unit);
        } else if (previousUnit == NULL) {
          
            new_unit->nextNode = unitList;
            head = new_unit;
            printf("Unidade %d adicionada no início da lista.\n", new_unit->unitValue);
        } else {
       
            previousUnit->nextNode = new_unit;
            new_unit->nextNode = currentUnit;
            printf("Unidade %d adicionada na lista.\n", new_unit->unitValue);
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
