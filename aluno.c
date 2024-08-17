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

void aluno_terminar_atividade(Aluno *aluno) {
    aluno->concluiu_atividade = true;
    printf("alunoSO_%d_AT%d terminou atividade %d", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_aguardar_entrega(Aluno *aluno)
{
    printf("alunoSO_%d_AT%d chega para entrega", aluno->id, aluno->tipo_atividade);
}

void aluno_entrar_sala(Aluno *aluno) {
    printf("alunoSO_%d_AT%d entrou na sala", aluno->id, aluno->tipo_atividade);
}

void aluno_entregar_atividade(Aluno *aluno) {
    printf("alunoSO_%d_AT%d entrega atividade %d", aluno->id, aluno->tipo_atividade, aluno->tipo_atividade);
}

void aluno_sair_sala(Aluno *aluno) {
    printf("alunoSO_%d_AT%d saiu da sala", aluno->id, aluno->tipo_atividade);
}
