#include <stdio.h>
#include <stdlib.h>
#include "23.h"
#include "aux_cod.c"

int cadastrar_nos(Arvore23 **arvore, int maximo)
{
    int status;

    do
    {
        printf("\nO primeiro nó está livre ou ocupado?");
        printf("\n[%d] - %s", LIVRE, "Livre");
        printf("\n[%d] - %s", OCUPADO, "Ocupado");
        leia_int("\nStatus: ", &status);

        if(status != LIVRE && status != OCUPADO)
            printf("\nDigite uma opção válida!\n");
    } while(status != LIVRE && status != OCUPADO);

    Data no;
    leia_numero_no("\nEndereço inicial: ", &no.numero_inicial, 0, maximo);
    int minimo = no.numero_inicial;
    do
    {
        no.status = status;
        leia_numero_no("\nEndereço final: ", &no.numero_final, no.numero_inicial, maximo);

        arvore23_inserir(arvore, no);

        no.numero_inicial = no.numero_final + 1;
        status = !status;
    }while(no.numero_final < (maximo - 1));

    return minimo;
}

Arvore23 *buscar_no_memoria(Arvore23 **arvore, int quant, int status, Data **info_escolhido)
{
    Arvore23 *no;
    if(*arvore != NULL)
    {
        no = buscar_no_memoria(&((*arvore)->esquerdo), quant, status, info_escolhido);

        if(*info_escolhido == NULL)
        {
            if((*arvore)->info1.status == status && quantidade_disponivel((*arvore)->info1) >= quant)
            {
                *info_escolhido = &((*arvore)->info1);
                no = *arvore;
            }
            else
            {
                no = buscar_no_memoria(&((*arvore)->centro), quant, status, info_escolhido);
                if((*arvore)->n_infos == 2)
                {
                    if((*arvore)->info2.status == status && quantidade_disponivel((*arvore)->info2) >= quant)
                    {
                        *info_escolhido = &((*arvore)->info2);
                        no = *arvore;
                    }
                    else if(*info_escolhido == NULL)
                        no = buscar_no_memoria(&((*arvore)->direito), quant, status, info_escolhido);
                }
            }
        }
    }
    else
        *info_escolhido = NULL;

    return no;
}

void concatenar_no(Arvore23 **raiz, int *numero_final, int limite, int valor_remover)
{
    *numero_final = limite;
    arvore23_remover(raiz, valor_remover);
}

Arvore23 *buscar_menor_bloco(Arvore23 **raiz, Arvore23 *no, Data *info, Data **valor_menor)
{
    Arvore23 *menor, *pai;
    *valor_menor = NULL;

    if(eh_folha(*no))
    {
        if(no->info1.numero_inicial != info->numero_inicial)
            menor = no;
        else
            menor = arvore23_buscar_menor_pai(*raiz, info->numero_inicial);

        if(menor != NULL)
        {
            if(menor->n_infos == 2 && menor->info2.numero_inicial < info->numero_inicial)
                *valor_menor = &(menor->info2);
            else
                *valor_menor = &(menor->info1);
        }
    }
    else if(no->info1.numero_inicial == info->numero_inicial)
        menor = arvore23_buscar_maior_filho(no->esquerdo, &pai, valor_menor);
    else
        menor = arvore23_buscar_maior_filho(no->centro, &pai, valor_menor);

    return menor;
}

Arvore23 *buscar_maior_bloco(Arvore23 **raiz, Arvore23 *no, Data *info, Data **valor_maior)
{
    Arvore23 *maior;
    Arvore23 *pai;
    *valor_maior = NULL;

    if(eh_folha(*no))
    {
        if(no->n_infos == 2 && no->info1.numero_inicial == info->numero_inicial)
            maior = no;
        else
            maior = arvore23_buscar_maior_pai(*raiz, info->numero_inicial);

        if(maior != NULL)
        {
            if(maior->info1.numero_inicial > info->numero_inicial)
                *valor_maior = &(maior->info1);
            else
                *valor_maior = &(maior->info2);
        }
    }
    else
    {
        if(no->info1.numero_inicial == info->numero_inicial)
            maior = arvore23_buscar_menor_filho(no->centro, &pai);
        else
            maior = arvore23_buscar_menor_filho(no->direito, &pai);

        if(maior != NULL)
            *valor_maior = &(maior->info1);
    }

    return maior;
}

void modificar_no(Arvore23 **raiz, Arvore23 *no, Data *info, int quant)
{
    Arvore23 *menor;
    Data *valor_menor;

    menor = buscar_menor_bloco(raiz, no, info, &valor_menor);

    if(quant < quantidade_disponivel(*info))
    {
        if(menor == NULL)
        {
            Data data;
            data.numero_inicial = info->numero_inicial;
            data.numero_final = info->numero_inicial + quant - 1;
            data.status = !(info->status);

            info->numero_inicial += quant;
            arvore23_inserir(raiz, data);
        }
        else
        {
            valor_menor->numero_final += quant;
            info->numero_inicial += quant;
        }
    }
    else
    {
        Arvore23 *maior;
        Data *valor_maior;

        maior = buscar_maior_bloco(raiz, no, info, &valor_maior);

        if(menor == NULL && maior == NULL)
            info->status = !(info->status);
        else
        {
            if(menor == NULL)
            {
                info->status = !(info->status);
                concatenar_no(raiz, &(info->numero_final), valor_maior->numero_final, valor_maior->numero_inicial);
            }
            else if(maior == NULL)
                concatenar_no(raiz, &(valor_menor->numero_final), info->numero_final, info->numero_inicial);
            else
            {
                int numero = valor_maior->numero_inicial;
                concatenar_no(raiz, &(valor_menor->numero_final), valor_maior->numero_final, info->numero_inicial);
                arvore23_remover(raiz, numero);
            }
        }
    }
}

int alocar_desalocar_no(Arvore23 **arvore, int quant_nos, int status)
{
    Data *info_escolhido;
    info_escolhido = NULL;
    Arvore23 *no_escolhido;
    no_escolhido = buscar_no_memoria(arvore, quant_nos, status, &info_escolhido);

    if(info_escolhido != NULL)
    {
        printf("\nNó escolhido: \n");
        no23_exibir(*info_escolhido);

        modificar_no(arvore, no_escolhido, info_escolhido, quant_nos);
    }
    else
        printf("\nNão há nó disponível\n");

    return 1;
}