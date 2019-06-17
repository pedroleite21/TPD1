
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1           // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 100000 // trabalho final com o valores 10.000, 100.000, 1.000.000

#define M 1000
#define N 1000000

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

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

int main()
{
    //int vetor[ARRAY_SIZE];
    int i, j;

    int l = M, c = N;
    //int(*saco)[c] = malloc(l * sizeof *saco);
    int(*vetor) = malloc(c * sizeof(int));

    clock_t begin = clock();
    double time_spent;
    clock_t end;

    //for (i = 0; i < l; i++)
    //for (j = 0; j < c; j++)
    //saco[i][j] = c - j;
    for (j = 0; j < c; j++)
        vetor[j] = c - j;

    // #ifdef DEBUG
    //     printf("\nVetor: ");
    //     for (i = 0; i < ARRAY_SIZE; i++) /* print unsorted array */
    //         printf("[%03d] ", vetor[i]);
    // #endif

    //for (i = 0; i < l; i++)
   // {
        //qsort(vetor, c, sizeof(int), cmpfunc);
        bs(c, vetor);
    //}

    // #ifdef DEBUG
    //     printf("\nVetor: ");
    //     for (i = 0; i < ARRAY_SIZE; i++) /* print sorted array */
    //         printf("[%03d] ", vetor[i]);
    // #endif

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f seconds\n", time_spent);

    free(vetor);

    return 0;
}
