/**
 * Program: aluno.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor Ângelo
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
    if (aluno->tipo_atividade == 1) // Verifica qual atividade vai ser entregue
    {
        sem_wait(vagas_aluno_atv1); // Espera a ter vagas no grupo para quer quer entregar a atividade 01
    }
    else
    {
        sem_wait(vagas_aluno_atv2); // Espera a ter vagas no grupo para quer quer entregar a atividade 02
    }
}

void aluno_entrar_sala(Aluno *aluno, sem_t *entrar_grupo, sem_t *grupo_formado, int *vagas_grupo)
{
    sem_wait(entrar_grupo); // Aguarda sua vez de entrar no grupo
    *vagas_grupo -= 1;
    if (*vagas_grupo == 0)
    {
        sem_post(grupo_formado); // Avisa o professor que tem grupo para entrar na sala
        sem_post(grupo_formado); // Avisa o professor que tem grupo para entrar na sala
        sem_post(grupo_formado); // Avisa o professor que tem grupo para entrar na sala
#if DEBUG
        printf("Grupo formado\n");
#endif
    }

#if DEBUG
    printf("Vagas restantes na sala: %d\n", *vagas_grupo);
#endif

    sem_post(entrar_grupo); // Deixa o próximo entrar no grupo

    sem_wait(grupo_formado); // Espera o grupo estar formado para entrar na sala
    printf("alunoSO_%d_AT%d entrou na sala\n", aluno->id, aluno->tipo_atividade);
#if DEBUG
    printf("Grupo entrando...\n");
#endif
}

void aluno_entregar_atividade(Aluno *aluno, sem_t *professor)
{
    sem_post(professor); // Avisa o professor que tem atividade para receber
#if DEBUG
    printf("Professor recebendo...\n");
#endif
    printf("alunoSO_%d_AT%d entrega atividade %d\n", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_sair_sala(Aluno *aluno, sem_t *sair_grupo, sem_t *entrar_grupo, sem_t *grupo_formado, int *vagas_grupo, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2, sem_t *grupo_desformado)
{
    sem_wait(sair_grupo); // Aguarda sua vez de entrar no grupo

    printf("alunoSO_%d_AT%d saiu da sala\n", aluno->id, aluno->tipo_atividade);
    *vagas_grupo += 1;
    if (*vagas_grupo == 3)
    {
        sem_post(grupo_desformado); // Desforma o grupo
        sem_post(grupo_desformado);
        sem_post(grupo_desformado);
        sem_post(grupo_formado); // Libera para próximo aluno formar grupo
        sem_post(entrar_grupo);  // Libera para o próximo na fila entrar num novo grupo
    }

#if DEBUG
    printf("Alunos que saíram da sala: %d\n", *vagas_grupo);
#endif

    sem_post(sair_grupo); // Deixa o próximo entrar no grupo

    sem_wait(grupo_desformado);

    if (aluno->tipo_atividade == 1)
    {
        sem_post(vagas_aluno_atv1); // Libera vaga no grupo da atividade 01
    }
    else
    {
        sem_post(vagas_aluno_atv2); // Libera vaga no grupo da  atividade 02
    }
}
