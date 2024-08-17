/**
 * Program: aluno.c
 * Autores: André Felipe Mireski, Alexandre Tolomeotti, Victor
 * Data: 17/08/2024
 * Data de entrega: 22/08/2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "aluno.h"

struct aluno {
    unsigned int id;
    unsigned int tipo_atividade;
};

void fazerAtividade();

void aguardarEntrega();

void entrarSala();

void entregarAtividade();

void sairSala();
