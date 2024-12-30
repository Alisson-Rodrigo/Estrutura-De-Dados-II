#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct listaEncadeadaUnidade
{
  char *nome_unidade;
  struct listaEncadeadaUnidade *prox;
} ListaEncadeadaUnidade;

typedef struct Inglesbin
{
  ListaEncadeadaUnidade *unidades;
  char *palavraIngles;
  struct Inglesbin *dir, *esq;
} Inglesbin;

typedef struct Info
{
  Inglesbin *palavraIngles;
  char *palavraPortugues;
} Info;

typedef struct PortuguesRB
{
  Info info;
  int cor;
  struct PortuguesRB *esq;
  struct PortuguesRB *dir;
} PortuguesRB;

#endif // STRUCTS_H