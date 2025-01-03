#include <stdio.h>
#include <stdlib.h>
#include "23.h"

void limpa_buffer()
{
    while(getchar() != '\n');
}

void leia_int(char *texto, int *variavel)
{
    printf("%s", texto);
    scanf("%d", variavel);
    limpa_buffer();
}

void leia_numero_no(char *texto, int *variavel, int minimo, int maximo)
{
    do
    {
        leia_int(texto, variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um número entre %d e %d\n", minimo, maximo-1);
    } while(*variavel < minimo || *variavel >= maximo);
}

int quantidade_disponivel(Data info)
{
    return info.numero_final - info.numero_inicial + 1;
}
