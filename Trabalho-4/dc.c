#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VETOR_SIZE 40
#define TAM_SACO 1000

#define TRABALHO 1

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

int *interleaving(int vetor[], int tam)
{
    int *vetor_auxiliar;
    int i1, i2, i_aux;

    vetor_auxiliar = (int *)malloc(sizeof(int) * tam);

    i1 = 0;
    i2 = tam / 2;

    for (i_aux = 0; i_aux < tam; i_aux++)
    {
        if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2))) || (i2 == tam))
            vetor_auxiliar[i_aux] = vetor[i1++];
        else
            vetor_auxiliar[i_aux] = vetor[i2++];
    }

    return vetor_auxiliar;
}

int main(int argc, char **argv)
{
    int my_rank;
    int i, j;
    int l = TAM_SACO, c = VETOR_SIZE;
    int proc_n, count;
    int filhoe, filhod, pai;
    int delta;
    double t_inicial, t_final;
    MPI_Status status;

    c = atoi(argv[1]);
    delta = atoi(argv[2]);
    int(*message) = malloc(c * sizeof(int));

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    if (my_rank == 0) //  RAIZ
    {
        for (j = 0; j < c; j++)
            message[j] = c - j; 
        count = c;
        
        t_inicial =  MPI_Wtime();
    }
    else
    {
        // recebe
        MPI_Recv(message, c, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        MPI_Get_count(&status, MPI_INT, &count);

        pai = status.MPI_SOURCE;

        printf("[%d]: Recebi vetor do pai [%d] - tam: %d\n", my_rank, pai, count);
    }

    //dividir ou conquistar?
    if (count <= delta)
    { //conquisto
        bs(count, message);
        printf("[%d]: Ordenando vetor\n", my_rank);
    }
    else
    { //divido
        printf("[%d]: Dividindo...\n", my_rank);
        //filho esquerda
        filhoe = my_rank * 2 + 1;
        MPI_Send(&message[0], count / 2, MPI_INT, filhoe, TRABALHO, MPI_COMM_WORLD);
        printf("[%d]: Mandando pra filho esquerdo [%d]...\n", my_rank, filhoe);
        //filho direita
        filhod = my_rank * 2 + 2;
        MPI_Send(&message[count / 2], count / 2, MPI_INT, filhod, TRABALHO, MPI_COMM_WORLD);
        printf("[%d]: Mandando pra filho direito [%d]...\n", my_rank, filhod);

        MPI_Recv(&message[0], count, MPI_INT, filhoe, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&message[count/2], count, MPI_INT, filhod, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        printf("[%d]: intercalando vetor\n", my_rank);
        message = interleaving(message, count);
    }

    if (my_rank != 0)
    {
        printf("[%d]: Enviando vetor para %d - tam: %d\n", my_rank, pai, count);
        MPI_Send(message, count, MPI_INT, pai, TRABALHO, MPI_COMM_WORLD);
    }
    else
    {
        t_final = MPI_Wtime();
        printf("[%d]: Tempo decorrido %.6f\n", my_rank, (t_final - t_inicial));
        printf("[%d]: Vetor ordenado: %d - %d\n", my_rank, message[0], message[c-1]);
        printf("\n");
    }

    MPI_Finalize();
    free(message);
    return 0;
}
