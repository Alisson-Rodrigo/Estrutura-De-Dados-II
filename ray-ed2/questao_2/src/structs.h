#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct InglesBin
{
  char palavraIngles[50];
  int unidade;
  struct InglesBin *esq;
  struct InglesBin *dir;
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