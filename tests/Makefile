PREFIX ?= /usr/local

CFLAGS =-Wall -g -std=c99 -Ideps -I../

handler: *.c
	${CC} ${CFLAGS} -o bin/handler handler.c ../lib/handler.c ../lib/operation.c ../lib/account.c

account: *.c
	${CC} ${CFLAGS} -o bin/account account.c ../lib/handler.c ../lib/operation.c ../lib/account.c

deps:
	clib install silentbicycle/greatest
