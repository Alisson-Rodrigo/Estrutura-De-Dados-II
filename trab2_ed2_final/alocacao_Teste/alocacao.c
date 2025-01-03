#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int inicializar_blocos(Arvore23 **arvore, int maximo) {
    int status;

    // Solicita o estado inicial do nó
    do {
        printf("\nO primeiro nó está livre ou ocupado?");
        printf("\n[%d] - %s", LIVRE, "Livre");
        printf("\n[%d] - %s", OCUPADO, "Ocupado");
        printf("\nStatus: ");
        scanf("%d", &status);
        while (getchar() != '\n'); // Limpa o buffer
        if (status != LIVRE && status != OCUPADO) {
            printf("\nDigite uma opção válida!\n");
        }
    } while (status != LIVRE && status != OCUPADO);

    Data no;
    printf("\nEndereço inicial: ");
    scanf("%d", &no.numero_inicial);
    while (getchar() != '\n'); // Limpa o buffer
    if (no.numero_inicial < 0 || no.numero_inicial >= maximo) {
        printf("\nDigite um valor entre 0 e %d.\n", maximo - 1);
    }

    int minimo = no.numero_inicial;

    // Solicita os nós subsequentes
    do {
        no.status = status;

        do {
            printf("\nEndereço final: ");
            scanf("%d", &no.numero_final);
            while (getchar() != '\n'); // Limpa o buffer
            if (no.numero_final < no.numero_inicial || no.numero_final >= maximo) {
                printf("\nDigite um valor entre %d e %d.\n", no.numero_inicial, maximo - 1);
            }
        } while (no.numero_final < no.numero_inicial || no.numero_final >= maximo);

        arvore23_inserir(arvore, no);
        no.numero_inicial = no.numero_final + 1;
        status = !status; // Alterna entre LIVRE e OCUPADO
    } while (no.numero_final < (maximo - 1));

    return minimo;
}

Arvore23 *bloco_menor(Arvore23 **raiz, Arvore23 *no, Data *info, Data **valor_menor)
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

Arvore23 *bloco_maior(Arvore23 **raiz, Arvore23 *no, Data *info, Data **valor_maior)
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



Arvore23 *procurar_no(Arvore23 **arvore, int quant, int status, Data **info_escolhido) {
    Arvore23 *no;
    if (*arvore != NULL) {
        no = procurar_no(&((*arvore)->esquerdo), quant, status, info_escolhido);

        if (*info_escolhido == NULL) {
            if ((*arvore)->info1.status == status &&
                ((*arvore)->info1.numero_final - (*arvore)->info1.numero_inicial + 1) >= quant) {
                *info_escolhido = &((*arvore)->info1);
                no = *arvore;
            } else {
                no = procurar_no(&((*arvore)->centro), quant, status, info_escolhido);
                if ((*arvore)->n_infos == 2) {
                    if ((*arvore)->info2.status == status &&
                        ((*arvore)->info2.numero_final - (*arvore)->info2.numero_inicial + 1) >= quant) {
                        *info_escolhido = &((*arvore)->info2);
                        no = *arvore;
                    } else if (*info_escolhido == NULL)
                        no = procurar_no(&((*arvore)->direito), quant, status, info_escolhido);
                }
            }
        }
    } else
        *info_escolhido = NULL;

    return no;
}

void juntar_no(Arvore23 **raiz, int *numero_final, int limite, int valor_remover) {
    *numero_final = limite;
    arvore23_remover(raiz, valor_remover);
}

void alterar_no_arvore(Arvore23 **raiz, Arvore23 *no, Data *info, int quant) {
    Arvore23 *menor;
    Data *valor_menor = NULL;

    menor = bloco_menor(raiz, no, info, &valor_menor);

    if (quant < (info->numero_final - info->numero_inicial + 1)) {
        if (menor == NULL) {
            Data novo_bloco;
            novo_bloco.numero_inicial = info->numero_inicial;
            novo_bloco.numero_final = info->numero_inicial + quant - 1;
            novo_bloco.status = !(info->status);

            info->numero_inicial += quant;
            arvore23_inserir(raiz, novo_bloco);
        } else {
            valor_menor->numero_final += quant;
            info->numero_inicial += quant;
        }
    } else {
        Arvore23 *maior;
        Data *valor_maior = NULL;

        maior = bloco_maior(raiz, no, info, &valor_maior);

        if (menor == NULL && maior == NULL) {
            info->status = !(info->status);
        } else {
            if (menor == NULL) {
                info->status = !(info->status);
                juntar_no(raiz, &(info->numero_final), valor_maior->numero_final, valor_maior->numero_inicial);
            } else if (maior == NULL) {
                juntar_no(raiz, &(valor_menor->numero_final), info->numero_final, info->numero_inicial);
            } else {
                int numero = valor_maior->numero_inicial;
                juntar_no(raiz, &(valor_menor->numero_final), valor_maior->numero_final, info->numero_inicial);
                arvore23_remover(raiz, numero);
            }
        }
    }
}

int gerenciar_bloco_memoria(Arvore23 **estrutura, int quantidade, int estado) {
    Data *bloco_selecionado = NULL;
    Arvore23 *nodo_encontrado = procurar_no(estrutura, quantidade, estado, &bloco_selecionado);

    if (bloco_selecionado != NULL) {
        printf("\nBloco selecionado:\n");
        no23_exibir(*bloco_selecionado);
        alterar_no_arvore(estrutura, nodo_encontrado, bloco_selecionado, quantidade);
    } else {
        printf("\nNenhum bloco disponível para os critérios informados.\n");
    }

    return 1;
}

