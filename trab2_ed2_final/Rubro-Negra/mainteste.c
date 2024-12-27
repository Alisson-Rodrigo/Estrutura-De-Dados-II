#include "portugues.c"
#include "ingles.c"
#include "ingles.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int inserirPalavraPortugues(Arv_portugues **arvore, char *palavraPortugues, char *palavraIngles, int unidade)
{
    int resultado = 0; 

    Arv_portugues *noExistente = BuscarPalavra(arvore, palavraPortugues);

    if (noExistente != NULL)
    {
        Arv_ingles *traducao = noExistente->info.palavraIngles;
        while (traducao != NULL)
        {
            if (strcmp(traducao->palavraIngles, palavraIngles) == 0 && traducao->unidade == unidade)
            {
                printf("A traducao '%s' para '%s' ja existe na unidade %d. Ignorando.\n",
                       palavraIngles, palavraPortugues, unidade);
                resultado = 0; 
                break;
            }
            traducao = traducao->dir;
        }

        if (traducao == NULL) 
        {
            AdicionarTraducaoEN(noExistente, palavraIngles, unidade);
            resultado = 1;
        }
    }
    else
    {
        Info novoInfo = CriarInfo(palavraPortugues, palavraIngles, unidade);
        inserirArvRec(arvore, &novoInfo);
        resultado = 1;
    }

    return resultado; 
}


void carregarArquivo(const char *nomeArquivo, Arv_portugues **arvore)
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
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        }
        else
        {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);

            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while (traducaoPortugues != NULL)
            {
                while (*traducaoPortugues == ' ')
                    traducaoPortugues++; 

                if (strlen(traducaoPortugues) > 0)
                {
                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                }

                traducaoPortugues = strtok(NULL, ",;");
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}


void imprimirPalavrasPorUnidade(Arv_portugues *arvore, int unidade)
{
    if (arvore != NULL)
    {
        Arv_ingles *traducao = arvore->info.palavraIngles;
        while (traducao != NULL)
        {
            if (traducao->unidade == unidade)
            {
                printf("%s: %s;\n", arvore->info.palavraPortugues, traducao->palavraIngles);
            }
            traducao = traducao->dir; 
        }


        imprimirPalavrasPorUnidade(arvore->esq, unidade);
        imprimirPalavrasPorUnidade(arvore->dir, unidade);
    }
}



void imprimirTraducoesIngles(Arv_portugues *arvore, char *palavraPortugues)
{
    if (arvore != NULL)
    {
     
        if (strcmp(arvore->info.palavraPortugues, palavraPortugues) == 0)
        {
            printf("Traducoes em Ingles para '%s': %s\n",
                   arvore->info.palavraPortugues, arvore->info.palavraIngles->palavraIngles);
        }

        
        imprimirTraducoesIngles(arvore->esq, palavraPortugues);
        imprimirTraducoesIngles(arvore->dir, palavraPortugues);
    }
}



void removerPalavraInglesEArvore(Arv_portugues **arvore, char *palavraIngles, int unidade)
{
    if (*arvore != NULL)
    {

       
        removerPalavraInglesEArvore(&(*arvore)->esq, palavraIngles, unidade);

        if (*arvore != NULL && (*arvore)->info.palavraIngles != NULL)
        {
            Arv_ingles *raizIngles = (*arvore)->info.palavraIngles;
            int removida = 0;

            
            Arv_ingles *temp = raizIngles;
            while (temp != NULL)
            {
                if (strcmp(temp->palavraIngles, palavraIngles) == 0 && temp->unidade == unidade)
                {
                    removida = removerPalavraIngles(&(*arvore)->info.palavraIngles, palavraIngles);
                    printf("Debug: A palavra '%s' na unidade %d foi removida.\n", palavraIngles, unidade);
                    break;
                }
                temp = temp->dir; 
            }

            
            if (removida && (*arvore)->info.palavraIngles == NULL)
            {
                printf("Debug: A palavra '%s' era a única tradução. Removendo nó correspondente na árvore principal.\n", palavraIngles);
                RemoverNo(arvore, (*arvore)->info.palavraPortugues);
            }
        }
        if (*arvore != NULL)
        {
            removerPalavraInglesEArvore(&(*arvore)->dir, palavraIngles, unidade);
        }
    }
}

void removerPalavraPortuguesEUnidade(Arv_portugues **arvore, char *palavraPortugues, int unidade)
{
    if (*arvore != NULL)
    {
        
        Arv_portugues *no = BuscarPalavra(arvore, palavraPortugues);

        if (no != NULL)
        {
            
            Arv_ingles *raizIngles = no->info.palavraIngles;
            int traducaoRemovida = 0;

            if (raizIngles != NULL)
            {
                Arv_ingles *temp = raizIngles;

                while (temp != NULL)
                {
                    if (temp->unidade == unidade)
                    {
                        traducaoRemovida = removerPalavraIngles(&raizIngles, temp->palavraIngles);
                        break;
                    }
                    temp = temp->dir; 
                }
            }

            
            no->info.palavraIngles = raizIngles;

            if (traducaoRemovida)
            {
                printf("A traducao na unidade %d foi removida.\n", unidade);

                
                if (no->info.palavraIngles == NULL)
                {
                    printf("A palavra '%s' nao possui mais traducoes e sera removida da arvore principal.\n", palavraPortugues);
                    RemoverNo(arvore, palavraPortugues);
                }
            }
            else
            {
                printf("Nenhuma traducao encontrada para a palavra '%s' na unidade %d.\n", palavraPortugues, unidade);
            }
        }
        else
        {
            printf("A palavra '%s' nao foi encontrada na arvore.\n", palavraPortugues);
        }
    }
}


int main()
{
    Arv_portugues *arvore = NULL;
    int opcao, unidade;
    char palavraPortugues[50], palavraIngles[50];

    
    carregarArquivo("C:/Users/purolight/Documents/GitHub/ED2-JOB2/Trabalho_segunda_provaVERSAO-REFERENCIA/trabalhoEd2.txt", &arvore);

    while (1)
    {
        printf("\nMENU\n");
        printf("1. Exibir palavras associadas a uma unidade\n");
        printf("2. Exibir traducoes em ingles para uma palavra em portugues\n");
        printf("3. Remover palavra em ingles e atualizacoes associadas\n");
        printf("4. Remover palavra em portugues e traducoes associadas\n");
        printf("5. Sair do programa\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1: 
        {
            int continuar = 1; 
            do
            {
                printf("Informe a unidade (1 a 2): ");
                scanf("%d", &unidade);

                
                if (unidade < 1 || unidade > 2)
                {
                    printf("Unidade invalida.\n");
                    printf("Deseja digitar a unidade novamente ou voltar ao menu principal?\n");
                    printf("Digite 1 para tentar novamente ou 0 para voltar ao menu principal: ");
                    scanf("%d", &continuar);
                }
                else
                {
                   
                    printf("Palavras da unidade %d:\n", unidade);
                    imprimirPalavrasPorUnidade(arvore, unidade);
                    continuar = 0; 
                }
            } while (continuar == 1);
        }
        break;

        case 2: 
        {
            printf("Informe a palavra em portugues: ");
            scanf(" %[^\n]", palavraPortugues); 
            printf("Traducoes em ingles para '%s':\n", palavraPortugues);
            imprimirTraducoesIngles(arvore, palavraPortugues);
        }
        break;

        case 3: 
        {
            printf("Informe a palavra em ingles: ");
            scanf(" %[^\n]", palavraIngles);

            int continuar = 1; 
            do
            {
                printf("Informe a unidade (1 a 2): ");
                scanf("%d", &unidade);

               
                if (unidade < 1 || unidade > 2)
                {
                    printf("Unidade invalida.\n");
                    printf("Deseja digitar a unidade novamente ou voltar ao menu principal?\n");
                    printf("Digite 1 para tentar novamente ou 0 para voltar ao menu principal: ");
                    scanf("%d", &continuar);
                }
                else
                {
                    removerPalavraInglesEArvore(&arvore, palavraIngles, unidade);
                    continuar = 0; 
                }
            } while (continuar == 1);
        }
        break;

        case 4: 
        {
            printf("Informe a palavra em portugues: ");
            scanf(" %[^\n]", palavraPortugues);

            int continuar = 1; 
            do
            {
                printf("Informe a unidade (1 a 2): ");
                scanf("%d", &unidade);

               
                if (unidade < 1 || unidade > 2)
                {
                    printf("Unidade invalida.\n");
                    printf("Deseja digitar a unidade novamente ou voltar ao menu principal?\n");
                    printf("Digite 1 para tentar novamente ou 0 para voltar ao menu principal: ");
                    scanf("%d", &continuar);
                }
                else
                {
                    removerPalavraPortuguesEUnidade(&arvore, palavraPortugues, unidade);
                    continuar = 0; 
                }
            } while (continuar == 1);
        }
        break;

        case 5: 
        {
            printf("Saindo do programa...\n");
            exit(0);
        }
        break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}

