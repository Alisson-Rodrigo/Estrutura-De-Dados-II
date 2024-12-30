#pragma once
#include "structs.h"
#include "arvrb.h"
#include "arvbin.h"
#include "lista_encadeada.h"

int Remove_palavra_ingles_unidade(PortuguesRB **raiz, const char *palavras_ingles, const char *unidade);

int remove_unidade(Inglesbin **raiz, const char *palavra_ingles, const char *unidade);

int remove_palavra_ingles_unidade(PortuguesRB *raiz, const char *palavra_ingles, const char *unidade, PortuguesRB **top);

int _remove_palavra_portugues_unidade(Inglesbin **raiz, const char *palavraPortugues, const char *unidade);

int remove_palavra_portugues_unidade(PortuguesRB *raiz, char *palavraPortugues, const char *unidade, PortuguesRB **top);

int Remove_palavra_portugues_unidade(PortuguesRB **raiz, char *palavraPortugues, const char *unidade);
