#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "const.h"

void printOpcoes()
{
	printf("O que deseja fazer?\n\n");
	printf("1 - Verificar Saldo   3 - Realizar Depósito\n");
	printf("2 - Realizar Saque    4 - Sair    \n");
}

int main(int argc, char *argv[]) 
{
	char *pd;
	int stat;
	double saldo;
	float saque;
	char valor[30];

	char *numconta;
	char *server;
	int opcao;
	float senha;

	if (argc != 3) {
		fprintf(stderr, "Uso: \n%s <nome_do_servidor> <conta>\n\n", argv[0]);
		return 1;
	}
	pd = argv[2];
	server = argv[1];
	numconta =  argv[2];
	stat = callrpc(argv[1], VCPROG, VCVERS, VC,
		(xdrproc_t)xdr_wrapstring,(char *)&pd, 
		(xdrproc_t)xdr_double, (char *)&saldo);
	if(stat !=0) {
		clnt_perrno(stat);
		printf("\n");
		return 1;	
	}
	if(saldo < 0.0)
		printf("Resultado: conta não encontrada!\n");
	else {
		printf("/*****************************************/\n");
		printf("/           BEM VINDO AO BANCO            /\n");
		printf("/*****************************************/\n");
		printf("Conta: %s\n\n", numconta);
		printOpcoes();


		while(1) {
			fflush(stdin);
			scanf("%d", &opcao);
			
			switch(opcao) 
			{
				case 1:
					printf("Aguarde...\n");
					pd = "saldo 10.23";
					stat = callrpc(server, VCPROG, VCVERS, VC,
								   (xdrproc_t)xdr_wrapstring, (char *)&pd,
								   (xdrproc_t)xdr_double, (char *)&saldo);

					if(saldo >= 0.0) {
						printf("Seu saldo: R$ %.2f\n\n", saldo);

						printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
					} else {
						printf("Oops... algum erro se deu por aqui");
					}

					break;
				case 2:
					printf("Digite o valor desejado: ");
					scanf("%f", &saque);

					sprintf(valor, "saque %.2f", saque);

					pd = valor;
					stat = callrpc(server, VCPROG, VCVERS, VC,
								   (xdrproc_t)xdr_wrapstring, (char *)&pd,
								   (xdrproc_t)xdr_double, (char *)&saldo);

					printf("Confirmar saque de: R$ %.2f?\nDigite a senha: ", saldo);
					scanf("%f", &senha);

					sprintf(valor, "confirmasaque %.1f", senha);
					pd = valor;
					stat = callrpc(server, VCPROG, VCVERS, VC,
								   (xdrproc_t)xdr_wrapstring, (char *)&pd,
								   (xdrproc_t)xdr_double, (char *)&saldo);

					if(saldo > 0.0) {
						printf("Ok! O seu dinheiro já tá saindo :)\n");
						printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
					} else {
						printf("Oops... algum erro se deu por aqui\n");
						exit(1);
					}
					break;
				case 3:
					printf("Digite o valor desejado: ");
					scanf("%f", &saque);

					sprintf(valor, "deposito %.2f", saque);
					pd = valor;
					stat = callrpc(server, VCPROG, VCVERS, VC,
								   (xdrproc_t)xdr_wrapstring, (char *)&pd,
								   (xdrproc_t)xdr_double, (char *)&saldo);

					printf("Confirmar depósito de: R$ %.2f?\nDigite a senha: ", saldo);
					scanf("%f", &senha);

					sprintf(valor, "confirmadeposito %.1f", senha);
					pd = valor;
					stat = callrpc(server, VCPROG, VCVERS, VC,
								   (xdrproc_t)xdr_wrapstring, (char *)&pd,
								   (xdrproc_t)xdr_double, (char *)&saldo);

					if(saldo > 0.0) {
						printf("Ok! O seu dinheiro já tá na conta :)\n");
						printf("Deseja realizar outra operaçao?\nSe sim, digite 5. Se deseja sair, digite 4.\n");
					} else {
						printf("Oops... algum erro se deu por aqui\n");
						exit(1);
					}
					break;
				case 4:
					exit(1);
				case 5:
					printOpcoes();
					break;
				default:
					printf("Opção inválida!!!\n");

					printOpcoes();
					break;
			}
		}
	}
	return 0;


}
