#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.c"
#include "arvrb.c"
#include "unidade.c"
#include <ctype.h>
#include <time.h>

// Funcao para limpar caracteres indesejados
void clearCharacters(char *str)
{
    char *end;

    // Remove espacos e caracteres indesejados do final
    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == ';' || *end == ','))
    {
        *end = '\0';
        end--;
    }

    // Remove espacos do inicio
    char *start = str;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    // Copia a string limpa para o inicio
    memmove(str, start, strlen(start) + 1);
}

void loadFile(const char *fileName, RedBlackTreePT **treeRef)
{
    FILE *filePointer = fopen(fileName, "r");
    if (filePointer == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char inputLine[256];
    int currentUnit = 0;

    while (fgets(inputLine, sizeof(inputLine), filePointer))
    {
        // Remove o caractere de nova linha
        inputLine[strcspn(inputLine, "\n")] = 0;

        if (inputLine[0] == '%')
        {
            // Atualiza a unidade atual
            sscanf(inputLine, "%% Unidade %d", &currentUnit);
        }
        else
        {
            char englishWord[50], portugueseTranslations[200];

            // Separa a palavra em ingles e suas traducoes em portugues
            if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
            {
                // Limpa a palavra em ingles
                clearCharacters(englishWord);

                // Divide as traducoes em portugues
                char *portugueseTranslationToken = strtok(portugueseTranslations, ",;");
                while (portugueseTranslationToken != NULL)
                {
                    // Limpa cada traducao em portugues
                    clearCharacters(portugueseTranslationToken);

                    // Insere a palavra na arvore
                    insertPortugueseWord(treeRef, portugueseTranslationToken, englishWord, currentUnit);

                    // Proxima traducao
                    portugueseTranslationToken = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(filePointer);
}

void printPathAndFindWord(RedBlackTreePT **tree, const char *word) {
    RedBlackTreePT *currentNode = *tree;
    int found = 0;
    char path[1024] = ""; // Armazena o caminho percorrido

    while (currentNode != NULL && !found) {
        if (strcmp(word, currentNode->info.portugueseWord) == 0) {
            found = 1;
        } else if (strcmp(word, currentNode->info.portugueseWord) < 0) {
            strcat(path, "Esquerda -> ");
            currentNode = currentNode->left;
        } else {
            strcat(path, "Direita -> ");
            currentNode = currentNode->right;
        }
    }

    if (found) {
        // Remove o último " -> "
        if (strlen(path) > 0) path[strlen(path) - 4] = '\0';
        printf("Palavra encontrada: %s\n", word);
        printf("Caminho percorrido: %s\n", path);
    } else {
        printf("Palavra não encontrada: %s\n", word);
    }
}
void printPathOnly(RedBlackTreePT **tree, const char *word) {
    RedBlackTreePT *currentNode = *tree;
    char path[1024] = ""; // String para armazenar o caminho
    int found = 0;

    while (currentNode != NULL && !found) {
        if (strcmp(word, currentNode->info.portugueseWord) == 0) {
            found = 1;
        } else if (strcmp(word, currentNode->info.portugueseWord) < 0) {
            strcat(path, "Esquerda -> ");
            currentNode = currentNode->left;
        } else {
            strcat(path, "Direita -> ");
            currentNode = currentNode->right;
        }
    }

    // Exibe o caminho percorrido
    if (found) {
        if (strlen(path) > 0) path[strlen(path) - 4] = '\0'; // Remove o último " -> "
        printf("Caminho percorrido: %s\n", path);
    } else {
        printf("Caminho percorrido: Palavra nao encontrada\n");
    }
}


int main() {
    RedBlackTreePT *arvore = NULL;

    // Carrega dados do arquivo
    const char *nomeArquivo = "C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/text.txt"; // Nome do arquivo a ser carregado
    loadFile(nomeArquivo, &arvore);

    // Lista de 30 palavras para busca
     const char *palavras[] = {
         "estrela",
        "mar", "rio", "cachoeira", "tempo", "vento", "chuva", "amor", "esperanca", "alegria", "tristeza",
        "familia", "coracao", "alma", "mente", "cidade", "campo", "montanha", "cavalo", "passaro", "peixe",
        "onibus", "barramento", "problema", "bicicleta", "ventilador", "rede", "sistema", 
        "rede de computadores", "rede de relacionamento"
    };

    int numPalavras = sizeof(palavras) / sizeof(palavras[0]);

    printf("### Realizando Experimento com %d palavras ###\n\n", numPalavras);

    clock_t startTime, endTime;
    double totalTime = 0;

    // Buscar cada palavra e medir o tempo
    for (int i = 0; i < numPalavras; i++) {
        printf("Buscando palavra: %s\n", palavras[i]);
        startTime = clock();
        printPathAndFindWord(&arvore, palavras[i]);
        endTime = clock();

        double elapsedTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        totalTime += elapsedTime;

        printf("Tempo gasto: %.6f segundos\n\n", elapsedTime);
    }

    printf("### Resultados do Experimento ###\n");
    printf("Tempo total para buscar %d palavras: %.6f segundos\n", numPalavras, totalTime);
    printf("Tempo médio por palavra: %.6f segundos\n", totalTime / numPalavras);

    // Liberar memória da árvore
    free(arvore);

    return 0;
}
