#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VETOR_SIZE 40
#define TAM_SACO 1000

#define TRABALHO 1

void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
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
    int filhoe, filhod;
    int delta;
    MPI_Status status;

    c = atoi(argv[1]);
    delta = atoi(argv[2]);

    int(*saco)[TAM_SACO] = malloc(TAM_SACO * sizeof *saco);
    int(*message) = malloc(c * sizeof(int));

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    if (my_rank == 0) //  RAIZ
    {
        // inicializar o saco
        //for (i = 0; i < l; i++)
            for (j = 0; j < c; j++)
                message[j] = c - j;
        //message = saco[0];
    }
    else
    {
        // recebe
        MPI_Recv(message, c, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        MPI_Get_count(&status, MPI_INT, &count);

        printf("[%d] - %d\n", my_rank, count);
    }

    //dividir ou conquistar?
    if (c <= delta)
    {   //conquisto
        bs(c, message);
    }
    else
    {   //divido
        //filho esquerda
        filhoe = my_rank * 2 + 1;
        MPI_Send(&message[0], c/2, MPI_INT, filhoe, TRABALHO, MPI_COMM_WORLD);

        //filho direita
        filhod = my_rank * 2 + 2;
        MPI_Send(&message[c/2], c/2, MPI_INT, filhod, TRABALHO, MPI_COMM_WORLD);
    
        MPI_Recv(&message[0], c, MPI_INT, filhoe, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //MPI_Recv(&message[0], c, MPI_INT, filhoe, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
   
    }

    MPI_Finalize();
    free(saco);
    free(message);

    return 0;
}