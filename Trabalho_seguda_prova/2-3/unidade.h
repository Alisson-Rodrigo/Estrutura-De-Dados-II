#ifndef UNIDADE_H
#define UNIDADE_H


typedef struct Unit {
    int unitValue;              
    struct Unit *nextNode;     
} Unit;


Unit *create_unit(int valor);


Unit *insert_unit_sorted(Unit *lista, Unit *novo_no);



Unit *find_unit(Unit *lista, int valor);


void print_units(Unit *lista);


void free_list(Unit *lista);
int remove_unit(Unit **lista, int nome);

#endif 
