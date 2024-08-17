/**
 * Program: aluno.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "aluno.h"

struct aluno
{
    int id;
    int tipo_atividade;
    bool concluiu_atividade;
};

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
    printf("alunoSO_%d_AT%d fazendo atividade %d", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_terminar_atividade(Aluno *aluno)
{
    aluno->concluiu_atividade = true;
    printf("alunoSO_%d_AT%d terminou atividade %d", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_aguardar_entrega(Aluno *aluno, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2, int *vagas_sala)
{
    printf("alunoSO_%d_AT%d chega para entrega", aluno->id, aluno->tipo_atividade);
    if (aluno->tipo_atividade == 1) // Verifica qual atividade vai ser entregue
    {
        sem_wait(vagas_aluno_atv1); // Espera a ter vagas no grupo para quer quer entregar a atividade 01
    }
    else
    {
        sem_wait(vagas_aluno_atv2); // Espera a ter vagas no grupo para quer quer entregar a atividade 02
    }
}

void aluno_entrar_sala(Aluno *aluno, sem_t *sala, int *vagas_sala)
{
    if (*vagas_sala == 0)
        sem_wait(sala); // Espera liberar a sala

    *vagas_sala -= 1;

    printf("alunoSO_%d_AT%d entrou na sala", aluno->id, aluno->tipo_atividade);
}

void aluno_entregar_atividade(Aluno *aluno, sem_t* professor, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2)
{
    sem_wait(professor); // Aguarda o professor estar com as mãos livres para receber a tarefa;
    printf("alunoSO_%d_AT%d entrega atividade %d", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);

    if (aluno->tipo_atividade == 1)
    {
        sem_post(vagas_aluno_atv1); // Libera vaga no grupo da atividade 01
    }
    else
    {
        sem_post(vagas_aluno_atv2); // Libera vaga no grupo da  atividade 02
    }
}

void aluno_sair_sala(Aluno *aluno, sem_t *sala, int *vagas_sala)
{
    *vagas_sala += 1;

    if (vagas_sala < 3)
        sem_wait(sala); // Espera os outros entregarem;

    sem_post(vagas_sala); // Libera a sala

    printf("alunoSO_%d_AT%d saiu da sala", aluno->id, aluno->tipo_atividade);
}
