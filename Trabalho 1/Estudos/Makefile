all:		adm caixa agencia

servidor:	adm.c const.h
			@gcc -o adm adm.c

caixa:		caixa.c	const.h
			@gcc -o caixa caixa.c	

agencia: 	agencia.c const.h
		 	@gcc -o agencia agencia.c	

clean:		@rm -f adm caixa *~
