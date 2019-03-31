#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "const.h"

void printOpcoes()
{
    printf("O que deseja fazer?\n\n");
    printf("1 - Abertura de conta           3 - Autentificaçao de conta\n");
    printf("2 - Fechamento de conta         4 - Sair\n");
}

int main(int argc, char *argv[])
{
    char *pd;
    int stat;
    double saldo;
    char *server;
    float id;
    char valor[30];
    int opcao;

    if (argc != 2)
    {
        fprintf(stderr, "Uso: \n%s <nome_do_servidor>\n\n", argv[0]);
        return 1;
    }

    server = argv[1];
    pd = "agencia 10.23";
    stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                   (xdrproc_t)xdr_wrapstring, (char *)&pd,
                   (xdrproc_t)xdr_double, (char *)&saldo);
    if (stat != 0 && saldo > 0.0)
    {
        clnt_perrno(stat);
        printf("\n");
        return 1;
    }

    if (saldo < 0.0)
        printf("Nao foi possivel conectar com o banco\n");
    else
    {
        printOpcoes();

        while (1)
        {
            fflush(stdin);
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1:
                pd = "abertura 10.2";
                stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (saldo > 0.0)
                {
                    printf("Digite numero de identificacao:\t");
                    scanf("%f", &id);

                    sprintf(valor, "id %.1f", id);
                    pd = valor;
                    stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                                   (xdrproc_t)xdr_wrapstring, (char *)&pd,
                                   (xdrproc_t)xdr_double, (char *)&saldo);
                    if (saldo > 0.0)
                    {
                        printf("Conta aberta :)\n");
                        printf("Sua conta é: %1.1f\n", saldo);
                        printf("Sua senha é: 1234\n");

                        printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
                    }
                    else
                    {
                        printf("Já existe uma conta com esse id \n");
                        exit(1);
                    }
                }
                break;
            case 2:
                printf("Digite numero de identificacao:\t");
                scanf("%f", &id);

                sprintf(valor, "cancela %.1f", id);
                pd = valor;
                stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (saldo > 0.0)
                {
                    printf("Conta: %.1f\n", saldo);

                    printf("DESEJA CANCELAR?\n1 - Sim\t\t0 - Nao\n");
                    scanf("%f", &id);

                    if (id > 0)
                    {
                        sprintf(valor, "concancela %.1f", saldo);
                        pd = valor;
                        stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                                       (xdrproc_t)xdr_wrapstring, (char *)&pd,
                                       (xdrproc_t)xdr_double, (char *)&saldo);
                        if (saldo > 0.0)
                        {
                            printf("Cancelamento concluido \n\n\n");
                            printOpcoes();
                            break;
                        }
                        else
                        {
                            printf("Ops... encontramos algum problema no sistema");
                            exit(1);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    printf("Nao conseguimos encontrar a conta");
                    exit(1);
                }
                break;
            case 3:
                break;
            case 4:
                exit(1);
            case 5:
                printOpcoes();
                break;
            }
        }
    }
    return 0;
}
