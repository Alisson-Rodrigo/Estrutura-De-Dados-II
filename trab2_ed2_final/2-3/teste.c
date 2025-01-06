#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.c"
#include "arvbin.c"
#include "unidade.c"
#include <ctype.h>

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

void loadFile(const char *nomeArquivo, PortugueseTree **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    int unidadeAtual = -1; // Inicia com um valor inválido

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        if (linha[0] == '%')
        {
            // Verifica a linha de unidade e extrai o número
            if (sscanf(linha, "%% Unidade %d", &unidadeAtual) == 1)
            {
                printf("Carregando Unidade: %d\n", unidadeAtual);
            }
            else
            {
                printf("Erro ao interpretar a unidade na linha: %s\n", linha);
                unidadeAtual = -1; // Reseta para inválido
            }
        }
        else if (unidadeAtual != -1)
        {
            // Processa linhas de palavras e traduções
            char palavraIngles[50], traducoesPortugues[200];

            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
                clearCharacters(palavraIngles); // Limpa caracteres indesejados

                // Divide as traduções em português separadas por vírgula
                char *traducaoPortugues = strtok(traducoesPortugues, ",");
                while (traducaoPortugues != NULL)
                {
                    while (*traducaoPortugues == ' ')
                    {
                        traducaoPortugues++; // Remove espaços no início
                    }

                    clearCharacters(traducaoPortugues); // Limpa caracteres indesejados
                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);

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
}

void imprimirUnidadesExistentes(PortugueseTree *raiz, int *unidades, int *numUnidades)
{
    if (raiz)
    {
        // Verifica as unidades da primeira informação
        Inglesbin *ingles = raiz->info1.englishWord;
        while (ingles)
        {
            Unidade *unidadeAtual = ingles->unidades;
            while (unidadeAtual)
            {
                int existe = 0;
                for (int i = 0; i < *numUnidades; i++)
                {
                    if (unidades[i] == unidadeAtual->unidade)
                    {
                        existe = 1;
                        break;
                    }
                }
                if (!existe)
                {
                    unidades[*numUnidades] = unidadeAtual->unidade;
                    (*numUnidades)++;
                }
                unidadeAtual = unidadeAtual->prox;
            }
            ingles = ingles->esq ? ingles->esq : ingles->dir;
        }

        // Percorre a subárvore central
        imprimirUnidadesExistentes(raiz->cent, unidades, numUnidades);

        // Verifica as unidades da segunda informação
        if (raiz->nInfos == 2)
        {
            ingles = raiz->info2.englishWord;
            while (ingles)
            {
                Unidade *unidadeAtual = ingles->unidades;
                while (unidadeAtual)
                {
                    int existe = 0;
                    for (int i = 0; i < *numUnidades; i++)
                    {
                        if (unidades[i] == unidadeAtual->unidade)
                        {
                            existe = 1;
                            break;
                        }
                    }
                    if (!existe)
                    {
                        unidades[*numUnidades] = unidadeAtual->unidade;
                        (*numUnidades)++;
                    }
                    unidadeAtual = unidadeAtual->prox;
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }

        // Percorre a subárvore direita
        if (raiz->nInfos == 2)
        {
            imprimirUnidadesExistentes(raiz->right, unidades, numUnidades);
        }
    }
}
void imprimirPalavrasPortuguesPorUnidade(PortugueseTree *arvore, int unidade, int *unidadeImpressa)
{
    if (arvore)
    {
        // Percorre a subárvore esquerda
        imprimirPalavrasPortuguesPorUnidade(arvore->left, unidade, unidadeImpressa);

        // Verifica se a palavra em português está associada à unidade e imprime
        if (arvore->info1.englishWord != NULL)
        {
            Inglesbin *ingles = arvore->info1.englishWord;
            while (ingles)
            {
                if (buscar_unidade(ingles->unidades, unidade))
                {
                    if (!(*unidadeImpressa))
                    {
                        printf("%% Unidade %d\n", unidade);
                        *unidadeImpressa = 1;
                    }
                    printf("- %s\n", arvore->info1.portugueseWord);
                    break; // Palavra em português já encontrada para esta unidade
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }

        // Percorre a subárvore central
        imprimirPalavrasPortuguesPorUnidade(arvore->cent, unidade, unidadeImpressa);

        // Se houver um segundo elemento, verifica também
        if (arvore->nInfos == 2 && arvore->info2.englishWord != NULL)
        {
            Inglesbin *ingles = arvore->info2.englishWord;
            while (ingles)
            {
                if (buscar_unidade(ingles->unidades, unidade))
                {
                    if (!(*unidadeImpressa))
                    {
                        printf("%% Unidade %d\n", unidade);
                        *unidadeImpressa = 1;
                    }
                    printf("- %s\n", arvore->info2.portugueseWord);
                    break; // Palavra em português já encontrada para esta unidade
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }

        // Percorre a subárvore direita
        if (arvore->nInfos == 2)
        {
            imprimirPalavrasPortuguesPorUnidade(arvore->right, unidade, unidadeImpressa);
        }
    }
}
void imprimirPalavrasInglesPorUnidade(PortugueseTree *arvore, int unidade, int *unidadeImpressa)
{
    if (arvore)
    {
        // Percorre a subárvore esquerda
        imprimirPalavrasInglesPorUnidade(arvore->left, unidade, unidadeImpressa);

        // Verifica as palavras em inglês associadas ao primeiro elemento do nó
        if (arvore->info1.englishWord != NULL)
        {
            Inglesbin *ingles = arvore->info1.englishWord;
            while (ingles)
            {
                if (buscar_unidade(ingles->unidades, unidade))
                {
                    if (!(*unidadeImpressa))
                    {
                        printf("%% Unidade %d\n", unidade);
                        *unidadeImpressa = 1;
                    }
                    printf("- %s\n", ingles->palavraIngles);
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }

        // Percorre a subárvore central
        imprimirPalavrasInglesPorUnidade(arvore->cent, unidade, unidadeImpressa);

        // Verifica as palavras em inglês associadas ao segundo elemento do nó, se existir
        if (arvore->nInfos == 2 && arvore->info2.englishWord != NULL)
        {
            Inglesbin *ingles = arvore->info2.englishWord;
            while (ingles)
            {
                if (buscar_unidade(ingles->unidades, unidade))
                {
                    if (!(*unidadeImpressa))
                    {
                        printf("%% Unidade %d\n", unidade);
                        *unidadeImpressa = 1;
                    }
                    printf("- %s\n", ingles->palavraIngles);
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }

        // Percorre a subárvore direita
        if (arvore->nInfos == 2)
        {
            imprimirPalavrasInglesPorUnidade(arvore->right, unidade, unidadeImpressa);
        }
    }
}

void menu()
{
    printf("\n------------------------------------------------------------------------------------------------- \n");
    printf("\nMenu de opções:\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em português e as equivalentes em inglês.\n");
    printf("2 - Informar uma palavra em português e imprimir todas as palavras em inglês equivalentes.\n");
    printf("3 - Informar uma palavra em inglês e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("4 - Informar uma palavra em português e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("5 - Imprimir a árvore completa.\n");
    printf("6 - Imprimir todas as unidades existentes.\n");
    printf("7 - Imprimir todas as palavras em português de uma unidade específica.\n");
    printf("8 - Imprimir todas as palavras em inglês de uma unidade específica.\n");
    printf("0 - Sair.\n");
    printf("Escolha uma opção: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

int main()
{
    PortugueseTree *raiz = NULL;
    PortugueseTree *pai = NULL;

    char palavra[50];
    int unidade;
    int removido;
    loadFile("C:/Users/jorge/OneDrive/Documentos/GitHub/EstruturaDeDadosII/trabalhoEd2.txt", &raiz);
    int op;
    int unidadeImpressa = 0;
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
            printf("Insira a palavra em inglês que deseja remover: ");
            scanf("%s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%d", &unidade);

            // Chama a função para remover a palavra em inglês da árvore binária e da árvore 2-3
            removerTraducaoIngles(&raiz, palavra, unidade, &pai);

            printf("\nPalavra '%s' da unidade %d removida com sucesso.\n", palavra, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;

        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em português que deseja remover: ");
            scanf("%s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%d", &unidade);
            removerTraducaoPortugues(&raiz, palavra, unidade, &pai);
            printf("\n--------------------------------------------------------------- \n");
            break;

        case 5:
            printf("\n--------------------------------------------------------------- \n");
            exibir_tree23(raiz);
            printf("\n--------------------------------------------------------------- \n");
            break;

        case 6:
        {
            printf("\n--------------------------------------------------------------- \n");
            printf("Unidades existentes:\n");

            int unidades[100] = {0};
            int numUnidades = 0;
            imprimirUnidadesExistentes(raiz, unidades, &numUnidades);

            for (int i = 0; i < numUnidades; i++)
            {
                printf("Unidade: %d\n", unidades[i]);
            }
            printf("\n--------------------------------------------------------------- \n");
            break;
        }
        case 7:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja listar as palavras em português: ");
            scanf("%d", &unidade);

            imprimirPalavrasPortuguesPorUnidade(raiz, unidade, &unidadeImpressa);

            if (!unidadeImpressa)
            {
                printf("Nenhuma palavra encontrada para a unidade %d.\n", unidade);
            }

            printf("\n--------------------------------------------------------------- \n");
            break;
        case 8:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja listar as palavras em inglês: ");
            scanf("%d", &unidade);

            int unidadeImpressa = 0;
            imprimirPalavrasInglesPorUnidade(raiz, unidade, &unidadeImpressa);

            if (!unidadeImpressa)
            {
                printf("Nenhuma palavra em inglês encontrada para a unidade %d.\n", unidade);
            }

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
