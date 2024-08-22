/**
 * Header: aluno.h
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor Ângelo
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

/**
 * Aluno: Cria um novo Aluno
 * @param id: identificador do Aluno
 * @param tipo_atividade: tipo da atividade que o aluno fez
 */
Aluno *cria_aluno(int id, int tipo_atividade);

/**
 * Aluno: Função que simula o comportamento de um aluno fazendo uma atividade
 * @param aluno: estrutura Aluno
 */
void aluno_fazer_atividade(Aluno *aluno);

/**
 * Aluno: Função que simula o comportamento de um aluno terminando uma atividade
 * @param aluno: estrutura Aluno
 */
void aluno_terminar_atividade(Aluno *aluno);


/**
 * Aluno: Simula a chegada dos alunos na porta da sala e os direciona para a fila de atividade correta
 * @param aluno: estrutura Aluno
 * @param vagas_aluno_atv1: semáforo que controla quantidade de alunos que quer entregar a atividade 1
 * @param vagas_aluno_atv2: semáforo que controla quantidade de alunos que quer entregar a atividade 2
 */
void aluno_aguardar_entrega(Aluno *aluno, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2);

/**
 * Aluno: Função que simula a montagem do grupo que irá entrar na sala. O grupo só entra na sala estando todos os membros juntos.
 * @param aluno: estrutura Aluno
 * @param entrar_grupo: semáforo que controla se um aluno pode entrar no grupo
 * @param grupo_formado: semáforo que controla se o grupo foi formado
 * @param vagas_grupo: quantidade de alunos que o grupo possuí
 */
void aluno_entrar_sala(Aluno *aluno, sem_t *entrar_grupo, sem_t* grupo_formado, int *vagas_grupo);

/**
 * Aluno: Função que simula o comportamento de um aluno entregando a tarefa para o professor.
 * @param aluno: estrutura Aluno
 * @param professor: semáforo responsável por avisar o professor que ele tem uma tarefa para receber
 */
void aluno_entregar_atividade(Aluno *aluno, sem_t* professor);

/**
 * Aluno: Função que simula o comportamento de um aluno saindo da sala. O grupo deve sair da sala ao mesmo tempo.
 * @param aluno: estrutura Aluno
 * @param sair_grupo: semáforo que controla se um aluno pode sair do grupo
 * @param entrar_grupo: semáforo que irá liberar novos alunos para formar grupo quanto o atual for desfeito
 * @param grupo_formado: semáforo que libera a formação de um novo grupo de alunos
 * @param vagas_grupo: quantidade de alunos que o grupo possuí
 * @param vagas_aluno_atv1: semáforo que controla fila de alunos que quer entregar a atividade 1
 * @param vagas_aluno_atv2: semáforo que controla fila de alunos que quer entregar a atividade 2
 * @param grupo_desformado: semáforo que avisa quando os três membros do grupo entregaram e o grupo ser desfeito
 */
void aluno_sair_sala(Aluno *aluno, sem_t *sair_grupo, sem_t *entrar_grupo, sem_t* grupo_formado, int *vagas_grupo, sem_t *vagas_aluno_atv1, sem_t *vagas_aluno_atv2, sem_t* grupo_desformado);

#endif