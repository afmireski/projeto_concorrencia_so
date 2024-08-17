/**
 * Header: professor.h
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _PROFESSOR_H_
#define _PROFESSOR_H_

void professor_receber_atividade(sem_t *professor, int *n_entregas);

void professor_finalizar_entrega_atividades(sem_t *sala);

#endif