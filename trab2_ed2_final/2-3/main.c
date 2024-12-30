#include <stdio.h>
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

void menu()
{
    printf("\n------------------------------------------------------------------------------------------------- \n");
    printf("\nMenu de opções:\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em português e as equivalentes em inglês.\n");
    printf("2 - Informar uma palavra em português e imprimir todas as palavras em inglês equivalentes.\n");
    printf("3 - Informar uma palavra em inglês e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("4 - Informar uma palavra em português e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("5 - Imprimir a arvore completa\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

int main()
{
    Portugues23 *raiz = NULL;
    Portugues23 *pai = NULL;

    char palavra[50];
    int unidade;
    int removido;
    carregarArquivo("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/trabalhoEd2.txt", &raiz);
    int op, res;
    do
    {
        menu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja imprimir as palavras: ");
            scanf("%d", &unidade);

            // Variável de controle para a impressão do cabeçalho
            int unidadeImpressa = 0;

            // Chama a função para imprimir palavras da unidade
            imprimirPalavrasUnidade(raiz, unidade, &unidadeImpressa);

            printf("\n--------------------------------------------------------------- \n");
            break;

        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em português que deseja imprimir as palavras em inglês: ");
            scanf("%s", palavra);
            exibir_traducao_Portugues(&raiz, palavra); // Chama a função para exibir traduções da palavra
            printf("\n--------------------------------------------------------------- \n");
            break;

        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em ingles que deseja remover: ");
            scanf("%s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%d", &unidade);
            BuscarPalavraIngles(&raiz, palavra, unidade, &pai);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja remover: ");
            scanf("%s", palavra);
            removido = remover23(&pai, &raiz, palavra);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            exibir_tree23(raiz);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 0:
            printf("\n--------------------------------------------------------------- \n");
            printf("Saindo do programa...\n");
            printf("\n--------------------------------------------------------------- \n");
            break;
        default:
            printf("Insira um valor válido. \n");
            break;
        }
    } while (op != 0);

    freeTree(raiz);

    return 0;
}