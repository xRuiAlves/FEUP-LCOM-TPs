# Makefile for SnakeyNix

COMPILER_TYPE= gnu

CC= gcc

PROG= SnakeyNix
SRCS= SnakeyNix.c utilities.c Bitmap.c video_gr.c vbe.c Game.c Snake.c Apple.c Mouse.c timer.c ps2.c rtc.c font.c StateMachine.c kbd.c Button.c portaserie.c

# Assembky Source Files
SRCS+= assembly_kbd.S assembly_timer.S assembly_ps2.S

CFLAGS= -Wall


DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?=/usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
