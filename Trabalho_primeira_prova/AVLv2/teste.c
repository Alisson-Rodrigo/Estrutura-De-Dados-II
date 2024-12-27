#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "notas.c"

// Função para medir o tempo no Windows
void iniciar_medicao(LARGE_INTEGER *inicio, LARGE_INTEGER *freq) {
    QueryPerformanceFrequency(freq);
    QueryPerformanceCounter(inicio);
}

long long finalizar_medicao(LARGE_INTEGER inicio, LARGE_INTEGER fim, LARGE_INTEGER freq) {
    QueryPerformanceCounter(&fim);
    return (fim.QuadPart - inicio.QuadPart) * 1000000000 / freq.QuadPart; // Converte para nanosegundos
}

// Função para desalocar a árvore de notas
void liberar_arvore(arvore_notas *raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz); // Libera o nó atual
}

// Caso para números ordenados
void caso_numeros_ordenados() {
    LARGE_INTEGER inicio, fim, freq;
    long long tempo_total_insercao = 0;
    long long tempo_total_busca = 0;
    int i, j;

    int num_execucoes = 30;
    int num_insercoes;

    // Pergunta ao usuário quantos números deseja inserir
    printf("Quantos números deseja inserir (ordenados)? ");
    scanf("%d", &num_insercoes);

    for (j = 0; j < num_execucoes; j++) {
        arvore_notas *raiz = NULL; // Reinicializa a árvore a cada iteração
        long long tempo_insercao_atual = 0;

        // Inserir elementos ordenados
        for (i = 0; i < num_insercoes; i++) {
            arvore_notas *nova_nota = criar_nota();
            nova_nota->codigo_disciplina = i; // Código em ordem crescente
            snprintf(nova_nota->semestre_cursado, sizeof(nova_nota->semestre_cursado), "2023-2"); // Exemplo de semestre
            nova_nota->nota_final = (float)(rand() % 101); // Nota aleatória de 0 a 100

            iniciar_medicao(&inicio, &freq);
            raiz = inserir_nota(raiz, nova_nota);
            tempo_insercao_atual += finalizar_medicao(inicio, fim, freq);
        }

        // Somar o tempo total de inserção ao total geral
        tempo_total_insercao += tempo_insercao_atual;

        // Buscar o mesmo elemento 30 vezes e medir o tempo
        for (i = 0; i < num_execucoes; i++) {
            int codigo_busca = rand() % num_insercoes; // Código a ser buscado

            iniciar_medicao(&inicio, &freq);
            arvore_notas *resultado = buscar_notas(raiz, codigo_busca);
            tempo_total_busca += finalizar_medicao(inicio, fim, freq);
        }

        // Limpar a árvore ao final de cada iteração
        liberar_arvore(raiz);
    }

    // Calcular a média de tempo de inserção e busca
    long long media_tempo_insercao = tempo_total_insercao / num_execucoes;
    long long media_tempo_busca = tempo_total_busca / num_execucoes;

    // Mostrar a média de inserção e busca
    printf("Números Ordenados:\n");
    printf("Tempo médio de inserção: %lld nanosegundos\n", media_tempo_insercao);
    printf("Tempo médio de busca: %lld nanosegundos\n\n", media_tempo_busca);
}

// Caso para números decrescentes
void caso_numeros_decrescentes() {
    LARGE_INTEGER inicio, fim, freq;
    long long tempo_total_insercao = 0;
    long long tempo_total_busca = 0;
    int i, j;


    int num_execucoes = 30;
    int num_insercoes;

    // Pergunta ao usuário quantos números deseja inserir
    printf("Quantos números deseja inserir (decrescentes)? ");
    scanf("%d", &num_insercoes);

    for (j = 0; j < num_execucoes; j++) {
        arvore_notas *raiz = NULL; // Reinicializa a árvore a cada iteração
        long long tempo_insercao_atual = 0;

        // Inserir elementos em ordem decrescente
        for (i = num_insercoes - 1; i >= 0; i--) {
            arvore_notas *nova_nota = criar_nota();
            nova_nota->codigo_disciplina = i; // Código em ordem decrescente
            snprintf(nova_nota->semestre_cursado, sizeof(nova_nota->semestre_cursado), "2023-2"); // Exemplo de semestre
            nova_nota->nota_final = (float)(rand() % 101); // Nota aleatória de 0 a 100

            iniciar_medicao(&inicio, &freq);
            raiz = inserir_nota(raiz, nova_nota);
            tempo_insercao_atual += finalizar_medicao(inicio, fim, freq);
        }

        // Somar o tempo total de inserção ao total geral
        tempo_total_insercao += tempo_insercao_atual;

        // Buscar o mesmo elemento 30 vezes e medir o tempo
        for (i = 0; i < num_execucoes; i++) {
            int codigo_busca = rand() % num_insercoes; // Código a ser buscado

            iniciar_medicao(&inicio, &freq);
            arvore_notas *resultado = buscar_notas(raiz, codigo_busca);
            tempo_total_busca += finalizar_medicao(inicio, fim, freq);
        }

        // Limpar a árvore ao final de cada iteração
        liberar_arvore(raiz);
    }

    // Calcular a média de tempo de inserção e busca
    long long media_tempo_insercao = tempo_total_insercao / num_execucoes;
    long long media_tempo_busca = tempo_total_busca / num_execucoes;

    // Mostrar a média de inserção e busca
    printf("Números Decrescentes:\n");
    printf("Tempo médio de inserção: %lld nanosegundos\n", media_tempo_insercao);
    printf("Tempo médio de busca: %lld nanosegundos\n\n", media_tempo_busca);
}

// Caso para números aleatórios
void caso_numeros_aleatorios() {
    LARGE_INTEGER inicio, fim, freq;
    long long tempo_total_insercao = 0;
    long long tempo_total_busca = 0;
    int i, j;

    int num_execucoes = 30;
    int num_insercoes;

    // Pergunta ao usuário quantos números deseja inserir
    printf("Quantos números deseja inserir (aleatórios)? ");
    scanf("%d", &num_insercoes);

    for (j = 0; j < num_execucoes; j++) {
        arvore_notas *raiz = NULL; // Reinicializa a árvore a cada iteração
        long long tempo_insercao_atual = 0;

        // Inserir elementos com códigos de disciplina aleatórios
        for (i = 0; i < num_insercoes; i++) {
            arvore_notas *nova_nota = criar_nota();
            nova_nota->codigo_disciplina = rand() % num_insercoes; // Código aleatório
            snprintf(nova_nota->semestre_cursado, sizeof(nova_nota->semestre_cursado), "2023-2"); // Exemplo de semestre
            nova_nota->nota_final = (float)(rand() % 101); // Nota aleatória de 0 a 100

            iniciar_medicao(&inicio, &freq);
            raiz = inserir_nota(raiz, nova_nota);
            tempo_insercao_atual += finalizar_medicao(inicio, fim, freq);
        }

        // Somar o tempo total de inserção ao total geral
        tempo_total_insercao += tempo_insercao_atual;

        // Buscar o mesmo elemento 30 vezes e medir o tempo
        for (i = 0; i < num_execucoes; i++) {
            int codigo_busca = rand() % num_insercoes; // Código a ser buscado

            iniciar_medicao(&inicio, &freq);
            arvore_notas *resultado = buscar_notas(raiz, codigo_busca);
            tempo_total_busca += finalizar_medicao(inicio, fim, freq);
        }

        // Limpar a árvore ao final de cada iteração
        liberar_arvore(raiz);
    }

    // Calcular a média de tempo de inserção e busca
    long long media_tempo_insercao = tempo_total_insercao / num_execucoes;
    long long media_tempo_busca = tempo_total_busca / num_execucoes;

    // Mostrar a média de inserção e busca
    printf("Números Aleatórios:\n");
    printf("Tempo médio de inserção: %lld nanosegundos\n", media_tempo_insercao);
    printf("Tempo médio de busca: %lld nanosegundos\n\n", media_tempo_busca);
}

int main() {
    int opcao;

    do {
        // Exibir o menu
        printf("Selecionar caso de teste:\n");
        printf("1 - Números Ordenados\n");
        printf("2 - Números Decrescentes\n");
        printf("3 - Números Aleatórios\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        // Executar a opção escolhida
        switch (opcao) {
            case 1:
                caso_numeros_ordenados();
                break;
            case 2:
                caso_numeros_decrescentes();
                break;
            case 3:
                caso_numeros_aleatorios();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

