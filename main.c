/**
 * Program: main.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

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

// Funções declaradas
Aluno **monta_alunos();
void destroi_alunos(Aluno **chunks, int n_alunos);
void cria_threads(pthread_t *threads, Aluno **alunos);
void *acao_aluno(void *param);
void *acao_professor(void *param);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Quantidade de alunos não informada\n");
        return 1;
    }

    n_alunos = atoi(argv[1]);

    // Inicializa os semáforos
    iniciar_sistema(VAGAS_SALA);

    Aluno **alunos = monta_alunos();
    int n_threads = n_alunos + 1;
    pthread_t threads[n_threads];
    cria_threads(threads, alunos);

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    destroi_alunos(alunos, n_alunos);
    finalizar_sistema();

    return 0;
}

void *acao_aluno(void *param)
{
    Aluno *aluno = (Aluno *)param;

    aluno_fazer_atividade(aluno);
    aluno_terminar_atividade(aluno);

    aluno_aguardar_entrega(aluno);
    aluno_entrar_sala(aluno);
    aluno_entregar_atividade(aluno);
    aluno_sair_sala(aluno);

    return NULL;
}

void *acao_professor(void *param) {
    while (n_entregas < n_alunos) {
        professor_receber_atividade(&professor, &n_entregas);
        printf("Professor recebeu %d de %d atividades\n", n_entregas, n_alunos);
    }

    professor_finalizar_entrega_atividades(NULL);
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

    printf("Montou alunos\n");

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
    for (int i = 0; i < n_alunos; i++)
    {
        pthread_create(&threads[i], NULL, (void *)acao_aluno, alunos[i]);
#if DEBUG
        printf("Criou thread aluno %d\n", i);
#endif
    }

    pthread_create(&threads[n_alunos], NULL, (void *)acao_professor, NULL);
#if DEBUG
    printf("Criou thread professor\n");
#endif
}
