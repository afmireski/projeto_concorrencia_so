/**
 * Program: aluno.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "aluno.h"

typedef struct aluno {
    int id;
    int tipo_atividade;
    bool concluiu_atividade;
} Aluno;

// Contadores de alunos por atividade
int alunos_atividade1 = 0;
int alunos_atividade2 = 0;

// Mutex e condição para sincronização de grupos
pthread_mutex_t grupo_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t grupo_formado = PTHREAD_COND_INITIALIZER;

// Mutex e semáforo para controle da sala
pthread_mutex_t sala_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t vaga_sala; // Semáforo para controlar vagas na sala

void iniciar_sistema(int vagas) {
    sem_init(&vaga_sala, 0, vagas); // Inicializa o semáforo com o número de vagas
}

Aluno *cria_aluno(int id, int tipo_atividade) {
    Aluno *aluno = (Aluno *)malloc(sizeof(Aluno));
    aluno->id = id;
    aluno->tipo_atividade = tipo_atividade;
    aluno->concluiu_atividade = false;
    return aluno;
}

void aluno_fazer_atividade(Aluno *aluno) {
    printf("alunoSO_%d_AT%d fazendo atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_terminar_atividade(Aluno *aluno) {
    aluno->concluiu_atividade = true;
    printf("alunoSO_%d_AT%d terminou atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_aguardar_entrega(Aluno *aluno) {
    printf("alunoSO_%d_AT%d chega para entrega\n", aluno->id, aluno->tipo_atividade);

    pthread_mutex_lock(&grupo_mutex);
    
    if (aluno->tipo_atividade == 1) {
        alunos_atividade1++;
    } else {
        alunos_atividade2++;
    }

    // Verifica se um grupo pode ser formado
    if (alunos_atividade1 >= 2 && alunos_atividade2 >= 1) {
        printf("Grupo formado: 2 de atividade 1 e 1 de atividade 2\n");
        alunos_atividade1 -= 2;
        alunos_atividade2 -= 1;
        pthread_cond_broadcast(&grupo_formado);
    } else {
        pthread_cond_wait(&grupo_formado, &grupo_mutex);
    }

    pthread_mutex_unlock(&grupo_mutex);
}

void aluno_entrar_sala(Aluno *aluno) {
    sem_wait(&vaga_sala);  // Espera por uma vaga na sala
    pthread_mutex_lock(&sala_mutex);
    printf("alunoSO_%d_AT%d entrou na sala\n", aluno->id, aluno->tipo_atividade);
    pthread_mutex_unlock(&sala_mutex);
}

void aluno_entregar_atividade(Aluno *aluno) {
    pthread_mutex_lock(&sala_mutex);
    printf("alunoSO_%d_AT%d entrega atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
    pthread_mutex_unlock(&sala_mutex);
}

void aluno_sair_sala(Aluno *aluno) {
    pthread_mutex_lock(&sala_mutex);
    printf("alunoSO_%d_AT%d saiu da sala\n", aluno->id, aluno->tipo_atividade);
    pthread_mutex_unlock(&sala_mutex);
    sem_post(&vaga_sala); // Libera uma vaga na sala
}

void finalizar_sistema() {
    sem_destroy(&vaga_sala); // Destrói o semáforo
}
