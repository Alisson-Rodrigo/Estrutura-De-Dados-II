#include <time.h>
#include <stdio.h>
#include "arv23.c"
#include "arvbin.c"
#include <windows.h>
#include <stdio.h>
#include <string.h>

int inserirPalavraPortugues(Arv_portugues **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    int inseriu = 0;

    // Busca a palavra na árvore
    Arv_portugues *noExistente = NULL;
    noExistente =  SearchWordInTree(arvore, palavraPortugues);

    if (noExistente != NULL) {
        printf("A palavra já existe. Adicionando tradução...\n");
        AdicionarTraducaoEN(noExistente, palavraIngles, unidade);
        inseriu = 1;
    } else {
        Info novoInfo = CriarInfo(palavraPortugues, palavraIngles, unidade);
        inserirArvRec(arvore, &novoInfo);
        inseriu = 1;
    }
    return inseriu;
}
void carregarArquivo(const char *nomeArquivo, Arv_portugues **arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;  // Remover o \n no final da linha

        if (linha[0] == '%') {
            // Atualiza a unidade corretamente
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);

            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while (traducaoPortugues != NULL) {
                while (*traducaoPortugues == ' ') 
                    traducaoPortugues++;  // Ignorar espaços extras

                if (strlen(traducaoPortugues) > 0) {
                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                }

                traducaoPortugues = strtok(NULL, ",;");    
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}

// Função para buscar uma palavra em português e registrar o caminho percorrido
int buscarPalavraComCaminho(Arv_portugues *arvore, char *palavraPortugues, char *caminho) {
    if (arvore == NULL) {
        strcat(caminho, "NULL");
        return 0;
    }

    if (strcmp(arvore->info.palavraPortugues, palavraPortugues) == 0) {
        return 1; // Palavra encontrada
    }

    if (strcmp(palavraPortugues, arvore->info.palavraPortugues) < 0) {
        strcat(caminho, "Esquerda -> ");
        return buscarPalavraComCaminho(arvore->esq, palavraPortugues, caminho);
    } else {
        strcat(caminho, "Direita -> ");
        return buscarPalavraComCaminho(arvore->dir, palavraPortugues, caminho);
    }
}

// Função para medir o tempo em nanosegundos no Windows
LONGLONG getTempoNanosegundos() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1000000000) / frequency.QuadPart;
}

// Função para realizar o experimento
void realizarExperimento(Arv_portugues *arvore, char palavras[30][50]) {
    LONGLONG inicio, fim;
    double tempoTotal = 0;

    printf("\n--- Experimento ---\n");
    for (int i = 0; i < 30; i++) {
        char caminho[2048] = "";
        inicio = getTempoNanosegundos();

        int encontrado = buscarPalavraComCaminho(arvore, palavras[i], caminho);

        fim = getTempoNanosegundos();
        double tempoGasto = (double)(fim - inicio);
        tempoTotal += tempoGasto;

        printf("Palavra: %s | Caminho: %s | Encontrada: %s | Tempo: %.0f nanosegundos\n",
               palavras[i], caminho, encontrado ? "Sim" : "Não", tempoGasto);
    }

    printf("\nTempo médio por busca: %.0f nanosegundos\n", tempoTotal / 30);
}int main() {
    Arv_portugues *arvore = NULL;

    // Carregar o arquivo
    carregarArquivo("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/Trabalho_segunda_provaVERSAO-REFERENCIA/Tabalho2_Ed2-main/trabalhoEd2.txt", &arvore);

    // Palavras para o experimento
     char palavras[30][50] = {
        "onibus", "barramento", "inseto", "bicicleta", "ventilador", 
        "casa", "livro", "escola", "tempo", "sol", 
        "lua", "estrela", "amigo", "carro", "felicidade", 
        "amizade", "trabalho", "lazer", "computador", "árvore",
        "flor", "jardim", "cachorro", "gato", "passarinho", 
        "peixe", "montanha", "rio", "praia", "mar"
    };

    // Realizar o experimento
    realizarExperimento(arvore, palavras);

    return 0;
}


