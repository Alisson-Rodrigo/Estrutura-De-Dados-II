#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.h"
#include "unidade.h"

void carregarArquivo(const char *nomeArquivo, Inglesbin **arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        if (linha[0] == '%') {
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^"]", palavraIngles, traducoesPortugues);

            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while (traducaoPortugues != NULL) {
                while (*traducaoPortugues == ' ') {
                    traducaoPortugues++;
                }

                *arvore = inserir_unidade(*arvore, palavraIngles, unidadeAtual);
                traducaoPortugues = strtok(NULL, ",;");
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}

void imprimirArvore(Inglesbin *raiz) {
    if (raiz != NULL) {
        imprimirArvore(raiz->leftChild);
        printf("Palavra: %s\n", raiz->englishWord);

        Unidade *unidade = raiz->unidades;
        while (unidade != NULL) {
            printf("  Unidade: %d\n", unidade->unidade);
            unidade = unidade->prox;
        }

        imprimirArvore(raiz->rightChild);
    }
}

int main() {
    Inglesbin *arvore = NULL;

    // Nome do arquivo de entrada
    const char *nomeArquivo = "entradas.txt";

    // Carregar palavras do arquivo
    carregarArquivo(nomeArquivo, &arvore);

    // Imprimir a árvore
    printf("\nArvore Binaria de Busca:\n");
    imprimirArvore(arvore);

    // Liberar memória
    free_arvore_binaria(arvore);

    return 0;
}
