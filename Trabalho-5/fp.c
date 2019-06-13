#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VETOR_SIZE 40
#define TAM_SACO 1000

#define TRABALHO 1

#define TRUE 1
#define FALSE 1

void bs(int n, int *vetor)
{
    int c = 0, d, troca, trocou = 1;

    while (c < (n - 1) & trocou)
    {
        trocou = 0;
        for (d = 0; d < n - c - 1; d++)
            if (vetor[d] > vetor[d + 1])
            {
                troca = vetor[d];
                vetor[d] = vetor[d + 1];
                vetor[d + 1] = troca;
                trocou = 1;
            }
        c++;
    }
}

int main(int argc, char **argv)
{
    int my_rank;
    int i, j;
    int l = TAM_SACO, c = VETOR_SIZE;
    int proc_n, count;
    int filhoe, filhod, pai;
    int pronto = FALSE;
    double t_inicial, t_final;
    MPI_Status status;

    c = atoi(argv[1]);
    delta = atoi(argv[2]);
    int(*message) = malloc(c * sizeof(int));

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    pronto = FALSE;

    while(PRONTO == FALSE) 
    {

    }



    MPI_Finalize();
    free(message);
    return 0;
}
