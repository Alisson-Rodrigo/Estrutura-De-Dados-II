#include <stdio.h>
#include <stdlib.h>
#include "23.c"
#include "alocacao.c"



int menu()
{
    int op;
    printf("\nMENU");
    printf("\n[1] - Alocar Nós");
    printf("\n[2] - Liberar Nós");
    printf("\n[3] - Exibir Nós (Pré-Ordem)");
    printf("\n[4] - Exibir Nós (Em-Ordem)");
    printf("\n[5] - Exibir Nós (Pós-Ordem)");
    printf("\n[0] - Sair");
    leia_int("\nOpção escolhida: ", &op);

    return op;
}

int main_main()
{
    Arvore23 *arvore;
    arvore = arvore23_criar();

    int maximo = 0;

    do
    {
        leia_int("Quantidade máxima de Blocos de memória: ", &maximo);
        if(maximo <= 0)
            printf("\nDigite uma quantidade positiva!\n\n");
    } while(maximo <= 0);

    int minimo = cadastrar_nos(&arvore, maximo);

    int op, quant_nos;
    do
    {
        op = menu();
        
        switch(op)
        {
            case 1:
                leia_numero_no("\nQuantidade de nós a serem alocados: ", &quant_nos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quant_nos, LIVRE);
                break;

            case 2:
                leia_numero_no("\nQuantidade de nós a serem liberados: ", &quant_nos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quant_nos, OCUPADO);
                break;

            case 3:
                printf("\nExibindo árvore [Pré-Ordem]\n");
                arvore23_exibir_pre(arvore);
                break;

            case 4:
                printf("\nExibindo árvore [Em-Ordem]\n");
                arvore23_exibir_ordem(arvore);
                break;

            case 5:
                printf("\nExibindo árvore [Pós-Ordem]\n");
                arvore23_exibir_pos(arvore);
                break;

            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpção inválida\n");
        }
    } while (op != 0);
    
    arvore23_desalocar(&arvore);
    return 0;
}

int main()
{
    //main_teste();
    main_main();

    return 0;
}