all:		adm caixa

servidor:	adm.c const.h
		@gcc -o adm adm.c

caixa:		caixa.c	const.h
		@gcc -o caixa caixa.c	

clean:
		@rm -f adm caixa *~
