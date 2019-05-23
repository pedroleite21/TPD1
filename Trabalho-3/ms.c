
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define M 1000
#define N 100000

int main(int argc, char **argv)
{
    int i, j;
    int l = 1000, c = N;
    int my_rank; // Identificador deste processo
    int proc_n;  // Numero de processos disparados pelo usuario na linha de comando (np)
    //int message; // Buffer para as mensagens
    //int saco[10][N];      // saco de trabalho
    MPI_Status status; // estrutura que guarda o estado de retorno

    // inicializo o saco de trabalho
    int(*saco)[N] = malloc(M * sizeof *saco);

    for (i = 0; i < l; i++)
        for (j = 0; j < c; j++)
            saco[i][j] = i*j;

    MPI_Init(&argc, &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

    // if (my_rank == 0)
    // {
    //     // MESTRE
    //     printf("\nMestre[%d]: ", my_rank);
    //     for (i = 0; i < 15; printf("%d ", saco[i++]))
    //         ;

    //     // mando o trabalho para os escravos fazerem

    //     for (i = 1; i < proc_n; i++)
    //     {
    //         message = saco[i - 1];
    //         MPI_Send(&message, 1, MPI_INT, i, 1, MPI_COMM_WORLD); // envio trabalho saco[i-1] para escravo com id = i
    //     }

    //     // recebo o resultado

    //     for (i = 1; i < proc_n; i++)
    //     {
    //         // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

    //     // executo opera��o

    //     printf("\nEscravo[%d]: recebi %d", my_rank, message);
    //     message = my_rank; // devolvo meu pid para o mestre

    //     // retorno resultado para o mestre

    //     MPI_Send(&message, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    // }

    // MPI_Finalize();
    free(saco);
    return 0;
}