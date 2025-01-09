#pragma once
#include "structs.h"
void aloca_lista_encadeada_unidade(ListaEncadeadaUnidade **lista);
void def_lista_encadeada_unidade(ListaEncadeadaUnidade *lista, const char *nome_unidade);
int buscar_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista, const char *nome_unidade);
void show_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista);
void free_lista_encadeada_unidade(ListaEncadeadaUnidade *lista);
int inserir_lista_encadeada_unidade(ListaEncadeadaUnidade **lista, char *nome);
int remover_lista_encadeada_unidade(ListaEncadeadaUnidade **lista, const char *nome);