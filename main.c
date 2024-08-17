/**
 * Program: main.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
 */

#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <stdbool.h> // bool
#include <pthread.h>

#include "aluno.h"
#include "professor.h"

#define DEBUG false

Aluno **monta_alunos(int n_alunos);

/**
 * Destrói os chunks criados
 *
 * @param chunks: chunks a serem destruídos
 */
void destroi_chunks(Aluno **chunks);

void cria_threads(pthread_t *threads, Aluno **alunos, int n_alunos);

void *acao_aluno(void *param);

void *acao_professor(void *param);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Quantidade de alunos não informada\n");
        return 1;
    }

    int n_alunos = atoi(argv[1]);

    Aluno **alunos = monta_alunos(n_alunos);

    /* Declara as threads */
    int n_threads = n_alunos + 1; // +1 para o professor

    pthread_t threads[n_threads];
    cria_threads(threads, alunos, n_alunos);

    /* Aguarda as threads terminarem */
    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
#if DEBUG
        printf("Thread %d terminou\n", i);
#endif
    }

    return 0;
}

void *acao_aluno(void *param)
{
    Aluno *aluno = (Aluno *)param;

    aluno_fazer_atividade(aluno);

    aluno_terminar_atividade(aluno);

    aluno_aguardar_entrega(aluno);

    aluno_entrar_sala(aluno);

    aluno_entregar_atividade(aluno);

    aluno_sair_sala(aluno);

    return NULL;
}

void *acao_professor(void *param) {

    

    return NULL;
}

Aluno **monta_alunos(int n_alunos)
{
    Aluno **chunks = (Aluno **)calloc(n_alunos, sizeof(Aluno *));

    int n_atividade01 = n_alunos / 3;             // 1/3 Atividade 01
    int n_atividade02 = n_alunos - n_atividade01; // 2/3 Atividade 02

    printf("Quantidade de alunos: %d\n", n_alunos);
    printf("Quantidade de atividade 01: %d\n", n_atividade01);
    printf("Quantidade de atividade 02: %d\n", n_atividade02);

    int atividades = 1;
    for (int i = 0; i < n_alunos; i++, atividades++)
    {
        Aluno *aluno = cria_aluno(i, atividades <= n_atividade01 ? 1 : 2);
    }
}

void cria_threads(pthread_t *threads, Aluno **alunos, int n_alunos)
{
    int i;
    for (i = 0; i < n_alunos; i++)
    {
        pthread_create(&threads[i], NULL, (void *)acao_aluno, alunos[i]);
#if DEBUG
        printf("Criou thread aluno %d\n", i);
#endif
    }

    pthread_create(&threads[i], NULL, (void *)professor_faz_algo);
#if DEBUG
    printf("Criou thread professor%d\n", i);
#endif
}
