/**
 * Program: main.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <stdbool.h>   // bool
#include <pthread.h>   /* POSIX Threads */
#include <semaphore.h> /* Semaphore */

#include "aluno.h"
#include "professor.h"

#define DEBUG false
#define VAGAS_SALA 3

// Variáveis de controle

int vagas_sala = VAGAS_SALA;
int n_entregas = 0; // variável que controla quantas atividades foram entregues
int n_alunos = 0;

sem_t sala;            // Variável que controla se um grupo pode entrar na sala
sem_t professor;       // Semáforo que controla se o professor pode, ou não, receber tarefas
sem_t vagas_aluno_at1; // Semáforo que controla número de alunos que quer entregar a atividade 1
sem_t vagas_aluno_at2; // Semáforo que controla número de alunos que quer entregar a atividade 2

// -----------

Aluno **monta_alunos();

void destroi_alunos(Aluno **chunks, int n_alunos);

void cria_threads(pthread_t *threads, Aluno **alunos);

void *acao_aluno(void *param);

void *acao_professor(void *param);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Quantidade de alunos não informada\n");
        return 1;
    }

    n_alunos = atoi(argv[1]);

    /* Inicializa alguns semáforos */
    Aluno **alunos = monta_alunos();
    sem_init(&sala, 0, 1);      // A sala está inicialmente aberta
    sem_init(&professor, 0, 0); // O professor está no aguardo da primeira tarefa
    // --

    /* Declara as threads */
    int n_threads = n_alunos + 1; // +1 para o professor

    pthread_t threads[n_threads];
    cria_threads(threads, alunos);

#if DEBUG
    printf("...\n");
#endif

    /* Aguarda as threads terminarem */
    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
#if DEBUG
        printf("Thread %d terminou\n", i);
#endif
    }

    destroi_alunos(alunos, n_alunos);
    sem_destroy(&sala);
    sem_destroy(&professor);
    sem_destroy(&vagas_aluno_at1);
    sem_destroy(&vagas_aluno_at2);

    return 0;
}

void *acao_aluno(void *param)
{
    Aluno *aluno = (Aluno *)param;

    aluno_fazer_atividade(aluno);

    aluno_terminar_atividade(aluno);

    while (true)
    {

        aluno_aguardar_entrega(aluno, &vagas_aluno_at1, &vagas_aluno_at2);

        aluno_entrar_sala(aluno, &sala, &vagas_sala);

        aluno_entregar_atividade(aluno, &professor, &vagas_aluno_at1, &vagas_aluno_at2);

        aluno_sair_sala(aluno, &sala, &vagas_sala);

        break;
    }

    return NULL;
}

void *acao_professor(void *param)
{
    while (true)
    {
        if (n_entregas == n_alunos)
            break;

        professor_receber_atividade(&professor, &n_entregas);
        printf("professor recebeu %d de %d atividades\n", n_entregas, n_alunos);
    }

    professor_finalizar_entrega_atividades(&sala);

    return NULL;
}

Aluno **monta_alunos()
{
    Aluno **alunos = (Aluno **)calloc(n_alunos, sizeof(Aluno *));

    int n_atividade02 = n_alunos / 3;             // 1/3 Atividade 02
    int n_atividade01 = n_alunos - n_atividade02; // 2/3 Atividade 01

    printf("Quantidade de alunos: %d\n", n_alunos);
    printf("Quantidade de atividade 01: %d\n", n_atividade01);
    printf("Quantidade de atividade 02: %d\n", n_atividade02);

    int atividades = 1;
    for (int i = 0; i < n_alunos; i++, atividades++)
    {
        Aluno *aluno = cria_aluno(i, atividades <= n_atividade01 ? 1 : 2);
        alunos[i] = aluno;
    }

    int vagas_atv2 = VAGAS_SALA / 3;
    int vagas_atv1 = VAGAS_SALA - vagas_atv1;

    sem_init(&vagas_aluno_at1, 0, vagas_atv1);
    sem_init(&vagas_aluno_at2, 0, vagas_atv2);

#if DEBUG
    printf("Montou alunos\n");
#endif

    return alunos;
}

void destroi_alunos(Aluno **alunos, int n_alunos)
{
    for (int i = 0; i < n_alunos; i++)
    {
        free(alunos[i]);
    }
    free(alunos);
}

void cria_threads(pthread_t *threads, Aluno **alunos)
{
    int i;
    for (i = 0; i < n_alunos; i++)
    {
        pthread_create(&threads[i], NULL, (void *)acao_aluno, alunos[i]);
#if DEBUG
        printf("Criou thread aluno %d\n", i);
#endif
    }

    pthread_create(&threads[i], NULL, (void *)acao_professor, (void *)NULL);
#if DEBUG
    printf("Criou thread professor%d\n", i);
#endif
}
