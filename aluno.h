/**
 * Header: aluno.h
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#ifndef _ALUNO_H_
#define _ALUNO_H_
#define DEBUG false

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h> /* Semaphore */

typedef struct aluno Aluno;

Aluno *cria_aluno(int id, int tipo_atividade);

void aluno_fazer_atividade(Aluno *aluno);

void aluno_terminar_atividade(Aluno *aluno);

void aluno_aguardar_entrega(Aluno *aluno, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2);

void aluno_entrar_sala(Aluno *aluno, sem_t *entrar_grupo, sem_t* grupo_formado, int *vagas_grupo);

void aluno_entregar_atividade(Aluno *aluno, sem_t* professor);

void aluno_sair_sala(Aluno *aluno, sem_t *sair_grupo, sem_t *entrar_grupo, sem_t* grupo_formado, int *vagas_grupo, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2, sem_t* grupo_desformado);

#endif