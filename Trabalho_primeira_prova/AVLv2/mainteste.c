// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.c"
#include "alunos.c"
#include "disciplinas.c"
#include "matricula.c"
#include "notas.c"

void exibir_menu()
{
    printf("\n--- Menu ---\n");
    printf("1. Cadastrar novo curso\n");
    printf("2. Cadastrar novo aluno\n");
    printf("3. Cadastrar nova disciplina\n");
    printf("4. Cadastrar nova matricula\n");
    printf("5. Cadastrar Nota\n");
    printf("6. Listar alunos de um curso\n");
    printf("7. Mostrar todos os cursos\n");
    printf("8. Mostrar todas as disciplinas de um curso\n");
    printf("9. Mostrar todas as disciplinas de um determinado periodo\n");
    printf("10. Mostrar todas as disciplinas que o aluno está matriculado\n");
    printf("11. Mostrar todas as notas de disciplinas de um determinado período de um aluno\n");
    printf("12. Mostrar a nota de uma disciplina de um aluno (inclui período e carga horária)\n");
    printf("13. Remover uma disciplina de um curso (sem alunos matriculados)\n");
    printf("14. Remover uma disciplina da matrícula de um aluno\n");
    printf("15. Mostrar o histórico de um aluno\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

int main()
{
    Arvore_curso *raiz_cursos = NULL;
    Aluno *raiz_alunos = NULL;
    int opcao = -1; // Inicialização correta
    int codigo, codigo_curso, matricula_num, carga_horaria, periodo;
    float nota; // Variável para armazenar a nota
    char nome[100], semestre_cursado[100];
    Aluno *aluno;
    Arvore_curso *curso;
    arvore_disciplinas *disciplina;

    while (opcao != 0)
    {
        exibir_menu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            // Cadastro de curso
            printf("Digite o codigo do curso: ");
            scanf("%d", &codigo);

            if (buscar_curso(raiz_cursos, codigo) != NULL)
            {
                printf("Codigo de curso ja existente. Cadastro do curso nao realizado.\n");
                break;
            }

            printf("Digite o nome do curso: ");
            scanf(" %[^\n]", nome);

            printf("Digite o periodo do curso: ");
            scanf("%d", &periodo);

            curso = criar_curso();
            curso->codigo = codigo;
            curso->periodo = periodo;
            strcpy(curso->nome, nome);

            raiz_cursos = inserir_curso(raiz_cursos, curso);
            printf("Curso cadastrado com sucesso!\n");
            break;

        case 2:
            // Cadastro de aluno
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            if (buscar_aluno(raiz_alunos, matricula_num) != NULL)
            {
                printf("Matricula ja existente. Cadastro do aluno nao realizado.\n");
                break;
            }
            printf("Digite o nome do aluno: ");
            scanf(" %[^\n]", nome);
            printf("Digite o codigo do curso do aluno: ");
            scanf("%d", &codigo_curso);

            // Verificar se o codigo do curso existe
            curso = buscar_curso(raiz_cursos, codigo_curso);
            if (curso == NULL)
            {
                printf("Codigo de curso nao encontrado. Cadastro do aluno nao realizado.\n");
                break;
            }

            aluno = criar_aluno();
            aluno->matricula = matricula_num;
            strcpy(aluno->nome, nome);
            aluno->codigo_curso = codigo_curso;
            aluno->raiz_matriculas = NULL;
            aluno->raiz_notas = NULL;

            raiz_alunos = inserir_aluno(raiz_alunos, aluno);
            printf("Aluno cadastrado com sucesso!\n");
            break;

        case 3:
            printf("Digite o codigo do curso da disciplina: ");
            scanf("%d", &codigo_curso);
            curso = buscar_curso(raiz_cursos, codigo_curso);
            if (curso == NULL)
            {
                printf("Codigo de curso nao encontrado. Cadastro da disciplina nao realizado.\n");
                break;
            }

            printf("Digite o codigo da disciplina: ");
            scanf("%d", &codigo);
            if (buscar_disciplina(curso->raiz_disciplinas, codigo) != NULL)
            {
                printf("Codigo de disciplina ja existente. Cadastro da disciplina nao realizado.\n");
                break;
            }

            printf("Digite o nome da disciplina: ");
            scanf(" %[^\n]", nome);
            printf("Digite a carga horaria da disciplina: ");
            scanf("%d", &carga_horaria);

            if (carga_horaria < 30 || carga_horaria > 90 || carga_horaria % 15 != 0)
            {
                printf("Carga horaria invalida. Cadastro da disciplina nao realizado.\n");
                break;
            }

            printf("Digite o periodo da disciplina: ");
            scanf("%d", &periodo);

            if (periodo < 1 || periodo > curso->periodo)
            {
                printf("Periodo invalido. Cadastro da disciplina nao realizado.\n");
                break;
            }

            disciplina = criar_disciplina();
            disciplina->codigo = codigo;
            strcpy(disciplina->nome, nome);
            disciplina->carga_horaria = carga_horaria;
            disciplina->periodo = periodo;

            curso->raiz_disciplinas = inserir_disciplina(curso->raiz_disciplinas, disciplina);
            printf("Disciplina cadastrada com sucesso.\n");
            break;

        case 4:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }

            curso = buscar_curso(raiz_cursos, aluno->codigo_curso);
            if (curso == NULL)
            {
                printf("Curso do aluno nao encontrado.\n");
                break;
            }

            printf("Digite o codigo da disciplina: ");
            scanf("%d", &codigo);
            disciplina = buscar_disciplina(curso->raiz_disciplinas, codigo);
            if (disciplina == NULL)
            {
                printf("Disciplina nao encontrada.\n");
                break;
            }

            if (buscar_matricula(aluno->raiz_matriculas, codigo) != NULL)
            {
                printf("Disciplina já matriculada.\n");
                break;
            }

            arvore_matricula *nova_matricula = criar_matricula();
            nova_matricula->codigo_disciplina = codigo;
            aluno->raiz_matriculas = inserir_matriculas(aluno->raiz_matriculas, nova_matricula);
            printf("Matricula cadastrada com sucesso.\n");
            break;

        case 5:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }

            printf("Digite o codigo da disciplina para cadastrar a nota: ");
            scanf("%d", &codigo);
            if (buscar_matricula(aluno->raiz_matriculas, codigo) == NULL)
            {
                printf("Disciplina nao matriculada pelo aluno. Cadastro da nota nao realizado.\n");
                break;
            }

            printf("Digite o semestre cursado: ");
            scanf("%s", semestre_cursado);

            printf("Digite a nota da disciplina: ");
            scanf("%f", &nota);

            arvore_notas *nova_nota = criar_nota();
            nova_nota->codigo_disciplina = codigo;
            nova_nota->nota_final = nota;
            strcpy(nova_nota->semestre_cursado, semestre_cursado);
            aluno->raiz_notas = inserir_nota(aluno->raiz_notas, nova_nota);

            aluno->raiz_matriculas = remover_matricula(aluno->raiz_matriculas, codigo);

            printf("Nota cadastrada com sucesso e disciplina removida das matriculas.\n");
            break;

        case 6:
            printf("Digite o codigo do curso: ");
            scanf("%d", &codigo);

            curso = buscar_curso(raiz_cursos, codigo);
            if (curso == NULL)
            {
                printf("Curso nao encontrado.\n");
                break;
            }

            printf("Alunos do curso %s:\n", curso->nome);
            imprimir_alunos(raiz_alunos, codigo);
            break;

        case 7:
            if (raiz_cursos == NULL)
            {
                printf("Nenhum curso cadastrado.\n");
                break;
            }
            printf("Cursos cadastrados:\n");
            imprimir_cursos(raiz_cursos);
            break;

        case 8:

            printf("Digite o codigo do curso: ");
            scanf("%d", &codigo);
            curso = buscar_curso(raiz_cursos, codigo);
            if (curso == NULL)
            {
                printf("Curso nao encontrado.\n");
                break;
            }
            printf("Disciplinas do curso %s:\n", curso->nome);
            imprimir_disciplinas(curso->raiz_disciplinas);
            break;

        case 9:

            printf("Digite o codigo do curso: ");
            scanf("%d", &codigo);
            curso = buscar_curso(raiz_cursos, codigo);
            if (curso == NULL)
            {
                printf("Curso nao encontrado.\n");
                break;
            }
            printf("Digite o periodo: ");
            scanf("%d", &periodo);
            printf("Disciplinas do curso %s do periodo %d:\n", curso->nome, periodo);
            imprimir_disciplinas_periodo(curso->raiz_disciplinas, periodo);
            break;

        case 10:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }
            printf("Disciplinas do aluno %s:\n", aluno->nome);
            imprimir_matricula(aluno->raiz_matriculas);
            break;
        case 11:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }
            printf("Digite o periodo: ");
            scanf("%d", &periodo);
            if (aluno->raiz_notas == NULL)
            {
                printf("Nenhuma nota cadastrada para o aluno.\n");
                break;
            }
            nova_nota = buscar_notas(aluno->raiz_notas, periodo);
            if (nova_nota == NULL)
            {
                printf("Nenhuma nota cadastrada para o aluno no periodo %d.\n", periodo);
                break;
            }
            printf("Notas do aluno %s no periodo %d:\n", aluno->nome, periodo);
            imprimir_notas(nova_nota);
            break;

        case 12:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }
            printf("Digite o codigo da disciplina: ");
            scanf("%d", &codigo);
            arvore_notas *nota_busca = buscar_notas(aluno->raiz_notas, codigo);
            if (nota_busca == NULL)
            {
                printf("Disciplina nao encontrada.\n");
                break;
            }
            curso = buscar_curso(raiz_cursos, aluno->codigo_curso);
            if (curso == NULL)
            {
                printf("Curso do aluno nao encontrado.\n");
                break;
            }
            disciplina = buscar_disciplina(curso->raiz_disciplinas, codigo);
            if (disciplina == NULL)
            {
                printf("Disciplina nao encontrada no curso.\n");
                break;
            }
            printf("Nota do aluno %s na disciplina %d: %.2f\n", aluno->nome, nota_busca->codigo_disciplina, nota_busca->nota_final);
            printf("Periodo: %d\n", disciplina->periodo);
            printf("Carga Horaria: %d\n", disciplina->carga_horaria);
            break;

        case 13:
            printf("Digite o codigo do curso: ");
            scanf("%d", &codigo);
            curso = buscar_curso(raiz_cursos, codigo);
            if (curso == NULL)
            {
                printf("Curso nao encontrado.\n");
                break;
            }

            printf("Digite o codigo da disciplina: ");
            scanf("%d", &codigo);
            arvore_disciplinas *disciplina_remover = buscar_disciplina(curso->raiz_disciplinas, codigo);
            if (disciplina_remover == NULL)
            {
                printf("Disciplina nao encontrada.\n");
                break;
            }

            int alunos_matriculados = 0;
            for (Aluno *a = raiz_alunos; a != NULL; a = a->prox)
            {
                if (buscar_matricula(a->raiz_matriculas, codigo) != NULL)
                {
                    alunos_matriculados = 1;
                    break;
                }
            }

            if (alunos_matriculados)
            {
                printf("Disciplina nao pode ser removida pois ha alunos matriculados ou notas cadastradas.\n");
            }
            else
            {
                curso->raiz_disciplinas = remover_disciplina(curso->raiz_disciplinas, codigo);
                printf("Disciplina removida com sucesso.\n");
            }
            break;

        case 14:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }
            printf("Digite o codigo da disciplina: ");
            scanf("%d", &codigo);
            arvore_matricula *matricula = buscar_matricula(aluno->raiz_matriculas, codigo);
            if (matricula == NULL)
            {
                printf("Disciplina nao encontrada na matricula do aluno.\n");
                break;
            }
            aluno->raiz_matriculas = remover_matricula(aluno->raiz_matriculas, codigo);
            printf("Disciplina removida com sucesso da matricula do aluno.\n");
            break;

        case 15:
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula_num);
            aluno = buscar_aluno(raiz_alunos, matricula_num);
            if (aluno == NULL)
            {
                printf("Matricula nao encontrada.\n");
                break;
            }
            printf("Historico do aluno %s:\n", aluno->nome);
            imprimir_historico(aluno, raiz_cursos);
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
