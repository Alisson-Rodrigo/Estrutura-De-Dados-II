#include <stdio.h>
#include <stdlib.h>
#include "23.c"

// Função para leitura de números inteiros com validação
void leia_numero(const char *texto, int *variavel, int minimo, int maximo) {
    do {
        printf("%s", texto);
        scanf("%d", variavel);
        while (getchar() != '\n'); // Limpa o buffer
        if (*variavel < minimo || *variavel >= maximo) {
            printf("\nDigite um número entre %d e %d\n", minimo, maximo - 1);
        }
    } while (*variavel < minimo || *variavel >= maximo);
}

// Cadastro inicial dos nós intercalados
int cadastrar_nos(Arvore23 **arvore, int maximo) {
    int status;
    leia_numero("O primeiro nó está [1 - Livre, 0 - Ocupado]: ", &status, 0, 2);

    Data no;
    leia_numero("Endereço inicial: ", &no.numero_inicial, 0, maximo - 1);
    int minimo = no.numero_inicial;

    while (no.numero_inicial < maximo) {
        no.status = status;
        leia_numero("Endereço final: ", &no.numero_final, no.numero_inicial, maximo);
        arvore23_inserir(arvore, no);
        no.numero_inicial = no.numero_final + 1;
        status = !status; // Alterna entre LIVRE e OCUPADO
    }
    return minimo;
}

// Busca por nós disponíveis
Arvore23 *buscar_no_memoria(Arvore23 **arvore, int quant, int status, Data **info_escolhido) {
    if (*arvore == NULL) return NULL;

    // Verifica o nó atual
    if ((*arvore)->info1.status == status && ((*arvore)->info1.numero_final - (*arvore)->info1.numero_inicial + 1) >= quant) {
        *info_escolhido = &(*arvore)->info1;
        return *arvore;
    }
    if ((*arvore)->n_infos == 2 && (*arvore)->info2.status == status &&
        ((*arvore)->info2.numero_final - (*arvore)->info2.numero_inicial + 1) >= quant) {
        *info_escolhido = &(*arvore)->info2;
        return *arvore;
    }

    // Busca nos filhos
    Arvore23 *no = buscar_no_memoria(&(*arvore)->esquerdo, quant, status, info_escolhido);
    if (*info_escolhido == NULL) {
        no = buscar_no_memoria(&(*arvore)->centro, quant, status, info_escolhido);
    }
    if ((*arvore)->n_infos == 2 && *info_escolhido == NULL) {
        no = buscar_no_memoria(&(*arvore)->direito, quant, status, info_escolhido);
    }

    return no;
}

// Alocação e liberação de nós
int alocar_desalocar_no(Arvore23 **arvore, int quant_nos, int status) {
    Data *info_escolhido = NULL;
    Arvore23 *no_escolhido = buscar_no_memoria(arvore, quant_nos, status, &info_escolhido);

    if (info_escolhido) {
        printf("\nNó escolhido:\n");
        no23_exibir(*info_escolhido);

        // Atualiza o nó diretamente
        if (quant_nos < (info_escolhido->numero_final - info_escolhido->numero_inicial + 1)) {
            Data novo_no = {
                .numero_inicial = info_escolhido->numero_inicial,
                .numero_final = info_escolhido->numero_inicial + quant_nos - 1,
                .status = !info_escolhido->status
            };
            info_escolhido->numero_inicial += quant_nos; // Atualiza o bloco original
            arvore23_inserir(arvore, novo_no);          // Insere o novo bloco
        } else {
            info_escolhido->status = !info_escolhido->status;
        }
        return 1;
    }

    printf("\nNão há nó disponível\n");
    return 0;
}

// Menu principal
int menu() {
    int op;
    printf("\nMENU");
    printf("\n[1] - Alocar Nós");
    printf("\n[2] - Liberar Nós");
    printf("\n[3] - Exibir Nós (Pré-Ordem)");
    printf("\n[4] - Exibir Nós (Em-Ordem)");
    printf("\n[5] - Exibir Nós (Pós-Ordem)");
    printf("\n[0] - Sair\n");
    leia_numero("Opção escolhida: ", &op, 0, 6);
    return op;
}

// Função principal
int main() {
    Arvore23 *arvore = arvore23_criar();
    int maximo, minimo;

    leia_numero("Quantidade máxima de Blocos de memória: ", &maximo, 1, maximo + 1);
    minimo = cadastrar_nos(&arvore, maximo);

    int op, quant_nos;
    do {
        op = menu();
        switch (op) {
            case 1:
                leia_numero("Quantidade de nós a serem alocados: ", &quant_nos, minimo, maximo + 1);
                alocar_desalocar_no(&arvore, quant_nos, LIVRE);
                break;
            case 2:
                leia_numero("Quantidade de nós a serem liberados: ", &quant_nos, minimo, maximo + 1);
                alocar_desalocar_no(&arvore, quant_nos, OCUPADO);
                break;
            case 3:
                printf("\nExibindo árvore [Pré-Ordem]\n");
                arvore23_exibir_pre(arvore);
                break;
            case 4:
                printf("\nExibindo árvore [Em-Ordem]\n");
                arvore23_exibir_ordem(arvore);
                break;
            case 5:
                printf("\nExibindo árvore [Pós-Ordem]\n");
                arvore23_exibir_pos(arvore);
                break;
            case 0:
                printf("\nFinalizando programa...\n");
                break;
            default:
                printf("\nOpção inválida\n");
        }
    } while (op != 0);

    arvore23_desalocar(&arvore);
    return 0;
}
