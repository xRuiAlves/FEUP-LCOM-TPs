# Makefile for lab4

COMPILER_TYPE= gnu

CC= gcc

PROG= lab4
SRCS= lab4.c test4.c ps2.c timer.c utilities.c kbd.c mouseSM.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
