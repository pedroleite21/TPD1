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
	double id;
} CONTA;

static CONTA contas[MAX_CONTAS];
int contaatual = 0;

char *contaAtual;
double numconta;
double valorsaque;
double contacanelar;

double *verifica_conta(char **idConta)
{
	static double ok = 1.0;
	static double erro = -1.0;
	static double valor = 1000.20;
	int i;
	double id;
	float l;
	char pd[20];

	sscanf((*idConta), "%s %f", pd, &l);

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
	}
	else if (strcmp(pd, "saque") == 0)
	{

		sscanf(*idConta, "saque %lf", &valorsaque);

		return &(valorsaque);
	}
	else if (strcmp(pd, "confirmasaque") == 0)
	{

		for (i = 0; i < MAX_CONTAS; i++)
		{
			if (strcmp(contas[i].idConta, contaAtual) == 0)
			{
				if (((contas[i].saldo - valorsaque) >= 0.0) && (l == contas[i].senha))
				{
					contas[i].saldo = contas[i].saldo - valorsaque;
					return &(contas[i].saldo);
				}
			}
		}
		return &(erro);
	}
	else if (strcmp(pd, "deposito") == 0)
	{

		sscanf(*idConta, "deposito %lf", &valorsaque);

		return &(valorsaque);
	}
	else if (strcmp(pd, "abertura") == 0)
	{
		return &(ok);
	}
	else if (strcmp(pd, "id") == 0)
	{
		sscanf(*idConta, "id %lf", &valorsaque);

		i = contaatual;
		numconta = 1.1;
		if (contaatual == 0)
		{
			contas[contaatual].id = valorsaque;
			strcpy(contas[0].idConta, "1.1");
			contas[0].saldo = 0;
			contas[0].senha = 1234;
		}
		else
		{
			for (int j = 0; j < MAX_CONTAS; j++)
			{
				if (contas[j].id == valorsaque)
				{
					return &(erro);
				}
			}

			sscanf(contas[contaatual - 1].idConta, "%lf", &numconta);
			numconta = numconta + 0.1;
			sprintf(contas[contaatual].idConta, "%.1lf", numconta);

			contas[contaatual].id = valorsaque;
			contas[contaatual].saldo = 0;
			contas[contaatual].senha = 1234.0;
		}
		contaatual++;

		return &(numconta);
	}
	else if (strcmp(pd, "cancela") == 0)
	{
		sscanf(*idConta, "cancela %lf", &contacanelar);

		for (int j = 0; j < MAX_CONTAS; j++)
		{
			if (contas[j].id == contacanelar)
			{
				sscanf(contas[j].idConta, "%lf", &numconta);
				return &(numconta);
			}
		}

		return &(erro);
	}
	else if(strcmp(pd, "concancela") == 0) 
	{
		for (int j = 0; j < MAX_CONTAS; j++)
		{
			if (contas[j].id == contacanelar) {
				contas[j].id = 9898.00;
				strcpy(contas[j].idConta, "XXXXXX");
				contas[j].saldo = 98989819819.00;
				contas[j].senha = 9898.00;

				return &(contas[j].saldo);
			}
		}

		return &(erro);
	}
	else if (strcmp(pd, "confirmadeposito") == 0)
	{

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
	}
	else if (strcmp(pd, "agencia") == 0)
	{
		return &(ok);
	}
	else
	{
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
