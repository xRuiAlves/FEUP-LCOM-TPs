# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= lab5
SRCS= lab5.c test5.c video_gr.c vbe.c kbd.c utilities.c video_test.c Sprite.c read_xpm.c timer.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?=/usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
