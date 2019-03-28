#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "const.h"

#define MAX_CONTAS 10

typedef struct {
	char idConta[10];
	double saldo;
} CONTA;

double *verifica_conta(char **idConta)
{
	static CONTA contas[MAX_CONTAS] = {
		{"1-1", 1520.63},
		{"1-2", 63.2}
	};	
	static double ok = 1.0;
	static double erro = -1.0;
	int i;

	for(i=0; i<MAX_CONTAS; i++) {
		if(strcmp(contas[i].idConta, *idConta) == 0) {
			return &(ok);
		}

	}
	return &erro;
}

double *obtem_saldo(char **idConta)
{
	static CONTA contas[MAX_CONTAS] = {
		{"1-1", 1520.63},
		{"1-2", 63.2}
	};	
	static double ok = 1.0;
	static double erro = -1.0;
	int i;

	for(i=0; i<MAX_CONTAS; i++) {
		if(strcmp(contas[i].idConta, *idConta) == 0) {
			return &(contas[i].saldo);
		}

	}
	return &erro;
}

int main() {
	if (registerrpc(VCPROG,VCVERS,VC,verifica_conta, (xdrproc_t)xdr_wrapstring,(xdrproc_t)xdr_double) == -1)  {
		fprintf(stderr,"Erro em registerrpc()!\n");
		exit(1);
	}

	svc_run();
	fprintf(stderr,"Erro em svc_run()!\n");
	return 1;

}


