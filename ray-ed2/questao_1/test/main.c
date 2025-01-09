#include "maintest.h"

#define TEST_SIZE 30

Portugues23 *buscaTestes(Portugues23 **tree, char *codigo, int n)
{
    Portugues23 *no = NULL;
    if (*tree != NULL)
    {
        // Comparando strings (codigo e palavras na árvore)
        if (strcmp(codigo, (*tree)->info1.palavraPortugues) == 0)
        {
            printf("Info 1: %s\n\n", (*tree)->info1.palavraPortugues);
            no = *tree;
        }
        else if ((*tree)->nInfos == 2 && strcmp(codigo, (*tree)->info2.palavraPortugues) == 0)
        {
            printf("Info 2: %s\n\n", (*tree)->info2.palavraPortugues);
            no = *tree;
        }
        else if(strcmp(codigo, (*tree)->info1.palavraPortugues) < 0)
        {
            printf("%d-esquerda \n", n);
            no = buscaTestes(&(*tree)->esq, codigo, n + 1);
        }
        else if ((*tree)->nInfos == 1 || strcmp(codigo, (*tree)->info2.palavraPortugues) < 0)
        {
            printf("%d-centro \n", n);
            no = buscaTestes(&(*tree)->cent, codigo, n + 1);
        }
        else
        {
            printf("%d-direita \n", n);
            no = buscaTestes(&(*tree)->dir, codigo, n + 1);
        }
    }
    return no;
}

void carregarArquivoTeste(const char *nomeArquivo, Portugues23 **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL)
    {
        char linha[256];

        char unidadeAtual[50];

        while (fgets(linha, sizeof(linha), arquivo))
        {

            if (linha[0] == '%')
            {
                // Atualiza a unidade corretamente
                sscanf(linha, "%% Unidade %[^\n]", unidadeAtual);
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

                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }

        fclose(arquivo);
    }
}

void buscaSequencial(char palavras[][20], Portugues23 *raiz)
{
    clock_t start, end;
    double totalTime, media;

    for (int i = 0; i < TEST_SIZE; i++){
        start = clock();
        buscaTestes(&raiz, palavras[i], 0); // Busca na árvore
        end = clock();
        totalTime += (double)(end - start) / CLOCKS_PER_SEC;
        printf("Tempo de busca %s: %.8lf s\n", palavras[i], totalTime);
        media+=totalTime;
        }

    media = (media / TEST_SIZE);
    printf("\nTempo medio de busca : %.8lf s\n", media);
}

void buscaAleatoria(char palavrasAleatorias[][20], Portugues23 *raiz)
{
    clock_t start, end;
    double totalTime, media;

    
    for (int i = 0; i < TEST_SIZE; i++){
        start = clock();
        buscaTestes(&raiz, palavrasAleatorias[i], 0); // Busca na árvore
        end = clock();
        totalTime += (double)(end - start) / CLOCKS_PER_SEC;
        printf("Tempo de busca %s: %.8lf s\n", palavrasAleatorias[i], totalTime);
        media+=totalTime;
        }

    media = (media / TEST_SIZE);
    printf("\nTempo medio de busca (aleatorio): %.8lf s\n", media);
}

int main_teste(void)
{

    Portugues23 *raiz = NULL;
    char palavras[30][20] = {
        "abacaxi", "aluno", "amigo", "avião", "bolsa", "cachorro", "carro", "casa",
        "chave", "computador", "escola", "felicidade", "flor", "floresta", "gato",
        "guerra", "janela", "jardim", "livro", "lua", "mesa", "oceano", "porta",
        "praia", "roupas", "sol", "tecnologia", "telefone", "universidade", "água"};
    char palavrasAleatorias[30][20] = {
        "universidade", "bolsa", "roupas", "felicidade", "sol", "avião", "gato", "tecnologia", "escola", "oceano",
        "cachorro", "abacaxi", "guerra", "floresta", "computador", "flor", "porta", "lua", "praia", "jardim", "mesa",
        "carro", "amigo", "janela",  "casa", "água", "telefone", "livro", "chave", "aluno"};

    carregarArquivoTeste("../teste.txt", &raiz);

    // Busca na árvore
    printf("\nBusca crescente:\n");
    buscaSequencial(palavras, raiz);
    printf("\nBusca aleatoria:\n");
    buscaAleatoria(palavrasAleatorias, raiz);

    freeTree(&raiz);
    
    return 0;
}
