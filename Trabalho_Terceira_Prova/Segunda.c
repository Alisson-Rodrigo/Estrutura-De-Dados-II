#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#define MAX_CONFIGURATIONS 81
#define INF 1000000

// Representação de uma configuração
typedef struct {
    int disks[4]; // 4 discos, cada posição indica o pino (0, 1 ou 2)
} Configuration;

// Função para gerar todas as configurações possíveis
void generateConfigurations(Configuration *configs) {
    int i, j;
    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        for (j = 0; j < 4; j++) {
            configs[i].disks[j] = (i / (int)pow(3, j)) % 3; // Calcula a base 3
        }
    }
}

// Exibir todas as configurações
void displayConfigurations(Configuration *configs) {
    int i, j;
    printf("\nTodas as configurações:\n");
    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        printf("No %d: ", i);
        for (j = 0; j < 4; j++) {
            printf("%d ", configs[i].disks[j] + 1); // Converte pino (0,1,2) para (1,2,3)
        }
        printf("\n");
    }
}

// Verifica se um movimento entre duas configurações é válido
int isValidMove(Configuration a, Configuration b) {
    int diffCount = 0;
    int from = -1, to = -1, smallestDisk = -1;
    int i;
    int isValid = 1; // Inicialmente válido

    for (i = 0; i < 4 && isValid; i++) {
        if (a.disks[i] != b.disks[i]) {
            diffCount++;
            if (diffCount > 1) {
                isValid = 0; // Mais de uma diferença detectada, inválido
            } else {
                from = a.disks[i];
                to = b.disks[i];
                smallestDisk = i;
            }
        }
    }

    if (diffCount != 1) {
        isValid = 0; // Deve haver exatamente uma diferença
    }

    for (i = 0; i < smallestDisk && isValid; i++) {
        if (a.disks[i] == from || b.disks[i] == to) {
            isValid = 0; // Condição inválida
        }
    }

    return isValid;
}


// Constrói a matriz de adjacência do grafo
void buildGraph(int graph[MAX_CONFIGURATIONS][MAX_CONFIGURATIONS], Configuration *configs) {
    int i, j;
    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        for (j = 0; j < MAX_CONFIGURATIONS; j++) {
            graph[i][j] = isValidMove(configs[i], configs[j]) ? 1 : INF;
        }
    }
}


void bellmanFord(int graph[MAX_CONFIGURATIONS][MAX_CONFIGURATIONS], int start, int end, Configuration *configs) {
    int dist[MAX_CONFIGURATIONS], prev[MAX_CONFIGURATIONS];
    int i, j, u, v;
    int at;

    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (i = 0; i < MAX_CONFIGURATIONS - 1; i++) {
        for (u = 0; u < MAX_CONFIGURATIONS; u++) {
            for (v = 0; v < MAX_CONFIGURATIONS; v++) {
                if (graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    prev[v] = u;
                }
            }
        }
    }

    printf("Menor caminho do início ao final: %d movimentos\n", dist[end]);
    printf("Caminho:\n");

    int path[MAX_CONFIGURATIONS], pathIndex = 0;
    for (at = end; at != -1; at = prev[at]) {
        path[pathIndex++] = at;
    }

    for (i = pathIndex - 1; i >= 0; i--) {
        printf("Configuracao %d: [", path[i]);
        for (j = 0; j < 4; j++) {
            printf("%d", configs[path[i]].disks[j] + 1); 
            if (j < 3) printf(", ");
        }
        printf("]\n");
    }
}

#ifdef _WIN32
long long getNanoseconds() {
    LARGE_INTEGER frequency, start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    return (start.QuadPart * 1000000000LL) / frequency.QuadPart;
}
#endif

int main() {
    Configuration configs[MAX_CONFIGURATIONS];
    int graph[MAX_CONFIGURATIONS][MAX_CONFIGURATIONS];
    int choice, start, end;

#ifdef _WIN32
    long long startTime, endTime;
#else
    struct timespec startTime, endTime;
#endif

    generateConfigurations(configs);
    buildGraph(graph, configs);

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Exibir todas as configuracoes\n");
        printf("2. Calcular o menor caminho entre duas configuracoes\n");
        printf("3. Sair\n");
        printf("==========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayConfigurations(configs);
                break;
            case 2:
                printf("Digite a configuracao inicial (0 a 80): ");
                scanf("%d", &start);
                printf("Digite a configuracao final (0 a 80): ");
                scanf("%d", &end);

#ifdef _WIN32
                startTime = getNanoseconds();
                bellmanFord(graph, start, end, configs);
                endTime = getNanoseconds();
                printf("Tempo gasto: %lld nanosegundos\n", endTime - startTime);
#else
                clock_gettime(CLOCK_MONOTONIC, &startTime);
                bellmanFord(graph, start, end, configs);
                clock_gettime(CLOCK_MONOTONIC, &endTime);

                long seconds = endTime.tv_sec - startTime.tv_sec;
                long nanoseconds = endTime.tv_nsec - startTime.tv_nsec;
                if (nanoseconds < 0) {
                    seconds -= 1;
                    nanoseconds += 1000000000;
                }

                printf("Tempo gasto: %ld segundos e %ld nanosegundos\n", seconds, nanoseconds);
#endif
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (choice != 3);

    return 0;
}
