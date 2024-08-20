/**
 * Program: aluno.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include "aluno.h"

// Variáveis de controle de grupo
static int alunos_atv1_no_grupo = 0;
static int alunos_atv2_no_grupo = 0;

// Mutex para controlar o acesso às variáveis de grupo
static pthread_mutex_t grupo_mutex = PTHREAD_MUTEX_INITIALIZER;
// Semáforo para bloquear a entrada na sala até que um grupo válido seja formado
static sem_t grupo_pronto;

Aluno *cria_aluno(int id, int tipo_atividade)
{
    Aluno *aluno = (Aluno *)malloc(sizeof(Aluno));
    aluno->id = id;
    aluno->tipo_atividade = tipo_atividade;
    aluno->concluiu_atividade = false;
    return aluno;
}

void aluno_fazer_atividade(Aluno *aluno)
{
    printf("alunoSO_%d_AT%d fazendo atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_terminar_atividade(Aluno *aluno)
{
    aluno->concluiu_atividade = true;
    printf("alunoSO_%d_AT%d terminou atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_aguardar_entrega(Aluno *aluno, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2)
{
    printf("alunoSO_%d_AT%d chega para entrega\n", aluno->id, aluno->tipo_atividade);

    pthread_mutex_lock(&grupo_mutex);

    // Controle de formação de grupo
    if (aluno->tipo_atividade == 1)
    {
        sem_wait(vagas_aluno_atv1); // Espera ter vagas no grupo da atividade 1
        alunos_atv1_no_grupo++;
    }
    else
    {
        sem_wait(vagas_aluno_atv2); // Espera ter vagas no grupo da atividade 2
        alunos_atv2_no_grupo++;
    }

    // Se o grupo estiver completo, libera o acesso à sala
    if (alunos_atv1_no_grupo == 2 && alunos_atv2_no_grupo == 1)
    {
        sem_post(&grupo_pronto);
    }
    pthread_mutex_unlock(&grupo_mutex);

    // Aguarda até que o grupo esteja pronto
    sem_wait(&grupo_pronto);
}

void aluno_entrar_sala(Aluno *aluno, sem_t *sala, int *vagas_sala)
{
    sem_wait(sala); // Garante exclusividade na entrada da sala

    *vagas_sala -= 1;
    printf("alunoSO_%d_AT%d entrou na sala\n", aluno->id, aluno->tipo_atividade);
    printf("Vagas restantes na sala: %d\n", *vagas_sala);
}

void aluno_entregar_atividade(Aluno *aluno, sem_t* professor, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2)
{
    sem_post(professor); // Avisa o professor que tem tarefa para ser entregue
    printf("alunoSO_%d_AT%d entrega atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_sair_sala(Aluno *aluno, sem_t *sala, int *vagas_sala)
{
    *vagas_sala += 1;
    printf("alunoSO_%d_AT%d saiu da sala\n", aluno->id, aluno->tipo_atividade);

    pthread_mutex_lock(&grupo_mutex);
    if (aluno->tipo_atividade == 1)
    {
        alunos_atv1_no_grupo--;
        sem_post(&grupo_pronto);
    }
    else
    {
        alunos_atv2_no_grupo--;
        sem_post(&grupo_pronto);
    }
    pthread_mutex_unlock(&grupo_mutex);

    sem_post(sala); // Libera a sala para o próximo grupo
}

