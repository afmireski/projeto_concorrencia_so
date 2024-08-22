/**
 * Program: main.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor Ângelo
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
#define VAGAS_SALA 3 // Quantidade de alunos que o grupo pode ter
#define VAGAS_ATV1 2 // Quantidade de alunos que fizeram a atividade 1 no grupo
#define VAGAS_ATV2 1 // Quantidade de alunos que fizeram a atividade 2 no grupo

// Variáveis de controle

int vagas_grupo = VAGAS_SALA;
int n_entregas = 0; // variável que controla quantas atividades foram entregues
int n_alunos = 0;   // Variável que controla quantos alunos existem

sem_t entrar_grupo;     // Mutex que controla se um aluno pode entrar no grupo
sem_t grupo_formado;    // Mutex que controla se o grupo foi formado
sem_t grupo_desformado; // Mutex que controla se o grupo foi desformado
sem_t sair_grupo;       // Mutex que controla se um aluno pode sair do grupo
sem_t professor;        // Semáforo que controla se o professor pode, ou não, receber tarefas
sem_t vagas_aluno_at1;  // Semáforo que controla número de alunos que quer entregar a atividade 1
sem_t vagas_aluno_at2;  // Semáforo que controla número de alunos que quer entregar a atividade 2

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

    n_alunos = atoi(argv[1]); // Obtém a quantidade de alunos via CLI

    /* Inicializa alguns semáforos */
    Aluno **alunos = monta_alunos();
    sem_init(&entrar_grupo, 0, 1);     // Inicialmente, qualquer um pode entrar no grupo
    sem_init(&grupo_formado, 0, 0);    // Inicialmente, o grupo não está formado
    sem_init(&grupo_desformado, 0, 0); // Inicialmente, o grupo não está desformado
    sem_init(&sair_grupo, 0, 1);       // Inicialmente, qualquer um pode sair do grupo
    sem_init(&professor, 0, 0);        // O professor está no aguardo da primeira tarefa
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
    sem_destroy(&entrar_grupo);
    sem_destroy(&grupo_formado);
    sem_destroy(&grupo_desformado);
    sem_destroy(&sair_grupo);
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

        aluno_entrar_sala(aluno, &entrar_grupo, &grupo_formado, &vagas_grupo);

        aluno_entregar_atividade(aluno, &professor);

        aluno_sair_sala(aluno, &sair_grupo, &entrar_grupo, &grupo_formado, &vagas_grupo, &vagas_aluno_at1, &vagas_aluno_at2, &grupo_desformado);

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

    professor_finalizar_entrega_atividades();

    return NULL;
}

Aluno **monta_alunos()
{
    Aluno **alunos = (Aluno **)calloc(n_alunos, sizeof(Aluno *));

    int n_atividade02 = n_alunos / 3;             // 1/3 Atividade 02
    int n_atividade01 = n_alunos - n_atividade02; // 2/3 Atividade 01

#if DEBUG
    printf("Quantidade de alunos: %d\n", n_alunos);
    printf("Quantidade de atividade 01: %d\n", n_atividade01);
    printf("Quantidade de atividade 02: %d\n", n_atividade02);
#endif

    int atividades = 1;
    for (int i = 0; i < n_alunos; i++, atividades++)
    {
        Aluno *aluno = cria_aluno(i, atividades <= n_atividade01 ? 1 : 2);
        alunos[i] = aluno;
    }

    sem_init(&vagas_aluno_at1, 0, VAGAS_ATV1); // Inicializa semáforo para controlar o total de vagas no grupo para a atividade 01
    sem_init(&vagas_aluno_at2, 0, VAGAS_ATV2); // Inicializa semáforo para controlar o total de vagas no grupo para a atividade 02

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
