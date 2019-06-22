PREFIX ?= /usr/local

CFLAGS =-Wall -g -std=c99 -Ideps -I../

handler: *.c
	${CC} ${CFLAGS} -o bin/handler handler.c ../handler.c ../operation.c ../account.c

account: *.c
	${CC} ${CFLAGS} -o bin/account account.c ../handler.c ../operation.c ../account.c

deps:
	clib install silentbicycle/greatest
