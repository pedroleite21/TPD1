#include <stdio.h>
#include <rpc/rpc.h>
#include "const.h"

int main(int argc, char *argv[]) 
{
	char *pd;
	int stat;
	double saldo;

	char numconta[10];
	int opcao;
	
	if (argc != 3) {
		fprintf(stderr, "Uso: \n%s <nome_do_servidor> <conta>\n\n", argv[0]);
		return 1;
	}
	pd = argv[2];
	printf("%s", pd);
	stat = callrpc(argv[1], VCPROG, VCVERS, VC,
		(xdrproc_t)xdr_wrapstring,(char *)&pd, 
		(xdrproc_t)xdr_double, (char *)&saldo);
	if(stat !=0) {
		clnt_perrno(stat);
		printf("\n");
		return 1;	
	}
	strcpy(numconta, argv[2]);
	if(saldo < 0.0)
		printf("Resultado: conta não encontrada!\n");
	else {
		printf("/*****************************************/\n");
		printf("/           BEM VINDO AO BANCO POC        /\n");
		printf("/*****************************************/\n");
		printf("Conta: %s\n\n", argv[2]);
		printf("O que deseja fazer?\n\n");
		printf("1 - Verificar Saldo   3 - Realizar Depósito\n");
		printf("2 - Realizar Saque    4 - Sair    \n");	

		while(1) {
			fflush(stdin);
			scanf("%d", &opcao);
			
			switch(opcao) 
			{
				case 1:
					printf("Aguarde...\n");	

					break;
				case 2:
					printf("Realizar Saque");
					break;
				case 3:
					printf("Realizar Depósito");
					break;
				case 4:
					exit(1);
				default:
					printf("Opção inválida!!!\n");
			}
		}
	}
	return 0;


}
