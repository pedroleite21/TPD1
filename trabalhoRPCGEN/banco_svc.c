/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "banco.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
banco_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		int abreconta_1_arg;
		int fechaconta_1_arg;
		int authconta_1_arg;
		transacao deposito_1_arg;
		transacao saque_1_arg;
		int retornasaldo_1_arg;
		token checksenha_1_arg;
		token gerasenha_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case abreConta:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) abreconta_1_svc;
		break;

	case fechaConta:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) fechaconta_1_svc;
		break;

	case authConta:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) authconta_1_svc;
		break;

	case deposito:
		_xdr_argument = (xdrproc_t) xdr_transacao;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) deposito_1_svc;
		break;

	case saque:
		_xdr_argument = (xdrproc_t) xdr_transacao;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) saque_1_svc;
		break;

	case retornaSaldo:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_float;
		local = (char *(*)(char *, struct svc_req *)) retornasaldo_1_svc;
		break;

	case checkSenha:
		_xdr_argument = (xdrproc_t) xdr_token;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) checksenha_1_svc;
		break;

	case geraSenha:
		_xdr_argument = (xdrproc_t) xdr_token;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) gerasenha_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (BANCO_PROG, BANCO_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, BANCO_PROG, BANCO_VERS, banco_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (BANCO_PROG, BANCO_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, BANCO_PROG, BANCO_VERS, banco_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (BANCO_PROG, BANCO_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}