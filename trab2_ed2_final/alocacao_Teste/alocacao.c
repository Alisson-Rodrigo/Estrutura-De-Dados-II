#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int inicializar_blocos(Arvore23 **estrutura_memoria, int capacidade_maxima) {
    int estado_inicial;

    // Solicitação do estado inicial
    do {
        printf("\n=== Configuração do Bloco Inicial ===\n");
        printf("Escolha o estado do bloco inicial:\n");
        printf("  [%d] -> Disponível\n", LIVRE);
        printf("  [%d] -> Ocupado\n", OCUPADO);
        printf("Digite sua escolha: ");
        scanf("%d", &estado_inicial);
        while (getchar() != '\n'); // Limpeza do buffer
        if (estado_inicial != LIVRE && estado_inicial != OCUPADO) {
            printf("\n[Erro] Escolha inválida. Por favor, tente novamente.\n");
        }
    } while (estado_inicial != LIVRE && estado_inicial != OCUPADO);

    Data bloco;
    // Solicitação do endereço inicial
    do {
        printf("\n>>> Insira o endereço inicial do bloco: ");
        scanf("%d", &bloco.numero_inicial);
        while (getchar() != '\n'); // Limpeza do buffer
        if (bloco.numero_inicial < 0 || bloco.numero_inicial >= capacidade_maxima) {
            printf("\n[Erro] O endereço inicial deve estar entre 0 e %d.\n", capacidade_maxima - 1);
        }
    } while (bloco.numero_inicial < 0 || bloco.numero_inicial >= capacidade_maxima);

    int endereco_minimo = bloco.numero_inicial;

    // Configuração dos blocos subsequentes
    do {
        bloco.status = estado_inicial;

        do {
            printf("\n>>> Insira o endereço final do bloco (deve ser >= %d): ", bloco.numero_inicial);
            scanf("%d", &bloco.numero_final);
            while (getchar() != '\n'); // Limpeza do buffer
            if (bloco.numero_final < bloco.numero_inicial || bloco.numero_final >= capacidade_maxima) {
                printf("\n[Erro] O endereço final deve estar entre %d e %d.\n", bloco.numero_inicial, capacidade_maxima - 1);
            }
        } while (bloco.numero_final < bloco.numero_inicial || bloco.numero_final >= capacidade_maxima);

        // Inserção do bloco na estrutura
        arvore23_inserir(estrutura_memoria, bloco);

        // Atualização do próximo bloco
        bloco.numero_inicial = bloco.numero_final + 1;
        estado_inicial = !estado_inicial; // Alterna entre Livre e Ocupado
    } while (bloco.numero_final < (capacidade_maxima - 1));

    printf("\n[Sucesso] Blocos inicializados com sucesso!\n");
    return endereco_minimo;
}


void juntar_no(Arvore23 **raiz, int *numero_final, int limite, int valor_remover) {
    *numero_final = limite;
    arvore23_remover(raiz, valor_remover);
}

Arvore23 *bloco_menor(Arvore23 **estrutura, Arvore23 *nodo, Data *info_dados, Data **menor_valor) {
    Arvore23 *menor_bloco, *pai_temporario;
    *menor_valor = NULL;

    if (eh_folha(*nodo)) {
        if (nodo->info1.numero_inicial != info_dados->numero_inicial) {
            menor_bloco = nodo;
        } else {
            menor_bloco = arvore23_buscar_menor_pai(*estrutura, info_dados->numero_inicial);
        }

        if (menor_bloco != NULL) {
            if (menor_bloco->n_infos == 2 && menor_bloco->info2.numero_inicial < info_dados->numero_inicial) {
                *menor_valor = &(menor_bloco->info2);
            } else {
                *menor_valor = &(menor_bloco->info1);
            }
        }
    } else if (nodo->info1.numero_inicial == info_dados->numero_inicial) {
        menor_bloco = arvore23_buscar_maior_filho(nodo->esquerdo, &pai_temporario, menor_valor);
    } else {
        menor_bloco = arvore23_buscar_maior_filho(nodo->centro, &pai_temporario, menor_valor);
    }

    return menor_bloco;
}

Arvore23 *bloco_maior(Arvore23 **estrutura, Arvore23 *nodo, Data *info_dados, Data **maior_valor) {
    Arvore23 *maior_bloco, *pai_temporario;
    *maior_valor = NULL;

    if (eh_folha(*nodo)) {
        if (nodo->n_infos == 2 && nodo->info1.numero_inicial == info_dados->numero_inicial) {
            maior_bloco = nodo;
        } else {
            maior_bloco = arvore23_buscar_maior_pai(*estrutura, info_dados->numero_inicial);
        }

        if (maior_bloco != NULL) {
            if (maior_bloco->info1.numero_inicial > info_dados->numero_inicial) {
                *maior_valor = &(maior_bloco->info1);
            } else {
                *maior_valor = &(maior_bloco->info2);
            }
        }
    } else {
        if (nodo->info1.numero_inicial == info_dados->numero_inicial) {
            maior_bloco = arvore23_buscar_menor_filho(nodo->centro, &pai_temporario);
        } else {
            maior_bloco = arvore23_buscar_menor_filho(nodo->direito, &pai_temporario);
        }

        if (maior_bloco != NULL) {
            *maior_valor = &(maior_bloco->info1);
        }
    }

    return maior_bloco;
}

void alterar_no_arvore(Arvore23 **estrutura, Arvore23 *nodo_atual, Data *info_atual, int tamanho) {
    Arvore23 *menor_bloco;
    Data *menor_dado = NULL;

    menor_bloco = bloco_menor(estrutura, nodo_atual, info_atual, &menor_dado);

    if (tamanho < (info_atual->numero_final - info_atual->numero_inicial + 1)) {
        if (menor_bloco == NULL) {
            Data novo_dado;
            novo_dado.numero_inicial = info_atual->numero_inicial;
            novo_dado.numero_final = info_atual->numero_inicial + tamanho - 1;
            novo_dado.status = !(info_atual->status);

            info_atual->numero_inicial += tamanho;
            arvore23_inserir(estrutura, novo_dado);
        } else {
            menor_dado->numero_final += tamanho;
            info_atual->numero_inicial += tamanho;
        }
    } else {
        Arvore23 *maior_bloco;
        Data *maior_dado = NULL;

        maior_bloco = bloco_maior(estrutura, nodo_atual, info_atual, &maior_dado);

        if (menor_bloco == NULL && maior_bloco == NULL) {
            info_atual->status = !(info_atual->status);
        } else {
            if (menor_bloco == NULL) {
                info_atual->status = !(info_atual->status);
                juntar_no(estrutura, &(info_atual->numero_final), maior_dado->numero_final, maior_dado->numero_inicial);
            } else if (maior_bloco == NULL) {
                juntar_no(estrutura, &(menor_dado->numero_final), info_atual->numero_final, info_atual->numero_inicial);
            } else {
                int remover_endereco = maior_dado->numero_inicial;
                juntar_no(estrutura, &(menor_dado->numero_final), maior_dado->numero_final, info_atual->numero_inicial);
                arvore23_remover(estrutura, remover_endereco);
            }
        }
    }
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

int gerenciar_bloco_memoria(Arvore23 **estrutura, int tamanho, int estado_atual) {
    Data *bloco_alvo = NULL;
    Arvore23 *nodo_alvo = procurar_no(estrutura, tamanho, estado_atual, &bloco_alvo);

    if (bloco_alvo != NULL) {
        printf("\nDetalhes do bloco selecionado:\n");
        no23_exibir(*bloco_alvo);
        alterar_no_arvore(estrutura, nodo_alvo, bloco_alvo, tamanho);
    } else {
        printf("\nNão foram encontrados blocos disponíveis com o critério especificado.\n");
    }

    return 1;
}


