#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VETOR_SIZE 1000000
#define TAM_TROCA ((int)(0.01 * VETOR_SIZE) // 1% do vetor é pra troca
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

int verifica_processos(int *v, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
        if (v[i] == 0)
            return FALSE;

    return TRUE;
}

int main(int argc, char **argv)
{
    int my_rank;
    int i, j;
    int c = VETOR_SIZE;
    int proc_n, count;
    int pronto = FALSE;
    int tam_vetor;
    int comp;
    double t_inicial, t_final;
    MPI_Status status;


    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    tam_vetor = VETOR_SIZE / proc_n;

    int vparte[tam_vetor + TAM_TROCA];
    
    for (i = 0; i < tam_vetor; i++)
        vparte[i] = VETOR_SIZE - tam_vetor * my_rank - i;

    int proc_pronto[proc_n];
    for (i = 0; i < proc_n; i++)
        proc_pronto[i] = 0;
    proc_pronto[0] = 1;

    pronto = FALSE;

    while (PRONTO == FALSE)
    {
        bs(tam_vetor, vparte);

        if (my_rank == proc_n - 1)
        {
            MPI_Send(&vparte[tam_vetor - 1], 1, MPI_INT, my_rank + 1, TRABALHO, MPI_COMM_WORLD);
        }

        if (my_rank != 0)
        {
            MPI_Recv(&comp, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, TRABALHO, &status);

            if (&vparte[tam_vetor - 1] > comp)
            {
                // ok vetor maior
                proc_pronto[my_rank] = 1;
            }
        }

        for (i = 0; i < proc_n : i++)
        {
            MPI_Bcast(&proc_pronto[i], 1, MPI_INT, i, MPI_COMM_WORLD);
        }

        pronto = verifica_processos(proc_pronto, proc_n);
        if(pronto) break;
    
        // TROCO VALORES PARA CONVERGIR
        if (my_rank != 0)
        {
            MPI_Send(&vparte[0], TAM_TROCA, MPI_INT, my_rank - 1, TRABALHO, MPI_COMM_WORLD);
        }
        
        if (my_rank != proc_n - 1)
        {
            MPI_Recv(&vparte[tam_vetor], TAM_TROCA, MPI_INT, my_rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            bs(2*TAM_TROCA, vparte[tam_vetor - TAM_TROCA]);

            MPI_Send(&vparte[tam_vetor], TAM_TROCA, MPI_INT, my_rank + 1, TRABALHO, MPI_COMM_WORLD);
        }

        if(my_rank != 0)
        {
            MPI_Recv(&vparte[0], TAM_TROCA, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }

    printf("PROCESSO: %d, first: %d, last: %d\n", my_rank, vparte[0], vparte[tam_vetor-1]);

    MPI_Finalize();

    return 0;
}
