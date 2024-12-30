#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.c"
#include "arvbin.c"
#include "unidade.c"
#include <ctype.h>

void limparCaracteres(char *str)
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

void carregarArquivo(const char *nomeArquivo, Portugues23 **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];      
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = 0; 

        if (linha[0] == '%') 
        {
          
            if (sscanf(linha, "%% Unidade %d", &unidadeAtual) != 1)
            {
                printf("Erro ao interpretar a unidade na linha: %s\n", linha);
            }
        }
        else 
        {
            char palavraIngles[50], traducoesPortugues[200];

            
            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
               
                limparCaracteres(palavraIngles);

                char *traducaoPortugues = strtok(traducoesPortugues, ",");
                while (traducaoPortugues != NULL)
                {
                    
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    
                    limparCaracteres(traducaoPortugues);


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
    int op;
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