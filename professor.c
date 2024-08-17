/**
 * Program: professor.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h> /* Semaphore */
#include "professor.h"

void professor_receber_atividade(sem_t *professor, int *n_entregas)
{
    sem_wait(professor); // Aguarda um aluno chegar para entregar uma tarefa

    printf("Professor recebeu atividade");

    *n_entregas += 1;

    // Volta a esperar por uma entrega
}

void professor_finalizar_entrega_atividades(sem_t *sala)
{
    printf("Professor recebeu todas as atividades");

    sem_wait(sala); // Aguarda todos os alunos saírem da sala antes de fechá-la
}
