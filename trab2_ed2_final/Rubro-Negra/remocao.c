
#include "remocao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Remove_palavra_ingles_unidade(PortuguesRB **raiz, const char *palavras_ingles, const char *unidade)
{
  int confirm = 1;
  confirm = remove_palavra_ingles_unidade(*raiz, palavras_ingles, unidade, raiz);
  return confirm;
}

int remove_unidade(Inglesbin **raiz, const char *palavraIngles, const char *unidade)
{
  int confirm = 1;

  if (*raiz)
  {
    if (strcmp((*raiz)->palavraIngles, palavraIngles) == 0)
    {
      confirm = remover_lista_encadeada_unidade(&(*raiz)->unidades, unidade);
      if (!(*raiz)->unidades)
        confirm = removerPalavraIngles(raiz, palavraIngles);
    }
    else if (strcmp((*raiz)->palavraIngles, palavraIngles) > 0)
      confirm = remove_unidade(&(*raiz)->esq, palavraIngles, unidade);
    else
      confirm = remove_unidade(&(*raiz)->dir, palavraIngles, unidade);
  }

  return confirm;
}

int remove_palavra_ingles_unidade(PortuguesRB *raiz, const char *palavraIngles, const char *unidade, PortuguesRB **top)
{
  int confirm = 0;

  if (raiz)
  {
    confirm = remove_palavra_ingles_unidade((raiz)->esq, palavraIngles, unidade, top);


    confirm = remove_palavra_ingles_unidade((raiz)->dir, palavraIngles, unidade, top);

    confirm = remove_unidade(&(raiz)->info.palavraIngles, palavraIngles, unidade);


    if (!(raiz)->info.palavraIngles)
      confirm = removerArvRB(top, raiz->info.palavraPortugues);

  }

  return confirm;
}

int _remove_palavra_portugues_unidade(Inglesbin **raiz, const char *palavraPortugues, const char *unidade)
{
  int confirm = 0;
  if (*raiz)
  {
    confirm = _remove_palavra_portugues_unidade(&(*raiz)->esq, palavraPortugues, unidade);
    confirm = _remove_palavra_portugues_unidade(&(*raiz)->dir, palavraPortugues, unidade) || confirm;
    confirm = remover_lista_encadeada_unidade(&(*raiz)->unidades, unidade);

    if (!(*raiz)->unidades)
      confirm = removerPalavraIngles(raiz, (*raiz)->palavraIngles);
  }

  return confirm;
}

int remove_palavra_portugues_unidade(PortuguesRB *raiz, char *palavraPortugues, const char *unidade, PortuguesRB **top)
{
  int confirm = 1;

  if (raiz)
  {
    if (strcmp((raiz)->info.palavraPortugues, palavraPortugues) == 0)
    {
      confirm = _remove_palavra_portugues_unidade(&(raiz)->info.palavraIngles, palavraPortugues, unidade);
      if (!(raiz)->info.palavraIngles)
        confirm = removerArvRB(top, palavraPortugues);
    }
    else if (strcmp(palavraPortugues, (raiz)->info.palavraPortugues) < 0)
      confirm = remove_palavra_portugues_unidade((raiz)->esq, palavraPortugues, unidade, top);
    else
      confirm = remove_palavra_portugues_unidade((raiz)->dir, palavraPortugues, unidade, top);
  }

  return confirm;
}

int Remove_palavra_portugues_unidade(PortuguesRB **raiz, char *palavraPortugues, const char *unidade)
{
  int confirm = 1;
  confirm = remove_palavra_portugues_unidade(*raiz, palavraPortugues, unidade, raiz);
  return confirm;
}