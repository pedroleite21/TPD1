#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VETOR_SIZE 1000000

#define TRABALHO 1

#define TRUE 1
#define FALSE 0

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
    int c = VETOR_SIZE;
    int proc_n, count;
    int pronto = FALSE;
    int parte;
    double t_inicial, t_final;
    MPI_Status status;

    //c = atoi(argv[1]);
    //delta = atoi(argv[2]);
    int(*message) = malloc(c * sizeof(int));
    

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    parte = VETOR_SIZE / proc_n;

    int(*vparte) = malloc(parte * sizeof(int));
    int(*vparteaux) = malloc(parte * sizeof(int));

    //inicializa o vetor
    for (i = 0; i < parte; i++)
        vparte[i] = parte - i;

    pronto = FALSE;

    while (PRONTO == FALSE)
    {
        bs(c, vparte);

        if(my_rank == proc_n - 1)
        {
            MPI_Send(vparte, parte, MPI_INT, my_rank + 1, TRABALHO, MPI_COMM_WORLD);
        } 

        if(my_rank != 0) 
        {
            MPI_Recv(&vparteaux, parte, MPI_INT, my_rank - 1, MPI_ANY_TAG, TRABALHO, &status);

            if(vparte[0] > vparteaux[parte-1]) {
                // ok vetor maior
                MPI_Bcast(vparte, parte, MPI_INT, my_rank, MPI_COMM_WORLD);
            }
        
        }

        // TROCO VALORES PARA CONVERGIR
        if(my_rank != 0)
        {
            MPI_Send(&vparte[0], parte /2 , MPI_INT, my_rank + 1, TRABALHO, MPI_COMM_WORLD);
        } else if (my_rank != proc_n -1) 
        {
            MPI_Recv(&vparteaux, parte / 2, MPI_INT, my_rank - 1, MPI_ANY_TAG, TRABALHO, &status);

            
        }


    }

    MPI_Finalize();
    free(vparte);
    free(vparteaux);
    free(message);
    return 0;
}
