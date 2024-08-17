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

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Quantidade de alunos não informada\n");
        return 1;
    }


    int n_alunos = atoi(argv[1]);

    int n_atividade01 = n_alunos/3; // 1/3 Atividade 01
    int n_atividade02 = n_alunos - n_atividade01; // 1/3 Atividade 02

    printf("Quantidade de alunos: %d\n", n_alunos);
    printf("Quantidade de atividade 01: %d\n", n_atividade01);
    printf("Quantidade de atividade 02: %d\n", n_atividade02);

    return 0;
}
