#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MAX_VERTICES 100
#define INF DBL_MAX

typedef struct {
    int v;
    double weight;
} Edge;

typedef struct {
    Edge edges[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
    int numEdges;
} Graph;

void initGraph(Graph *g, int numVertices) {
    g->numVertices = numVertices;
    g->numEdges = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            g->edges[i][j].weight = (i == j) ? 0.0 : INF;
        }
    }
}

void addEdge(Graph *g, int u, int v, double reliability) {
    if (reliability <= 0 || reliability > 1) {
        printf("Erro: Confiabilidade deve estar no intervalo (0, 1].\n");
        exit(1);
    }
    g->edges[u][v].v = v;
    g->edges[u][v].weight = -log(reliability);
    g->numEdges++;
}

void dijkstra(Graph *g, int start, int end, int *path, double *probability) {
    double dist[MAX_VERTICES];
    int prev[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    int n = g->numVertices;
    int continueLoop = 1; // Variável para controlar o loop principal

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0.0;

    int i = 0;
    while (i < n && continueLoop) { // Substitui o for com um controle adicional
        double minDist = INF;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) {
            continueLoop = 0; // Atualiza o controle para encerrar o loop
        } else {
            visited[u] = 1;

            for (int v = 0; v < n; v++) {
                if (!visited[v] && g->edges[u][v].weight < INF) {
                    double newDist = dist[u] + g->edges[u][v].weight;
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        prev[v] = u;
                    }
                }
            }
        }
        i++;
    }

    int current = end;
    int pathIndex = 0;
    while (current != -1) {
        path[pathIndex++] = current;
        current = prev[current];
    }

    *probability = exp(-dist[end]);

    for (int i = 0; i < pathIndex / 2; i++) {
        int temp = path[i];
        path[i] = path[pathIndex - 1 - i];
        path[pathIndex - 1 - i] = temp;
    }

    path[pathIndex] = -1; 
}


int main() {
    int repeat = 1;
    Graph g;
    while (repeat == 1) {
        int n, m;
        int erro = 0;

        printf("Digite o numero de vertices e arestas: ");
        if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m < 0 || n > MAX_VERTICES) {
            printf("Erro: Entrada invalida para vertices ou arestas.\n");
            erro = 1;
        }

        if (!erro) {
           
            initGraph(&g, n);

            printf("Digite as arestas no formato (u v r):\n");
            for (int i = 0; i < m; i++) {
                int u, v;
                double r;
                if (scanf("%d %d %lf", &u, &v, &r) != 3 || u < 0 || v < 0 || u >= n || v >= n) {
                    printf("Erro: Entrada invalida para arestas.\n");
                    erro = 1;
                    i = m; 
                } else {
                    addEdge(&g, u, v, r);
                }
            }
        }

        if (!erro) {
            int start, end;
            printf("Digite os vertices de origem e destino: ");
            if (scanf("%d %d", &start, &end) != 2 || start < 0 || end < 0 || start >= n || end >= n) {
                printf("Erro: Entrada invalida para origem ou destino.\n");
                erro = 1;
            }

            if (!erro) {
                int path[MAX_VERTICES] = {-1};
                double probability;
                dijkstra(&g, start, end, path, &probability);

                printf("Caminho mais confiavel: ");
                for (int i = 0; path[i] != -1; i++) {
                    printf("%d ", path[i]);
                }

                printf("\nProbabilidade de sucesso total: %.2lf (%.2lf%%)\n", probability, probability * 100);
            }
        }

        if (erro) {
            printf("Houve um erro. Deseja tentar novamente? (1 - Sim / 0 - Nao): ");
        } else {
            printf("Deseja calcular novamente? (1 - Sim / 0 - Nao): ");
        }
        if (scanf("%d", &repeat) != 1) {
            repeat = 0; 
        }
    }

    return 0;
}