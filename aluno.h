/**
 * Header: aluno.h
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h> /* Semaphore */

typedef struct aluno Aluno;

Aluno *cria_aluno(int id, int tipo_atividade);

void aluno_fazer_atividade(Aluno *aluno);

void aluno_terminar_atividade(Aluno *aluno);

void aluno_aguardar_entrega(Aluno *aluno);

void aluno_entrar_sala(Aluno *aluno);

void aluno_entregar_atividade(Aluno *aluno);

void aluno_sair_sala(Aluno *aluno);

#endif
