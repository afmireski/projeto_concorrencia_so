/**
 * Header: professor.h
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor Ângelo
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _PROFESSOR_H_
#define _PROFESSOR_H_

/**
 * Professor: Função que simula o recebimento de uma atividade pelo professor.
 * @param professor: semáforo responsável por notificar o professor que tem tarefa para receber
 * @param n_entregas: variável que controla quantas atividades foram entregues
 */
void professor_receber_atividade(sem_t *professor, int *n_entregas);

/**
 * Professor: Função que simula o professor finalizando a entrega das atividades
*/
void professor_finalizar_entrega_atividades();

#endif