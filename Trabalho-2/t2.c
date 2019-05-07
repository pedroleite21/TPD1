#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define nMaxProcessos 10

void main(int argc, char **argv)
{
   int my_rank; // Identificador deste processo
   int proc_n;  // Numero de processos disparados pelo usuario na linha de comando (np)
   int message; // Buffer para as mensagens
   int ID_NovaEleicao = 0;
   //int coordenador = 0;
   int opcao;

   //vetor de candidatos de 10 processos
   int candidatos[nMaxProcessos];
   int eleicao[2];
   int coordenador[2];

   MPI_Status status; // estrutura que guarda o estado de retorno

   MPI_Init(&argc, &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

   if (proc_n > 10)
   {
      printf("Número de processos inválido");
   }

   //while ()
   //{
      // preenche o anel com a posição dos processos
      for (int i = 0; i < nMaxProcessos; i++)
      {
         candidatos[i] = -1;
      }

      // receber da esquerda
      if (my_rank == 0)
      {
         // escolher processo que tera falha
         srand(time(NULL));

         while (ID_NovaEleicao == 0)
            ID_NovaEleicao = rand() % (proc_n);

         printf("Processo %d falhou \n\n", proc_n - 1);
         printf("Processo %d faz eleição \n\n", ID_NovaEleicao);

         eleicao[0] = ID_NovaEleicao;
         eleicao[1] = proc_n - 1;

         //message = ID_Falha;

         MPI_Send(&eleicao, 2, MPI_INT, my_rank + 1, 1, MPI_COMM_WORLD);
      }
      else if (my_rank != (proc_n - 1))
      {
         MPI_Recv(&eleicao, 2, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         MPI_Send(&eleicao, 2, MPI_INT, my_rank + 1, 1, MPI_COMM_WORLD);

         if (my_rank == eleicao[0])
         {
            // esperar receber do ultimo
            MPI_Recv(&eleicao, 2, MPI_INT, proc_n - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         }
      }
      else // ultimo processo
      {
         MPI_Recv(&eleicao, 2, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

         //envia pro coordenador
         MPI_Send(&eleicao, 2, MPI_INT, eleicao[0], 1, MPI_COMM_WORLD);
      }

      if (my_rank == eleicao[0])
      {
         //solicitar eleicao
         candidatos[0] = 1;

         int i = 1;
         candidatos[i] = my_rank;
         candidatos[0] = ++i;

         // printf("[ ");
         // for (i = 0; i < nMaxProcessos; i++)
         // {
         //    printf("%d, ", candidatos[i]);
         // }
         // printf("]\n");

         //envia mensagem de eleicao
         MPI_Send(&candidatos, nMaxProcessos, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);

         //printf("[%d] Esperando receber lista de candidatos de [%d].\n", my_rank, (my_rank + proc_n - 1) % proc_n);

         MPI_Recv(&candidatos, nMaxProcessos, MPI_INT, (my_rank + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

         printf("Recebido vetor de candidatos\n\n");

         // printf("[ ");
         // for (i = 0; i < candidatos[0]; i++)
         // {
         //    printf("%d, ", candidatos[i]);
         // }
         // printf("]\n");

         int melhorCandidato = 1;
         int tam = candidatos[0];

         for (i = 2; i < tam; i++)
         {
            if (candidatos[i] > melhorCandidato && candidatos[i] != 0)
               melhorCandidato = candidatos[i];
         }
         printf("Processo %d elegeu o coordenador %d \n\n ", eleicao[0], melhorCandidato);

         coordenador[0] = melhorCandidato;
         coordenador[1] = 1;

         MPI_Send(&coordenador, 2, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);
      }
      else
      {
         MPI_Recv(&candidatos, nMaxProcessos, MPI_INT, (my_rank + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

         if (my_rank != eleicao[1])
         {
            int i = candidatos[0];
            candidatos[i] = my_rank;
            candidatos[0] = ++i;
         }

         MPI_Send(&candidatos, nMaxProcessos, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);

         MPI_Recv(&coordenador, 2, MPI_INT, (my_rank + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         
         int i = coordenador[1];
         coordenador[1] = ++i;

         if (my_rank != coordenador[0] && my_rank != eleicao[1] && my_rank != 0)
            printf("Processo %d - Coordernador %d\n", my_rank, coordenador[0]);
         else if (my_rank == coordenador[0])
            printf("Processo %d - Ciente da coordenacao\n", my_rank);

         MPI_Send(&coordenador, 2, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);
      }

      if(coordenador[1] == proc_n) {
         
         printf("%d---------------------------------\n", coordenador[1]);

         // printf("Again? \t");
         // scanf("%d", opcao);

         // printf("%d\n", opcao);
      }

   //}

   MPI_Finalize();
}
