#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


#define true 1
#define false 0

#define nMaxProcessos 10

#define COMECAELEICAO 1
#define ELEICAO 2
#define DESATIVA 3

void main(int argc, char **argv)
{
   int my_rank; // Identificador deste processo
   int proc_n;  // Numero de processos disparados pelo usuario na linha de comando (np)
   int ID_NovaEleicao = 0;
   //int coordenador = 0;
   //int opcao;

   //vetor de candidatos de 10 processos
   int candidatos[nMaxProcessos];
   //int eleicao[2];
   //int coordenador[2];

   MPI_Status status; // estrutura que guarda o estado de retorno

   MPI_Init(&argc, &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

   int message[proc_n];
   int coordenador;
   int eleicao = false;
   int apto = true;

   if (proc_n > 10)
   {
      printf("Número de processos inválido");
   }
   
   
   if(my_rank == 0) {
      //p0
   
      while(1) {
         int opcao;
         printf("Deseja realizar uma nova eleição? 1 - Sim\n");
         fflush(0);
         scanf("%d", &opcao);

         if(opcao == 1) 
         {
            //escolhe qual o processo vai ser realizado a eleicao
            srand(time(NULL));

            while (ID_NovaEleicao == 0 || ID_NovaEleicao == proc_n - 1)
               ID_NovaEleicao = rand() % (proc_n);

            printf("Sorteio: Processo [%d] faz eleição \n\n", ID_NovaEleicao);

            //desativar o de maior prioridade
            MPI_Send(&message, proc_n, MPI_INT, proc_n-1, 3, MPI_COMM_WORLD);

            MPI_Recv(&message, proc_n, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);

            printf("Falha: Processo [%d] falha \n\n", message[0]);

            message[0] = ID_NovaEleicao;
            message[1] = proc_n-1;
    
            MPI_Send(&message, proc_n, MPI_INT, ID_NovaEleicao, 1, MPI_COMM_WORLD);

            MPI_Recv(&message, proc_n, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);

            printf("De volta ao P0 com Coordenador [%d] Eleito\n\n", message[0]);

            printf("----------------------------------\n\n");
         
         } else {
            MPI_Send(&message, proc_n, MPI_INT, 1, 5, MPI_COMM_WORLD);
            break;
         }
      }
   } else {
      // anel

      while(1)
      {
         MPI_Recv(&message, proc_n, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

         if(status.MPI_TAG == 1) 
         {
            coordenador = message[0];
            printf("Processo [%d] comecando eleicao\n", my_rank);
            eleicao = true;

            for(int i=0; i < proc_n; i++) 
            {
                message[i] = -1;
            }
         
            if(apto) message[my_rank - 1] = my_rank;

            //enviar 
            if(my_rank == proc_n - 1) {
               MPI_Send(&message, proc_n, MPI_INT, 1, 2, MPI_COMM_WORLD);
            } else {
               MPI_Send(&message, proc_n, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
            }            
      
         } else if(status.MPI_TAG == 2) {
            if(eleicao == true) {
               int eleito = message[my_rank - 1];

               printf("Começando a eleicao no Processo [%d]\n", eleito);
               int x;
               for(x=0; x< proc_n; x++) {
                  if(message[x] > eleito) {
                     eleito = message[x];
                  }
               }

               printf("Processo [%d] eleito!\n\n", eleito);

               message[0]= eleito;

               //passa a informacao do eleito para os outros

               if(my_rank == proc_n - 1) {
                  MPI_Send(&message, proc_n, MPI_INT, 1, 4, MPI_COMM_WORLD);
               } else {
                  MPI_Send(&message, proc_n, MPI_INT, my_rank+1, 4, MPI_COMM_WORLD);
               }
         
            } else {
               if(apto == true) {
                  message[my_rank - 1] = my_rank;      
               }

               if(my_rank == proc_n - 1) {
                  MPI_Send(&message, proc_n, MPI_INT, 1, 2, MPI_COMM_WORLD);
               } else {
                  MPI_Send(&message, proc_n, MPI_INT, my_rank+1, 2, MPI_COMM_WORLD);
               }

            }

         } else if(status.MPI_TAG == 3) {
            if(apto == true) {
               message[0] = my_rank;

               MPI_Send(&message, proc_n, MPI_INT, 0, 3, MPI_COMM_WORLD);
               apto = false;
            } else {
               MPI_Send(&message, proc_n, MPI_INT, my_rank-1, 3, MPI_COMM_WORLD);
            }
         } else if(status.MPI_TAG == 4) {
            
            if(eleicao == true) {
               MPI_Send(&message, proc_n, MPI_INT, 0, 4, MPI_COMM_WORLD);
            } else {
               printf("Processo [%d] reconhece novo Coordenador [%d]\n", my_rank, message[0]);
               if(my_rank == proc_n - 1) {
                  MPI_Send(&message, proc_n, MPI_INT, 1, 4, MPI_COMM_WORLD);
               } else {
                  MPI_Send(&message, proc_n, MPI_INT, my_rank+1, 4, MPI_COMM_WORLD);
               }
            }  
         } else if(status.MPI_TAG == 5) {
            printf("Kill Processo [%d]\n", my_rank);
            
            if(my_rank != proc_n - 1) {
               MPI_Send(&message, proc_n, MPI_INT, my_rank+1, 5, MPI_COMM_WORLD);
            }
            break;
         }
      }

      
   }

   MPI_Finalize();
}
