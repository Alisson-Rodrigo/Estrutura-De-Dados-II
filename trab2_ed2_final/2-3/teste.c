#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.c"
#include "arvbin.c"
#include "unidade.c"

void carregarArquivo(const char *nomeArquivo, Portugues23 **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];      // Para armazenar cada linha do arquivo
    int unidadeAtual = 0; // Unidade atual lida do arquivo

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        if (linha[0] == '%') // Identifica linhas que definem a unidade
        {
            // Atualiza a unidade usando sscanf para capturar o número após "Unidade"
            if (sscanf(linha, "%% Unidade %d", &unidadeAtual) != 1)
            {
                printf("Erro ao interpretar a unidade na linha: %s\n", linha);
            }
        }
        else // Processa as linhas de palavras e traduções
        {
            char palavraIngles[50], traducoesPortugues[200];

            // Divide a linha em palavra em inglês e traduções em português
            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
                // Usa strtok para separar as traduções por vírgula
                char *traducaoPortugues = strtok(traducoesPortugues, ",");
                while (traducaoPortugues != NULL)
                {
                    // Remove espaços em branco extras no início da tradução
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    // Insere a palavra em português e a tradução em inglês na árvore
                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);

                    // Formata o print no formato "Palavra Português: Inglês"
                    printf("Palavra Português: %s - Inglês: %s\n", traducaoPortugues, palavraIngles);

                    // Continua para a próxima tradução
                    traducaoPortugues = strtok(NULL, ",");
                }
            }
            else
            {
                printf("Erro ao interpretar a linha: %s\n", linha);
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}