#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANHO_TABELA1 101
#define TAMANHO_TABELA2 150
#define MAX_FUNCIONARIOS 1000

typedef struct {
    char Id[7];
    char userName[50];
    char functionName[20];
    int salary;
    bool isOccupied;
} Employee;

void rotate_left(char *matricula) {
    char temp = matricula[0];
    for (int i = 0; i < 5; i++) {
        matricula[i] = matricula[i + 1];
    }
    matricula[5] = temp;
}

int rotate_hash_function(char *matricula, int tamanho_tabela) {
    rotate_left(matricula);
    int d2 = matricula[1] - '0';
    int d4 = matricula[3] - '0';
    int d6 = matricula[5] - '0';
    return (d2 + d4 + d6) % tamanho_tabela;
}

int hash_fold_and_shift(char *matricula, int tamanho_tabela) {
    int grupo1 = (matricula[0] - '0') * 100 + (matricula[2] - '0') * 10 + (matricula[5] - '0');
    int grupo2 = (matricula[1] - '0') * 100 + (matricula[3] - '0') * 10 + (matricula[4] - '0');
    return (grupo1 + grupo2) % tamanho_tabela;
}

int handle_rotation_collision(int indice, int i, char *matricula, int tamanho_tabela) {
    int incremento = matricula[0] - '0'; // Primeiro dígito da matrícula
    return (indice + i * incremento) % tamanho_tabela;
}

int handle_fold_shift_collision(int indice, int i, int tamanho_tabela) {
    int incremento = 7; 
    return (indice + i * incremento) % tamanho_tabela;
}

bool insertEmployee(Employee *tabela, Employee funcionario, int tamanho_tabela, int (*hash_func)(char *, int), int *colisoes, int metodo) {
    int indice = hash_func(funcionario.Id, tamanho_tabela);
    bool inserido = false;
    bool metodo_valido = (metodo == 1 || metodo == 2);
    int i = 0;

    // Caso o método não seja válido, a inserção não será realizada
    if (!metodo_valido) {
        inserido = false;
    } else {
        while (i < tamanho_tabela && !inserido) {
            int posicao = 0;

            // Calcula a posição com base no método de tratamento de colisão
            if (metodo == 1) { 
                posicao = handle_rotation_collision(indice, i, funcionario.Id, tamanho_tabela);
            } else if (metodo == 2) { 
                posicao = handle_fold_shift_collision(indice, i, tamanho_tabela);
            }

            // Verifica se a posição está desocupada
            if (!tabela[posicao].isOccupied) {
                tabela[posicao] = funcionario;
                tabela[posicao].isOccupied = true;
                inserido = true;
            } else {
                (*colisoes)++; // Incrementa o contador de colisões
            }

            i++; // Incrementa o contador de tentativas
        }

        // Caso não tenha sido possível inserir, usa a posição 0 como fallback
        if (!inserido) {
            tabela[0] = funcionario;
            tabela[0].isOccupied = true;
        }
    }

    return inserido;
}




void initialize_employee_table(Employee *tabela, int tamanho_tabela) {
    for (int i = 0; i < tamanho_tabela; i++) {
        tabela[i].isOccupied = false;
    }
}


void generate_employee_data(Employee *dados, int total) {
    for (int i = 0; i < total; i++) {
        sprintf(dados[i].Id, "%06d", rand() % 1000000);
        sprintf(dados[i].userName, "Funcionario_%d", i);
        sprintf(dados[i].functionName, "Funcao_%d", i % 4);
        dados[i].salary = 3000 + rand() % 20000;
        dados[i].isOccupied = false;
    }
}


void print_hash_table(Employee *tabela, int tamanho_tabela) {
    printf("+--------+------------+------------+-------------------+-------------+\n");
    printf("| Indice | Matricula  | Nome       | Funcao            | Salario     |\n");
    printf("+--------+------------+------------+-------------------+-------------+\n");
    for (int i = 0; i < tamanho_tabela; i++) {
        if (tabela[i].isOccupied) {
            printf("| %6d | %-10s | %-10s | %-17s | %11d |\n",
                   i, tabela[i].Id, tabela[i].userName,
                   tabela[i].functionName, tabela[i].salary);
        } else {
            printf("| %6d | %-10s | %-10s | %-17s | %-11s |\n",
                   i, "Vazio", "Vazio", "Vazio", "Vazio");
        }
    }
    printf("+--------+------------+------------+-------------------+-------------+\n");
}


int main() {
   Employee tabela1[TAMANHO_TABELA1], tabela2[TAMANHO_TABELA2];
    Employee dados[MAX_FUNCIONARIOS];
    int continuar = 1;

    generate_employee_data(dados, MAX_FUNCIONARIOS);

    while (continuar) {
        int metodo, tamanho_tabela;
        int (*hash_func)(char *, int);

        printf("Escolha o método de hashing:\n");
        printf("1 - Rotacao e Extracao\n");
        printf("2 - Fold Shift\n");
        scanf("%d", &metodo);

        printf("Escolha o tamanho da tabela:\n");
        printf("1 - %d (Tabela 1)\n", TAMANHO_TABELA1);
        printf("2 - %d (Tabela 2)\n", TAMANHO_TABELA2);
        int opcao_tamanho;
        scanf("%d", &opcao_tamanho);

        if (opcao_tamanho == 1) {
            tamanho_tabela = TAMANHO_TABELA1;
            initialize_employee_table(tabela1, tamanho_tabela);
        } else {
            tamanho_tabela = TAMANHO_TABELA2;
            initialize_employee_table(tabela2, tamanho_tabela);
        }

        hash_func = (metodo == 1) ? rotate_hash_function : hash_fold_and_shift;

        int colisoes = 0;

        for (int i = 0; i < MAX_FUNCIONARIOS; i++) {
            if (opcao_tamanho == 1) {
                insertEmployee(tabela1, dados[i], tamanho_tabela, hash_func, &colisoes, metodo);
            } else {
                insertEmployee(tabela2, dados[i], tamanho_tabela, hash_func, &colisoes, metodo);
            }
        }

        printf("\nTabela escolhida:\n");
        if (opcao_tamanho == 1) {
            print_hash_table(tabela1, tamanho_tabela);
        } else {
            print_hash_table(tabela2, tamanho_tabela);
        }

        printf("\nTotal de colisoes: %d\n", colisoes);

        printf("\nDeseja continuar? (1 - Sim, 0 - Nao): ");
        scanf("%d", &continuar);
    }

    printf("Programa encerrado.\n");
    return 0;
}

