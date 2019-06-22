PREFIX ?= /usr/local

CFLAGS =-Wall -g -std=c99 -D_POSIX_C_SOURCE=2
MAIN ?= server
PORT ?= 8080
BIN  ?= 'bin'

build: make

make: *.c
	${CC} ${CFLAGS} -o ${BIN}/${MAIN} *.c lib/*.c

install: make
	cp ${MAIN} ${PREFIX}/bin/${MAIN}
	chmod u+x ${PREFIX}/bin/${MAIN}

uninstall:
	rm ${PREFIX}/bin/${MAIN}

run:
	${BIN}/${MAIN} ${PORT}

clean:
	rm ${BIN}/${MAIN}
