#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portugues_arv.c"
#include "ingles_arv.c"
// #include "arvbin.h"

int inserirPalavraPortugues(Arv_pt **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    Info promove;
    Arv_pt *pai = NULL;
    int inseriu;

    // Busca a palavra na árvore
    Arv_pt *noExistente = NULL;
    noExistente =  BuscarNoPorPalavra(arvore, palavraPortugues);

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

void carregarArquivo(const char *nomeArquivo, Arv_pt **arvore)
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
            // Atualiza a unidade corretamente
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        }
        else
        {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);
            
            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while(traducaoPortugues != NULL)
            {
                while (*traducaoPortugues == ' ') 
                    traducaoPortugues++;

                inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                traducaoPortugues = strtok(NULL, ",;");    
            }
            
        }
    }

    fclose(arquivo);
}

void menu(Arv_pt *raiz)
{
    int opcao;
    do
    {
        printf("\nMenu de Opcoes:\n");
        printf("1. Informar uma unidade e listar palavras em portugues e suas traducoes\n");
        printf("2. Informar uma palavra em portugues e listar traducoes em ingles\n");
        printf("3. Remover palavra em ingles e atualizacoes associadas\n");
        printf("4. Remover palavra em portugues e traducoes associadas\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1: // Informar unidade e listar palavras e traduções
            {
                int unidade;
                int continuar = 1; // Controla se o usuário deseja continuar digitando
                do
                {
                    printf("Informe o numero da unidade: ");
                    scanf("%d", &unidade);

                    // Valida a unidade
                    if (unidade < 1 || unidade > 2) // Ajuste para suportar 1 a 2 unidades
                    {
                        printf("Unidade invalida.\n");
                        printf("Deseja digitar a unidade novamente ou voltar ao menu principal?\n");
                        printf("Digite 1 para tentar novamente ou 0 para voltar ao menu principal: ");
                        scanf("%d", &continuar);
                    }
                    else
                    {
                        // Exibe as palavras e traduções para a unidade fornecida
                        printf("\nPalavras da unidade %d:\n", unidade);
                        ImprimirPorUnidade(raiz, unidade);
                        printf("\n");
                        continuar = 0; // Encerra a repetição
                    }
                } while (continuar == 1);

                break;

            }

            case 2: // Informar palavra em português e listar traduções em inglês
            {
                char palavraPortugues[100];
                printf("Informe a palavra em portugues: ");
                scanf(" %[^\n]s", palavraPortugues); // Lê a palavra com espaços
                printf("\nTraducoes em ingles para '%s':\n", palavraPortugues);
                ExibirTraducoesPortugues(&raiz, palavraPortugues);
                break;
            }

            case 3:
            {
                char palavraIngles[100];
                int unidade;
                printf("Informe a palavra em ingles: ");
                scanf(" %[^\n]s", palavraIngles);
                printf("Informe a unidade: ");
                scanf("%d", &unidade);
                RemoverTraducoesInglesNaUnidade(&raiz, palavraIngles, unidade);
                break;
            }

            case 4:
            {
                char palavraPortugues[100];
                printf("Informe a palavra em portugues: ");
                scanf(" %[^\n]s", palavraPortugues);
                RemoverPalavraArvore23(&raiz, palavraPortugues);
                break;

            }

            case 0: // Sair do programa
                printf("Saindo do programa...\n");
                break;

            default: // Opção inválida
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}




int main() {
    Arv_pt *raiz = NULL;

    // Carregar o arquivo com palavras e traduções
    carregarArquivo("C:/Users/PurooLight/Documents/GitHub/ED2-JOB2/trab2_ed2_final/trabalhoEd2.txt", &raiz);


    // Chamar o menu interativo
    menu(raiz);

    // Liberar memória alocada para a árvore
    LiberarArvore23(raiz);

    return 0;
}
