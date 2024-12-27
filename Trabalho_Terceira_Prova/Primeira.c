#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

// Função para medir o tempo com alta precisão
#ifdef _WIN32
double getTime() {
    LARGE_INTEGER frequency, start;
    QueryPerformanceFrequency(&frequency); // Frequência do contador
    QueryPerformanceCounter(&start);       // Contagem atual
    return (double)start.QuadPart / frequency.QuadPart * 1e9; // Em nanosegundos
}
#else
#include <sys/time.h>
double getTime() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double)time.tv_sec * 1e9 + time.tv_nsec; // Em nanosegundos
}
#endif

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

    for (i = 0; i < 4; i++) {
        if (a.disks[i] != b.disks[i]) {
            diffCount++;
            if (diffCount > 1) return 0;

            from = a.disks[i];
            to = b.disks[i];
            smallestDisk = i;
        }
    }

    if (diffCount != 1) return 0;

    for (i = 0; i < smallestDisk; i++) {
        if (a.disks[i] == from || b.disks[i] == to) return 0;
    }

    return 1;
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

// Implementação do Algoritmo de Dijkstra com caminho
void dijkstra(int graph[MAX_CONFIGURATIONS][MAX_CONFIGURATIONS], int start, int end, Configuration *configs) {
    int dist[MAX_CONFIGURATIONS], prev[MAX_CONFIGURATIONS], visited[MAX_CONFIGURATIONS] = {0};
    int i, j, v, at;

    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (i = 0; i < MAX_CONFIGURATIONS; i++) {
        int minDist = INF, u = -1;
        for (j = 0; j < MAX_CONFIGURATIONS; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (v = 0; v < MAX_CONFIGURATIONS; v++) {
            if (graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
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
        printf("Configuração %d: [", path[i]);
        for (j = 0; j < 4; j++) {
            printf("%d", configs[path[i]].disks[j] + 1); // Converte pino (0,1,2) para (1,2,3)
            if (j < 3) printf(", ");
        }
        printf("]\n");
    }
}

int main() {
    Configuration configs[MAX_CONFIGURATIONS];
    int graph[MAX_CONFIGURATIONS][MAX_CONFIGURATIONS];
    int choice, start, end;
    double startTime, endTime;

    generateConfigurations(configs);
    buildGraph(graph, configs);

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Exibir todas as configurações\n");
        printf("2. Calcular o menor caminho entre duas configurações\n");
        printf("3. Sair\n");
        printf("==========================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayConfigurations(configs);
                break;
            case 2:
                printf("Digite a configuração inicial (0 a 80): ");
                scanf("%d", &start);
                printf("Digite a configuração final (0 a 80): ");
                scanf("%d", &end);

                startTime = getTime();
                dijkstra(graph, start, end, configs);
                endTime = getTime();

                printf("Tempo gasto: %.2lf nanosegundos\n", endTime - startTime);
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (choice != 3);

    return 0;
}
