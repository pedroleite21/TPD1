
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define M 1000
#define N 1000000

#define TRABALHO 1
#define KILL 2

#define TRUE 1
#define FALSE 0

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

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

int control_empty(int tam, int *control)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        if (control[i] != 0)
            return FALSE;
    }
    return TRUE;
}

int main(int argc, char **argv)
{
    double t_inicial, t_final; // marcador de tempo
    int i, j;
    int l = M, c = N;
    int my_rank;       // Identificador deste processo
    int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)
    MPI_Status status; // estrutura que guarda o estado de retorno

    int(*saco)[c] = malloc(l * sizeof *saco);
    int(*message) = malloc(c * sizeof(int));
    int *proc_control;

    //for (i = 0; i < l; i++)
        for (j = 0; j < c; j++)
            message[j] = c - j;
            //saco[i][j] = c - j;

    MPI_Init(&argc, &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

    proc_control = calloc(proc_n - 1, sizeof *proc_control);

    int task, position;
    int received = M;

        if (my_rank == 0)
    {
        // MESTRE
        t_inicial = MPI_Wtime();

        //ENVIA TAREFAS
        for (i = 1; i < proc_n; i++)
        {
            proc_control[i - 1] = i;
            task = i;
            MPI_Send(saco[i - 1], c, MPI_INT, i, TRABALHO, MPI_COMM_WORLD);
            printf("[Mestre] : Enviando tarefa %d para [%d]\n", task, i);
        }

        while (received > 0) // enquanto todas as tarefas forem recebidos
        {
            // RECEBE DE QUEM FICOU PRONTO PRIMEIRO
            MPI_Probe(MPI_ANY_SOURCE, TRABALHO, MPI_COMM_WORLD, &status);
            position = proc_control[status.MPI_SOURCE - 1];

            MPI_Recv(saco[position], c, MPI_INT, MPI_ANY_SOURCE, TRABALHO, MPI_COMM_WORLD, &status);
            printf("[Mestre] : Recebi tarefa completa de [%d]\n", status.MPI_SOURCE);
            proc_control[status.MPI_SOURCE - 1] = 0;
            received--;

            if(task < M) { // se tiverem tarefas remanescentes envio
                proc_control[status.MPI_SOURCE - 1] = task;
                MPI_Send(saco[task], c, MPI_INT, status.MPI_SOURCE, TRABALHO, MPI_COMM_WORLD);

                printf("[Mestre] : Enviando nova tarefa %d para [%d]\n", task, status.MPI_SOURCE);

                task++;
            } else { // se nao tiverem tarefas remanescentes dou um kill
                MPI_Send(saco[0], c, MPI_INT, status.MPI_SOURCE, KILL, MPI_COMM_WORLD);
            }
        }

        t_final = MPI_Wtime();
        printf("[Mestre] : Tempo decorrido %.4f\n", t_final - t_inicial);
    }
    else
    {
        // ESCRAVO
        while (1)
        {
            MPI_Recv(message, c, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == TRABALHO)
            {
                printf("[%d] : Recebendo tarefa\n", my_rank);
                qsort(message, c, sizeof(int), cmpfunc); // quicksort
                //bs(c, message);
                MPI_Send(message, c, MPI_INT, 0, TRABALHO, MPI_COMM_WORLD);
            }
            else if (status.MPI_TAG == KILL)
            {
                printf("[%d] : KILL \n", my_rank);
                break;
            }
        }
    }

    MPI_Finalize();
    free(saco);
    free(message);
    return 0;
}