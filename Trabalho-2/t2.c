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
   int ID_Falha = 0;
   int coordenador = 0;

   //vetor de candidatos de 10 processos
   int candidatos[nMaxProcessos];

   MPI_Status status; // estrutura que guarda o estado de retorno

   MPI_Init(&argc, &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

   if (proc_n > 10)
   {
      printf("Número de processos inválido");
   }

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

      while (ID_Falha == 0)
         ID_Falha = rand() % (proc_n);

      printf("Processo %d é o coordenador\n", ID_Falha);
      //printf("Processo %d está com falha \n", ID_Falha);
      message = ID_Falha;

      MPI_Send(&message, 1, MPI_INT, my_rank + 1, 1, MPI_COMM_WORLD);
   }
   else if (my_rank != (proc_n - 1))
   {
      MPI_Recv(&message, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(&message, 1, MPI_INT, my_rank + 1, 1, MPI_COMM_WORLD);

      if (my_rank == message)
      {
         MPI_Recv(&message, 1, MPI_INT, proc_n - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      }
   }
   else
   {
      MPI_Recv(&message, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      MPI_Send(&message, 1, MPI_INT, message, 1, MPI_COMM_WORLD);
   }

   if (my_rank == message)
   {
      printf("Processo %d solicita eleicao devido a falha\n", message);

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

      printf("Recebido vetor de candidatos\n");

      // printf("[ ");
      // for (i = 0; i < nMaxProcessos; i++)
      // {
      //    printf("%d, ", candidatos[i]);
      // }
      // printf("]\n");

      printf("Eleicao de um novo coordenador\n");

      int melhorCandidato = 1;
      int tam = candidatos[0];

      for (i = 2; i < tam; i++)
      {
         if (candidatos[i] > melhorCandidato && candidatos[i] != 0)
            melhorCandidato = candidatos[i];
      }
      printf("Coordenador eleito %d \n\n ", melhorCandidato);

      MPI_Send(&melhorCandidato, 1, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);
   }
   else
   {
      MPI_Recv(&candidatos, nMaxProcessos, MPI_INT, (my_rank + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (my_rank != (proc_n - 1))
      {
         int i = candidatos[0];
         candidatos[i] = my_rank;
         candidatos[0] = ++i;
      }
      else
      {
         printf("[%d] não se cadastrou!\n", my_rank);
      }

      MPI_Send(&candidatos, nMaxProcessos, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);

      MPI_Recv(&coordenador, 1, MPI_INT, (my_rank + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if(my_rank != coordenador) printf("Processo %d - Coordernador %d\n", my_rank, coordenador);
      MPI_Send(&coordenador, 1, MPI_INT, ((my_rank + 1) % proc_n), 1, MPI_COMM_WORLD);
   }
   // else
   // {
   //    MPI_Recv(&message, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda

   //    printf("Pid: %d, mensagem %d\n", my_rank, message); // mostro mensagem na tela
   //    message += 1;                                       // incremento um na mensagem recebida

   //    if (my_rank == proc_n - 1)
   //    {                                               // sou o utlimo?
   //       printf("Pid: %d, sou o ultimo!\n", my_rank); // mostro mensagem na tela pois sou o ultimo
   //       MPI_Send(&message, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
   //    }
   //    else
   //       MPI_Send(&message, 1, MPI_INT, my_rank + 1, 1, MPI_COMM_WORLD); // envio para a direita

   //    if (my_rank == 1)
   //    {
   //       MPI_Recv(&message, 1, MPI_INT, proc_n - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda

   //       printf("Pid: %d, mensagem %d\n", my_rank, message); // mostro mensagem na tela
   //       message += 1;
   //    }
   // }

   // processo mensagem

   // enviar para a direita

   MPI_Finalize();
}
