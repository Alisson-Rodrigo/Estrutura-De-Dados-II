#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_EMPLOYEES 1000
#define TABLE_SIZE_SMALL 101
#define TABLE_SIZE_LARGE 150

typedef struct {
    char matricula[7];
    char nome[11];
    char funcao[20];
    float salario;
} Funcionario;

typedef struct {
    Funcionario *funcionario;
} TabelaHash;

// Função para rotacionar 2 dígitos para a esquerda
void rotacionar_esquerda_2_digitos(char *matricula) {
    char temp1 = matricula[0];
    char temp2 = matricula[1];
    int i;
    for (i = 0; i < 4; i++) {
        matricula[i] = matricula[i + 2];
    }
    matricula[4] = temp1;
    matricula[5] = temp2;
}

// Função de hash usando rotação à esquerda
int funcao_hash_rotacao(const char *matricula, int tamanho_tabela) {
    char rotacionada[7];
    strncpy(rotacionada, matricula, 6);
    rotacionada[6] = '\0';
    rotacionar_esquerda_2_digitos(rotacionada);
    int digito2 = rotacionada[1] - '0';
    int digito4 = rotacionada[3] - '0';
    int digito6 = rotacionada[5] - '0';
    int hash = (digito2 * 100 + digito4 * 10 + digito6) % tamanho_tabela;
    return hash;
}

// Função de hash usando fole shift
int funcao_hash_fole_shift(const char *matricula, int tamanho_tabela) {
    int grupo1 = (matricula[1] - '0') * 100 + (matricula[3] - '0') * 10 + (matricula[5] - '0');
    int grupo2 = (matricula[0] - '0') * 100 + (matricula[2] - '0') * 10 + (matricula[4] - '0');
    int hash = (grupo1 + grupo2) % tamanho_tabela;
    return hash;
}

// Inserir na tabela hash com tratamento de colisões
void inserir_tabela_hash(TabelaHash *tabela, int tamanho_tabela, Funcionario *funcionario, int usar_fole_shift, int *contagem_colisoes) {
    int hash = usar_fole_shift ? funcao_hash_fole_shift(funcionario->matricula, tamanho_tabela)
                               : funcao_hash_rotacao(funcionario->matricula, tamanho_tabela);
    int hash_original = hash;
    int primeiro_indice_visitado = -1;

    while (tabela[hash].funcionario != NULL) {
        (*contagem_colisoes)++;
        if (primeiro_indice_visitado == -1) {
            primeiro_indice_visitado = hash;
        }
        hash = (hash + (usar_fole_shift ? 7 : (funcionario->matricula[0] - '0'))) % tamanho_tabela;
        if (hash == hash_original) {
            tabela[primeiro_indice_visitado].funcionario = funcionario;
            return;
        }
    }
    tabela[hash].funcionario = funcionario;
}

// Gerar dados aleatórios para os funcionários
void gerar_dados_funcionarios(Funcionario *funcionarios, int num_funcionarios) {
    srand(time(NULL));
    int i, j;
    for (i = 0; i < num_funcionarios; i++) {
        for (j = 0; j < 6; j++) {
            funcionarios[i].matricula[j] = '0' + rand() % 10;
        }
        funcionarios[i].matricula[6] = '\0';
        for (j = 0; j < 10; j++) {
            funcionarios[i].nome[j] = 'A' + rand() % 26;
        }
        funcionarios[i].nome[10] = '\0';
        strcpy(funcionarios[i].funcao, "Funcionario");
        funcionarios[i].salario = (rand() % 12001) + 3000; // Salário entre 3000 e 15000
    }
}

// Imprimir a tabela hash
void imprimir_tabela_hash(TabelaHash *tabela, int tamanho_tabela) {
    int i;
    printf("+--------+------------+------------+-------------------+-------------+\n");
    printf("| Indice | Matricula  | Nome       | Funcao            | Salario     |\n");
    printf("+--------+------------+------------+-------------------+-------------+\n");
    for (i = 0; i < tamanho_tabela; i++) {
        if (tabela[i].funcionario != NULL) {
            printf("| %6d | %-10s | %-10s | %-17s | %11.2f |\n",
                   i, tabela[i].funcionario->matricula, tabela[i].funcionario->nome,
                   tabela[i].funcionario->funcao, tabela[i].funcionario->salario);
        } else {
            printf("| %6d | %-10s | %-10s | %-17s | %-11s |\n",
                   i, "Vazio", "Vazio", "Vazio", "Vazio");
        }
    }
    printf("+--------+------------+------------+-------------------+-------------+\n");
}

int main() {
    while (1) {
        int tamanho_tabela;
        printf("Escolha o tamanho da tabela:\n1. 101 posicoes\n2. 150 posicoes\nDigite sua escolha: ");
        int escolha_tamanho;
        scanf("%d", &escolha_tamanho);
        if (escolha_tamanho == 1) {
            tamanho_tabela = TABLE_SIZE_SMALL;
        } else {
            tamanho_tabela = TABLE_SIZE_LARGE;
        }

        TabelaHash *tabela_hash = calloc(tamanho_tabela, sizeof(TabelaHash));
        Funcionario funcionarios[NUM_EMPLOYEES];

        gerar_dados_funcionarios(funcionarios, NUM_EMPLOYEES);

        int escolha;
        printf("Escolha a funcao de hash:\n1. Rotacao com deslocamento para esquerda\n2. Fole shift\nDigite sua escolha: ");
        scanf("%d", &escolha);

        int usar_fole_shift = (escolha == 2);

        int contagem_colisoes = 0;
        LARGE_INTEGER inicio, fim, frequencia;
        QueryPerformanceFrequency(&frequencia);
        QueryPerformanceCounter(&inicio);
        int i;
        for (i = 0; i < NUM_EMPLOYEES; i++) {
            inserir_tabela_hash(tabela_hash, tamanho_tabela, &funcionarios[i], usar_fole_shift, &contagem_colisoes);
        }
        QueryPerformanceCounter(&fim);

        double tempo_decorrido = (double)(fim.QuadPart - inicio.QuadPart) * 1e9 / frequencia.QuadPart;

        printf("Hash finalizado.\n");
        printf("Colisoes: %d\n", contagem_colisoes);
        printf("Tempo para hash e resolver colisoes: %.0f nanosegundos\n", tempo_decorrido);

        imprimir_tabela_hash(tabela_hash, tamanho_tabela);

        free(tabela_hash);

        char continuar;
        printf("Deseja realizar outra operacao? (s/n): ");
        scanf(" %c", &continuar);
        if (continuar == 'n' || continuar == 'N') {
            break;
        }
    }

    return 0;
}
