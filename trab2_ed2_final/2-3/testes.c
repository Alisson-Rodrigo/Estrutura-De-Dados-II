#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> // Para QueryPerformanceCounter e QueryPerformanceFrequency
#include "arv23.c"   // Implementação da Árvore 2-3
#include "unidade.c" // Funções auxiliares
#include "arvbin.c"   // Implementação da Árvore 2-3
// Função para limpar caracteres indesejados
void clearCharacters(char *str)
{
    char *end = str + strlen(str) - 1;

    while (end > str && (*end == ';' || isspace((unsigned char)*end)))
    {
        *end = '\0';
        end--;
    }

    while (*str && isspace((unsigned char)*str))
    {
        str++;
    }
}

// Função para carregar o arquivo de dados
void loadFile23(const char *fileName, PortugueseTree **tree)
{
    FILE *filePointer = fopen(fileName, "r");
    if (filePointer == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char inputLine[256];
    int currentUnit = -1;

    while (fgets(inputLine, sizeof(inputLine), filePointer))
    {
        inputLine[strcspn(inputLine, "\n")] = 0;

        if (inputLine[0] == '%')
        {
            sscanf(inputLine, "%% Unidade %d", &currentUnit);
        }
        else if (currentUnit != -1)
        {
            char englishWord[50], portugueseTranslations[200];
            if (sscanf(inputLine, "%[^:]: %[^\n]", englishWord, portugueseTranslations) == 2)
            {
                clearCharacters(englishWord);

                char *translation = strtok(portugueseTranslations, ",");
                while (translation)
                {
                    clearCharacters(translation);
                    insertPortugueseTerm(tree, translation, englishWord, currentUnit);
                    translation = strtok(NULL, ",");
                }
            }
        }
    }

    fclose(filePointer);
}

// Função para busca na Árvore 2-3
void searchWord23(PortugueseTree **tree, const char *word)
{
    PortugueseTree *current = *tree;
    int found = 0;
    char path[1024] = "";

    while (current != NULL && !found)
    {
        if (strcmp(word, current->info1.portugueseWord) == 0)
        {
            found = 1;
        }
        else if (current->nInfos == 2 && strcmp(word, current->info2.portugueseWord) == 0)
        {
            found = 1;
        }
        else
        {
            if (strcmp(word, current->info1.portugueseWord) < 0)
            {
                strcat(path, "Esquerda -> ");
                current = current->left;
            }
            else if (current->nInfos == 1 || strcmp(word, current->info2.portugueseWord) < 0)
            {
                strcat(path, "Centro -> ");
                current = current->cent;
            }
            else
            {
                strcat(path, "Direita -> ");
                current = current->right;
            }
        }
    }

    if (found)
    {
        if (strlen(path) > 0)
            path[strlen(path) - 4] = '\0';
        printf("Palavra encontrada.\nCaminho percorrido: %s\n", path);
    }
    else
    {
        strcat(path, "NULL");
        printf("Palavra nao encontrada.\nCaminho percorrido: %s\n", path);
    }
}

// Função principal para Árvore 2-3
int main()
{
    PortugueseTree *tree23 = NULL;
    const char *fileName = "C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/text.txt";
    loadFile23(fileName, &tree23);

    const char *words[] = { "carro", "automovel", "erro", "engano", "roda", "ventilador", "soprador",
        "teia", "conexao", "estrutura", "organizacao", "cadeado", "fruta",
        "memoria", "companheiro", "documento", "esfera", "fio", "dispositivo",
        "vidro", "escrivaninha", "cursor", "acordo", "estrela", "mar", "rio",
        "cachoeira", "tempo", "vento", "chuva"};
    int numWords = sizeof(words) / sizeof(words[0]);

    printf("Buscando palavras na Árvore 2-3:\n\n");

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    double totalTime = 0;
    for (int i = 0; i < numWords; i++)
    {
        QueryPerformanceCounter(&start);
        printf("Palavra: %s\n", words[i]);
        searchWord23(&tree23, words[i]);
        QueryPerformanceCounter(&end);

        double elapsedTime = (double)(end.QuadPart - start.QuadPart) * 1e9 / frequency.QuadPart;
        totalTime += elapsedTime;

        printf("Tempo gasto: %.2f ns\n\n", elapsedTime);
    }

    printf("Tempo total: %.2f ns\nTempo médio: %.2f ns\n", totalTime, totalTime / numWords);

    return 0;
}
