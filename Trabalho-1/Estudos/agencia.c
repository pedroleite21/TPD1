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

void printOpcoes2()
{
    printf("O que deseja fazer?\n\n");
    printf("6 - Verificar Saldo   8 - Realizar Depósito\n");
    printf("7 - Realizar Saque    4 - Sair    \n");
}

int main(int argc, char *argv[])
{
    char *pd;
    int stat;
    double saldo;
    char *server;
    float id;
    float numconta;
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
                printf("Digite numero da conta:\t");
                scanf("%f", &id);
                numconta = id;
                sprintf(valor, "%.1f", id);
                pd = valor;
                stat = callrpc(argv[1], VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (stat != 0)
                {
                    clnt_perrno(stat);
                    printf("\n");
                    return 1;
                }
                if (saldo < 0.0)
                {
                    printf("Resultado: conta não encontrada!\n");
                    exit(1);
                }
                else
                {
                    printf("\n\nConta: %.1f\n\n", numconta);
                    printOpcoes2();
                }
                break;
            case 4:
                exit(1);
            case 5:
                printOpcoes();
                break;
            case 6:
                printf("Aguarde...\n");
                pd = "saldo 10.23";
                stat = callrpc(server, VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (saldo >= 0.0)
                {
                    printf("Seu saldo: R$ %.2f\n\n", saldo);

                    printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
                }
                else
                {
                    printf("Oops... algum erro se deu por aqui");
                }
                break;
            case 7:
                printf("Digite o valor desejado: ");
                scanf("%f", &id);

                sprintf(valor, "saque %.2f", id);

                pd = valor;
                stat = callrpc(server, VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                printf("Confirmar saque de: R$ %.2f?\nDigite a senha: ", saldo);
                scanf("%f", &id);

                sprintf(valor, "confirmasaque %.1f", id);
                pd = valor;
                stat = callrpc(server, VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (saldo > 0.0)
                {
                    printf("Ok! O seu dinheiro já tá saindo :)\n");
                    printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
                }
                else
                {
                    printf("Oops... algum erro se deu por aqui\n");
                    exit(1);
                }
                break;
            case 8:
                printf("Digite o valor desejado: ");
                scanf("%f", &id);

                sprintf(valor, "deposito %.2f", id);
                pd = valor;
                stat = callrpc(server, VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                printf("Confirmar depósito de: R$ %.2f?\nDigite a senha: ", saldo);
                scanf("%f", &id);

                sprintf(valor, "confirmadeposito %.1f", id);
                pd = valor;
                stat = callrpc(server, VCPROG, VCVERS, VC,
                               (xdrproc_t)xdr_wrapstring, (char *)&pd,
                               (xdrproc_t)xdr_double, (char *)&saldo);

                if (saldo > 0.0)
                {
                    printf("Ok! O seu dinheiro já tá na conta :)\n");
                    printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
                }
                else
                {
                    printf("Oops... algum erro se deu por aqui\n");
                    exit(1);
                }
                break;
            }
        }
    }
    return 0;
}
