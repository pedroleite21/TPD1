#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "const.h"

#define MAX_CONTAS 10

typedef struct
{
	char idConta[10];
	double saldo;
	double senha;
} CONTA;

char *contaAtual;

double valorsaque;

double *verifica_conta(char **idConta)
{
	static CONTA contas[MAX_CONTAS] = {
		{"1-1", 1520.63, 1},
		{"1-2", 63.2, 1}};
	static double ok = 1.0;
	static double erro = -1.0;
	static double valor = 1000.20;
	int i;
	float l;
	char pd[20];

	sscanf((*idConta),"%s %f", pd, &l);

	if (strcmp(pd, "saldo") == 0)
	{
		for (i = 0; i < MAX_CONTAS; i++)
		{
			if (strcmp(contas[i].idConta, contaAtual) == 0)
			{
				return &(contas[i].saldo);
			}
		}
		return &erro;
	} else if(strcmp(pd, "saque") == 0) {
		
		sscanf(*idConta, "saque %lf", &valorsaque);

		return &(valorsaque);
	} else if(strcmp(pd, "confirmasaque") == 0) {
		
		for (i = 0; i < MAX_CONTAS; i++)
		{
			if (strcmp(contas[i].idConta, contaAtual) == 0)
			{
				if (((contas[i].saldo - valorsaque) > 0.0) && (l == contas[i].senha))
				{
					contas[i].saldo = contas[i].saldo - valorsaque;
					return &(contas[i].saldo);
				}
			}
		}
		return &(erro);
	} else if(strcmp(pd, "deposito") == 0) {
		
		sscanf(*idConta, "deposito %lf", &valorsaque);

		return &(valorsaque);
	} else if(strcmp(pd, "confirmadeposito") == 0) {
		
		for (i = 0; i < MAX_CONTAS; i++)
		{
			if (strcmp(contas[i].idConta, contaAtual) == 0)
			{
				if (((contas[i].saldo + valorsaque) > 0.0) && (l == contas[i].senha))
				{
					contas[i].saldo = contas[i].saldo + valorsaque;
					return &(contas[i].saldo);
				}
			}
		}
		return &(erro);
	} else {
		contaAtual = *idConta;
		for (i = 0; i < MAX_CONTAS; i++)
		{
			if (strcmp(contas[i].idConta, *idConta) == 0)
			{
				return &(ok);
			}
		}
		return &erro;
	}
}

int main()
{
	if (registerrpc(VCPROG, VCVERS, VC, verifica_conta, (xdrproc_t)xdr_wrapstring, (xdrproc_t)xdr_double) == -1)
	{
		fprintf(stderr, "Erro em registerrpc()!\n");
		exit(1);
	}

	svc_run();
	fprintf(stderr, "Erro em svc_run()!\n");
	return 1;
}
