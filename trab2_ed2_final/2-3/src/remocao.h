#pragma once
#include "structs.h"
#include "arv23.h"
#include "arvbin.h"
#include "lista_encadeada.h"

int Remove_palavra_ingles_unidade(Portugues23 **raiz, const char *palavras_ingles, const char *unidade);

int remove_unidade(Inglesbin **raiz, const char *palavra_ingles, const char *unidade);

int remove_palavra_ingles_unidade(Portugues23 *raiz, const char *palavra_ingles, const char *unidade, Portugues23 **top);

int _remove_palavra_portugues_unidade(Inglesbin **raiz, const char *palavraPortugues, const char *unidade);

int remove_palavra_portugues_unidade(Portugues23 *raiz, char *palavraPortugues, const char *unidade, Portugues23 **top);

int Remove_palavra_portugues_unidade(Portugues23 **raiz, char *palavraPortugues, const char *unidade);
