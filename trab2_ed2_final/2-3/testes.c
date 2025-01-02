#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "portugues_arv.c"
#include "ingles_arv.c"
#include <windows.h> // Para QueryPerformanceCounter

typedef struct {
    char palavra[100];
    char caminho[500];
    int encontrada;
    long long tempo_ns; // Substituí LONGLONG por long long
} ResultadoBusca;

// Função para obter tempo em nanosegundos no Windows
long long getTempoNanosegundos() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1000000000) / frequency.QuadPart;
}

int inserirPalavraPortugues(Arv_pt **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    Info promove;
    Arv_pt *pai = NULL;
    int inseriu;

    Arv_pt *noExistente = BuscarNoPorPalavra(arvore, palavraPortugues);
    if (noExistente != NULL) {
        AdicionarTraducaoNo(noExistente, palavraPortugues, palavraIngles, unidade);
        inseriu = 1;
    } else {
        Info novoInfo = CriarInfo(palavraPortugues, palavraIngles, unidade);
        InserirInfoArvore23(arvore, &novoInfo, &promove, &pai);
        inseriu = 0;
    }
    return inseriu;
}

void carregarArquivo(const char *nomeArquivo, Arv_pt **arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        if (linha[0] == '%') {
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);
            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while (traducaoPortugues != NULL) {
                while (*traducaoPortugues == ' ') 
                    traducaoPortugues++;
                inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                traducaoPortugues = strtok(NULL, ",;");
            }
        }
    }

    fclose(arquivo);
}

int buscarPalavraComCaminho23(Arv_pt *arvore, char *palavraPortugues, char *caminho) {
    if (arvore == NULL) {
        strcat(caminho, "NULL");
        return 0;
    }

    if (strcmp(arvore->info1.palavraPortugues, palavraPortugues) == 0) {
        strcat(caminho, "Encontrado em Info1 -> ");
        return 1;
    }

    if (arvore->nInfos == 2 && strcmp(arvore->info2.palavraPortugues, palavraPortugues) == 0) {
        strcat(caminho, "Encontrado em Info2 -> ");
        return 1;
    }

    if (strcmp(palavraPortugues, arvore->info1.palavraPortugues) < 0) {
        strcat(caminho, "Esquerda -> ");
        return buscarPalavraComCaminho23(arvore->esq, palavraPortugues, caminho);
    } else if (arvore->nInfos == 1 || strcmp(palavraPortugues, arvore->info2.palavraPortugues) < 0) {
        strcat(caminho, "Centro -> ");
        return buscarPalavraComCaminho23(arvore->cent, palavraPortugues, caminho);
    } else {
        strcat(caminho, "Direita -> ");
        return buscarPalavraComCaminho23(arvore->dir, palavraPortugues, caminho);
    }
}

void realizarExperimento23(Arv_pt *arvore, char palavras[30][50]) {
    long long inicio, fim;
    double tempoTotal = 0;

    printf("\n--- Experimento ---\n");
    for (int i = 0; i < 30; i++) {
        char caminho[2048] = "";
        inicio = getTempoNanosegundos();

        int encontrado = buscarPalavraComCaminho23(arvore, palavras[i], caminho);

        fim = getTempoNanosegundos();
        double tempoGasto = (double)(fim - inicio);
        tempoTotal += tempoGasto;

        printf("Palavra: %s | Caminho: %s | Encontrada: %s | Tempo: %.0f nanosegundos\n",
               palavras[i], caminho, encontrado ? "Sim" : "Não", tempoGasto);
    }

    printf("\nTempo médio por busca: %.0f nanosegundos\n", tempoTotal / 30);
}

int main() {
    Arv_pt *arvore = NULL;

    carregarArquivo("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/Trabalho_segunda_provaVERSAO-REFERENCIA/Tabalho2_Ed2-main/trabalhoEd2.txt", &arvore);

    char palavras[30][50] = {
        "onibus", "barramento", "inseto", "bicicleta", "ventilador", 
        "casa", "livro", "escola", "tempo", "sol", 
        "lua", "estrela", "amigo", "carro", "felicidade", 
        "amizade", "trabalho", "lazer", "computador", "árvore",
        "flor", "jardim", "cachorro", "gato", "passarinho", 
        "peixe", "montanha", "rio", "praia", "mar"
    };

    realizarExperimento23(arvore, palavras);

    LiberarArvore23(arvore);

    return 0;
}
